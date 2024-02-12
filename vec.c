#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vec.h"

/// Returns a capacity that fits the given amount and is a power of two
size_t _vec_get_p2_cap(size_t amount) {
    if (amount == 0) { return 0; }

    size_t cap = 1;
    for (; cap < amount; cap *= 2) {}
    return cap;
}

/// Drops the pointer to data
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
void vec_drop(Vector *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->cap = 0;
    vec->len = 0;
}

/// Reserves a specific capacity for the vector
///
/// You must ensure that:
/// - `vec` is not NULL
void vec_reserve_unchecked(Vector *vec, size_t cap) {
    vec->cap = cap;

    if (cap == 0) {
        vec_drop(vec);
        return;
    }

    if (vec->data == NULL) {
        vec->data = malloc(vec->cap * vec->size);
    } else {
        vec->data = realloc(vec->data, vec->cap * vec->size);
    }

    if (vec->len > vec->cap) { vec->len = vec->cap; }
}
/// Reserves a specific capacity for the vector
VEC_RESERVE_RESULT vec_reserve(Vector *vec, size_t cap) {
    if (vec == NULL) { return VRR_INVALID_VEC; }
    vec_reserve_unchecked(vec, cap);

    return VRR_OK;
}

/// Reallocates double of the current capacity
void _vec_double(Vector *vec) {
    vec_reserve_unchecked(vec, vec->cap * 2);
}

/// Initializes the vector with the given data if any
VEC_INIT_RESULT vec_init(Vector *vec, size_t size, void *data, size_t amount) {
    if (vec == NULL) { return VIR_INVALID_VEC; }

    // Assign the default data
    vec->len = 0;
    vec->cap = 0;
    vec->size = size;
    vec->data = NULL;

    // If `size` is 0, everything else should be 0/NULL
    if (size == 0) { return 0; }

    // If the `amount` is 0, `data`, `len` and `cap` should be 0/NULL
    if (amount == 0) { return 0; }

    // Otherwise, we need to allocate a power of two for the `cap` and set the `len` equal to `amount`
    vec->cap = _vec_get_p2_cap(amount);
    vec->len = amount;

    // If provided `data` is valid
    if (data != NULL) {
        // Allocate the memory
        vec->data = malloc(vec->cap * vec->size);

        if (vec->data == NULL) {
            vec->len = 0;
            return 1;
        }

        // Copy the `data` into the `Vector`
        memcpy(vec->data, data, vec->size * amount);
    } else {
        vec->len = 0;
    };

    return VIR_OK;
}

/// Create a new `Vector` with the given data
Vector vec_new(size_t size, void *data, size_t amount) {
    Vector vec;
    vec_init(&vec, size, data, amount);
    return vec;
}

/// Returns a pointer to the element of the specified index
///
/// You must ensure that:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
/// - `index` is not out of bounds
void *vec_get_unchecked(Vector *vec, size_t index) {
    return vec->data + (vec->size * index);
}

/// Returns a pointer to the element of the specified index or NULL if:
///
/// - `vec` is NULL
/// - `vec->data` is NULL
/// - `index` is out of bounds
void *vec_get(Vector *vec, size_t index) {
    if (vec == NULL) { return NULL; }
    if (vec->data == NULL) { return NULL; }
    if (index > vec->len-1) { return NULL; }
    return vec_get_unchecked(vec, index);
}

/// Pushes the provided data as the last element of the vector
///
/// You must ensure that:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
/// - `data` is not NULL
void vec_push_unchecked(Vector *vec, void *data) {
    ++vec->len;

    // Double the capacity if there is not enough space
    if (vec->len > vec->cap) {
        if (vec->cap == 0) {
            vec_reserve_unchecked(vec, 1);
        } else {
            _vec_double(vec);
        }
    }

    // Copy the data
    memcpy(vec_get_unchecked(vec, vec->len - 1), data, vec->size);
}

/// Pushes the provided data as the last element of the vector
VEC_PUSH_RESULT vec_push(Vector *vec, void *data) {
    if (vec == NULL) { return VPR_INVALID_VEC; }
    if (vec->data == NULL) { return VPR_INVALID_VEC_DATA; }
    if (data == NULL) { return VPR_INVALID_DATA; }
    vec_push_unchecked(vec, data);

    return VPR_OK;
}

/// Pushes the provided data as the last elements of the vector
///
/// You must ensure that:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
/// - `data` is not NULL
void vec_push_multi_unchecked(Vector *vec, void *data, size_t amount) {
    if (amount == 0) { return; }
    vec->len += amount;

    // Change the capacity if there is not enough space
    if (vec->len > vec->cap) {
        vec_reserve_unchecked(vec, _vec_get_p2_cap(vec->len));
    }

    // Copy the data
    memcpy(vec_get_unchecked(vec, vec->len - amount), data, vec->size * amount);
}

/// Pushes the provided data as the last elements of the vector
VEC_PUSH_RESULT vec_push_multi(Vector *vec, void *data, size_t amount) {
    if (vec == NULL) { return VPR_INVALID_VEC; }
    if (vec->data == NULL) { return VPR_INVALID_VEC_DATA; }
    if (data == NULL) { return VPR_INVALID_DATA; }
    vec_push_multi_unchecked(vec, data, amount);

    return 0;
}

VEC_BINARY_SEARCH_RESULT vec_binary_search(Vector *vec, VEC_BINARY_SEARCH_COMP_RESULT (*comp)(void *vec_item), size_t beg, size_t end, size_t *found) {
    if (vec->len < end || vec->len < beg) { return VBSR_OUT_OF_BOUNDS; }
    if (beg > end) { return VBSR_INVALID_INPUT; }
    size_t new_beg = beg;
    size_t new_end = end;

    while (true) {
        size_t middle = (new_end + new_beg) / 2;
        char* current_char = vec->data + (vec->size * middle);

        if (middle == new_end || middle == new_beg) { return VBSR_NOT_FOUND; }

        switch (comp(current_char)) {
            case VBSCR_LEFT: { new_end = middle; break; }
            case VBSCR_FOUND: { *found = middle; return VBSR_OK; }
            case VBSCR_RIGHT: { new_beg = middle; break; }
            default: { return VBSR_COMP_INVALID_OUTPUT; }
        }
    }

    return VBSR_NOT_FOUND;
}

VEC_BINARY_SEARCH_COMP_RESULT vec_rf_comp(void *character) {
    switch (*(char*)character) {
        case '\n': { return VBSCR_FOUND; }
        case 0: { return VBSCR_LEFT; }
        default: { return VBSCR_RIGHT; }
    }
}

COMP_FUNC_RET vec_find_first(Vector *vec, bool (*comp)(void *vec_item), size_t beg, size_t end, size_t *index) {
    if (beg > end) { return CF_INVALID_INPUT; }
    if (end > vec->len-1) { return CF_OUT_OF_BOUNDS; }

    for (size_t x = beg; x <= end; x++) {
        if (comp(vec_get_unchecked(vec, x))) {
            *index = x;
            return CF_OK;
        }
    }

    return CF_NOT_FOUND;
}

COMP_FUNC_RET vec_contains(Vector *vec, bool (*comp)(void *vec_item, void *provided_item), void *item, size_t beg, size_t end, size_t *index) {
    if (beg > end) { return CF_INVALID_INPUT; }
    if (end > vec->len-1) { return CF_OUT_OF_BOUNDS; }

    for (size_t x = beg; x <= end; x++) {
        if (comp(vec_get_unchecked(vec, x), item)) {
            //*index = x;
            return CF_OK;
        }
    }

    return CF_NOT_FOUND;
}

Vector vec_copy(Vector *vec) {
    Vector copy;
    vec_init(&copy, vec->size, vec->data, vec->len);
    // TODO: Check if this is needed
    vec_reserve_unchecked(&copy, vec->cap);
    return copy;
}

void vec_swap_unchecked(Vector *vec, size_t first_index, size_t second_index) {
    // Make a copy of the value at the first index
    void *first_element_copy = malloc(vec->size);
    memcpy(first_element_copy, vec_get(vec, first_index), vec->size);

    // Swap the elemnts
    memcpy(vec_get(vec, first_index), vec_get(vec, second_index), vec->size);
    memcpy(vec_get(vec, second_index), first_element_copy, vec->size);
    free(first_element_copy);
}

// TODO: Implement errors
void vec_swap(Vector *vec, size_t first_index, size_t second_index) {
    if (first_index < 0 || first_index >= vec->len || second_index < 0 || second_index >= vec->len) { return; }
    vec_swap_unchecked(vec, first_index, second_index);
}

int vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize) {
    FILE *file;
    int err = 0;
    void *data;

    err = fopen_s(&file, file_name, "r");
    if (err != 0) { printf("ERROR: %d\n", err); }

    vec_init(vec, sizeof(Vector), NULL, 0);
    while (true) {
        Vector line;
        size_t index = 0;
        vec_init(&line, sizeof(char), NULL, 4);

        for (int i = 0;; i++) {
            char* fgets_ret;

            size_t shift = line.cap - 1;
            vec_reserve_unchecked(&line, ((line.cap - 2) * 2) + 2);
            size_t cap = shift;
            if (i == 0) { shift = 0; cap = line.cap; }
            fgets_ret = fgets(line.data + shift * line.size, cap, file);

            // Read failed
            if (fgets_ret == NULL) {
                // Check for errors
                err = ferror(file);
                if (err != 0) { printf("ERROR: %d", err); }

                // Check for EOF 
                if (feof(file) != 0) {
                    goto file_read;
                }
            }

            // Set the lenght
            line.len = line.cap;

            switch (vec_binary_search(&line, &vec_rf_comp, line.cap/2, line.cap, &index)) {
                case VBSR_OK: { vec_reserve_unchecked(&line, line.cap - 2); goto line_read; }
                case VBSR_NOT_FOUND: { break; }
                case VBSR_INVALID_INPUT: { printf("ERROR: Invalid input."); break; }
                case VBSR_OUT_OF_BOUNDS: { printf("ERROR: Out of bounds."); break; }
                case VBSR_COMP_INVALID_OUTPUT: { printf("ERROR: Comp invalid output."); break; }
                default: { printf("ERROR: In binary search"); break; }
            }
        }

    line_read:
        line.len = index;
        vec_push(vec, &line);
    }

file_read:
    fclose(file);
    return err;
}

void p_vec_info(Vector *vec) {
    printf("Len: %d\n", (int)vec->len);
    printf("Cap: %d\n", (int)vec->cap);
    printf("Size: %d\n", (int)vec->size);
}

// TODO: 
//typedef enum {
//    PVPT_INT,
//    PVPT_CHAR,
//    ...
//} P_VEC_PRINT_TYPE;

void p_vec_print(Vector *vec, char type) {
    // Print opening tag
    printf("[");

    // Print the elements
    for (int i = 0; i < vec->len - 1; i++) {
        char printfTemplate[5] = {'%', type, ',', ' ', '\0'};
        printf(printfTemplate, *(int*)(vec_get(vec, i)));
    }

    // Print the last element without a `, ` and print a closing tag
    printf("%d]\n", *(int*)(vec_get(vec, vec->len - 1)));
}

// TODO: `p_vec_print_custom` for custom types
