#include <corecrt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <vadefs.h>
#include "vec.h"

/// Denotes the last argument passed to a variadic function
const Vector VEC_VARIADIC_END = { 285, 3, 456, NULL };

/// Returns a capacity that fits the given amount and is a power of two
size_t _vec_get_p2_cap(size_t amount) {
    if (amount == 0) { return 0; }

    size_t cap = 1;
    for (; cap < amount; cap *= 2) {}
    return cap;
}

/// Returns a capacity that fits the given amount by doubling it continously
size_t _vec_get_cap(size_t cap, size_t amount) {
    if (amount == 0) { return 0; }

    size_t new_cap = cap;
    if (cap == 0) { new_cap = 1; }
    for (; new_cap < amount; new_cap *= 2) {}
    return new_cap;
}

/// Drops the `data` pointer
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
///
/// While using this function you must ensure that:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
void vec_drop_single_unchecked(Vector *vec) {
    free(vec->data);
    vec->data = NULL;
    vec->cap = 0;
    vec->len = 0;
    vec->size = 0;
}

/// Drops the `data` pointer
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
VEC_DROP_RESULT vec_drop_single(Vector *vec) {
    if (vec == NULL) { return VDR_INVALID_VEC; }
    if (vec->data == NULL) { return VDR_INVALID_VEC_DATA; }
    vec_drop_single_unchecked(vec);
    return VDR_OK;
}

/// Drops the `data` pointer
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
///
/// While using this function you must ensure that each:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
void _vec_drop_unchecked(Vector *vec, ...) {
    vec_drop_single_unchecked(vec);

    va_list args;
    va_start(args, vec);
    
    while (true) {
        Vector *arg = va_arg(args, Vector*);
        if (arg != NULL) {
            if (vec_is_eq_unchecked(arg, (Vector*)&VEC_VARIADIC_END)) { break; }
        }
        vec_drop_single_unchecked(arg);
    }

    va_end(args);
}

/// Drops the `data` pointer
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
VEC_DROP_RESULT _vec_drop(size_t *err_index, Vector *vec, ...) {
    VEC_DROP_RESULT result = vec_drop_single(vec);
    if (result != VDR_OK) { if (err_index != NULL) { *err_index = 0; }; return result; }

    va_list args;
    va_start(args, vec);
    
    for (size_t i = 1; true; i++) {
        Vector *arg = va_arg(args, Vector*);
        if (arg != NULL) {
            if (vec_is_eq_unchecked(arg, (Vector*)&VEC_VARIADIC_END)) { break; }
        }

        result = vec_drop_single(arg);
        if (result != VDR_OK) { if (err_index != NULL) { *err_index = i; }; return result; }
    }

    va_end(args);
    return VDR_OK;
}

/// Reserves a specific capacity for the vector
///
/// WARNING! Remember to free nested pointers with allocated
/// data if you're setting the capacity to `0` or you'll leak memory!
///
/// You must ensure that:
/// - `vec` is not NULL
void vec_reserve_unchecked(Vector *vec, size_t cap) {
    vec->cap = cap;

    if (cap == 0) {
        free(vec->data);
        vec->data = NULL;
        vec->cap = 0;
        vec->len = 0;
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
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
VEC_RESERVE_RESULT vec_reserve(Vector *vec, size_t cap) {
    if (vec == NULL) { return VRR_INVALID_VEC; }
    vec_reserve_unchecked(vec, cap);

    return VRR_OK;
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

    // Allocate the memory
    vec->data = malloc(vec->cap * vec->size);

    // Copy the data if it's valid
    if (data != NULL) {
        vec->len = amount;
        memcpy(vec->data, data, vec->size * amount);
    }

    return VIR_OK;
}

/// Create a new `Vector` with the given data
/// The amount will be alligned to set the capacity as a power of 2
Vector vec_new(size_t size, void *data, size_t amount) {
    Vector vec;
    vec_init(&vec, size, data, amount);
    return vec;
}

// TODO: make inline
/// Returns a pointer to the element of the specified index
///
/// You must ensure that:
/// - `vec` is not NULL
/// - `vec->data` is not NULL
/// - `index` is not out of bounds
///
/// It's also a good idea to ensure that:
/// - `vec->size` is not 0
void *vec_get_unchecked(Vector *vec, size_t index) {
    return vec->data + (vec->size * index);
}

/// Returns a pointer to the element of the specified index or NULL if:
///
/// - `vec` is NULL
/// - `vec->data` is NULL
/// - `vec->size` is 0
/// - `index` is out of bounds
void *vec_get(Vector *vec, size_t index) {
    if (vec == NULL) { return NULL; }
    if (vec->data == NULL) { return NULL; }
    if (vec->size == 0) { return NULL; }
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
            vec_reserve_unchecked(vec, vec->cap * 2);
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

    // Double the capacity until it fits
    if (vec->len > vec->cap) {
        vec_reserve_unchecked(vec, _vec_get_cap(vec->cap, vec->len));
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

void vec_remove_unchecked(Vector *vec, size_t index) {
    vec->len--;
    if (index == vec->len) { return; }

    memcpy(vec_get_unchecked(vec, index), vec_get_unchecked(vec, index+1), vec->size * (vec->len+1 - index+1));
}

VEC_REMOVE_RESULT vec_remove(Vector *vec, size_t index) {
    if (vec == NULL) { return VRER_INVALID_VEC; }
    if (vec->data == NULL) { return VRER_INVALID_VEC_DATA; }
    if (index >= vec->len) { return VRER_OUT_OF_BOUNDS; }

    vec->len--;
    if (index == vec->len) { return VRER_OK; }

    memcpy(vec_get_unchecked(vec, index), vec_get_unchecked(vec, index+1), vec->size * (vec->len+1 - index+1));
    return VRER_OK;
}

void vec_remove_range_unchecked(Vector *vec, size_t beg, size_t end) {
    vec->len -= end - beg;
    if (end == vec->len) { return; }
    memcpy(vec_get_unchecked(vec, beg), vec_get_unchecked(vec, end), vec->size * vec->len);
}

VEC_REMOVE_RANGE_RESULT vec_remove_range(Vector *vec, size_t beg, size_t end) {
    if (vec == NULL) { return VRERR_INVALID_VEC; }
    if (vec->data == NULL) { return VRERR_INVALID_VEC_DATA; }
    if (end < beg) { return VRERR_INVALID_BOUNDS; }
    if (end > vec->len || beg >= vec->len) { return VRERR_OUT_OF_BOUNDS; }

    vec_remove_range_unchecked(vec, beg, end);
    return VRERR_OK;
}

void vec_remove_normalized_ranges_unchecked(Vector *vec, size_t *ranges, size_t amount) {
    if (amount == 0) { return; }
    size_t initial_len = vec->len;
    size_t current_data_end = ranges[0];

    for (size_t i = 2; i < amount*2; i+=2) {
        size_t beg = ranges[i-2];
        size_t end = ranges[i-1];
        size_t next_beg = ranges[i];
        vec->len -= end - beg;
        //printf("MHM: %lld o: %lld", end-beg, current_data_end);
        memcpy(vec_get_unchecked(vec, current_data_end), vec_get_unchecked(vec, end), vec->size * (next_beg - end));
        current_data_end += next_beg - end;
    }

    size_t beg = ranges[amount*2-2];
    size_t end = ranges[amount*2-1];
    vec->len -= end - beg;
    //printf("```%lld %lld```", end, initial_len);
    if (end == initial_len) { return; }
    //printf("MHM: %lld from: %lld %lld %lld", end-beg, end, beg, current_data_end);
    memcpy(vec_get_unchecked(vec, current_data_end), vec_get_unchecked(vec, end), vec->size * (initial_len - end));
}

VEC_REMOVE_NORMALIZED_RANGES_RESULT vec_remove_normalized_ranges(Vector *vec, size_t *ranges, size_t amount) {
    if (vec == NULL) { return VRENRR_INVALID_VEC; }
    if (vec->data == NULL) { return VRENRR_INVALID_VEC_DATA; }
    if (ranges == NULL) { return VRENRR_INVALID_RANGES; }

    // Check if the ranges are normalized
    size_t highest_seen = ranges[0];
    for (size_t i = 1; i < amount*2; i++) {
        if (ranges[i] <= highest_seen) { return VRENRR_NON_NORMALIZED_RANGES; }
        highest_seen = ranges[i];
    }

    vec_remove_normalized_ranges_unchecked(vec, ranges, amount);
    return VRENRR_OK;
}

// TODO: Automatic normalization. For now I won't implement it as there are other functions that will be usefull while implementing this one.
//Vector _vec_normalize_ranges(size_t *ranges, size_t amount) {
//    Vector normalized_ranges = vec_new(sizeof(size_t), ranges, amount*2);
//    return normalized_ranges;
//}
//void vec_remove_ranges_unchecked(Vector *vec, size_t *ranges, size_t amount) { }
//VEC_REMOVE_RANGE_RESULT vec_remove_ranges(Vector *vec, size_t *ranges, size_t amount) { return VRERR_OK; }

/// Performs a binary search on the given vector
///
/// The `comp` function will get passed the currently checked
/// element of a vector, and an additional `searched` parameter
/// given by the user.
///
/// `beg` and `end` are boundries of indexes of where the search
/// will be performed, `<beg,end)`.
///
/// `index` will be assigned to the searched value index if found
/// otherwise it's left as is.
VEC_SEARCH_RESULT vec_binary_search(Vector *vec, VEC_BINARY_SEARCH_COMP_RESULT (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched) {
    if (vec == NULL) { return VSR_INVALID_VEC; }
    if (comp == NULL) { return VSR_INVALID_COMP; }
    if (searched == NULL) { return VSR_INVALID_SEARCHED; }
    if (vec->len < end || vec->len < beg) { return VSR_OUT_OF_BOUNDS; }
    if (beg >= end) { return VSR_INVALID_BOUNDS; }
    size_t new_beg = beg;
    size_t new_end = end;

    while (true) {
        size_t middle = (new_end + new_beg) / 2;
        char* current_char = vec->data + (vec->size * middle);

        if (middle == new_end || middle == new_beg) {
            VEC_BINARY_SEARCH_COMP_RESULT result = comp(current_char, searched);
            if (result == VBSCR_FOUND) { *index = middle; return VSR_OK; }
            return VSR_NOT_FOUND;
        }

        switch (comp(current_char, searched)) {
            case VBSCR_LEFT: { new_end = middle; break; }
            case VBSCR_FOUND: { *index = middle; return VSR_OK; }
            case VBSCR_RIGHT: { new_beg = middle; break; }
        }
    }

    return VSR_NOT_FOUND;
}

/// Comp function of binary search for `ints`
VEC_BINARY_SEARCH_COMP_RESULT vbsc_int(void *current_num, void *searched_num) {
    if (*(int*)current_num > *(int*)searched_num) { return VBSCR_LEFT; }
    if (*(int*)current_num < *(int*)searched_num) { return VBSCR_RIGHT; }
    if (*(int*)current_num == *(int*)searched_num) { return VBSCR_FOUND; }
    return -1;
}

/// Comp function of binary search for end of line in an zeroed out (from the right side) buffer
VEC_BINARY_SEARCH_COMP_RESULT vbsc_rf(void *current_char, void *_) {
    switch (*(char*)current_char) {
        case '\n': { return VBSCR_FOUND; }
        case 0: { return VBSCR_LEFT; }
        default: { return VBSCR_RIGHT; }
    }
}

// Comp function for `ints`
bool vc_int(void *current_num, void *searched_num) {
    return (*(int*)current_num == *(int*)searched_num);
}

// Comp function for `chars`
bool vc_char(void *current_num, void *searched_num) {
    return (*(char*)current_num == *(char*)searched_num);
}

/// Performs a linear search on the given vector, returning the
/// first element it finds.
///
/// The `comp` function will get passed the currently checked
/// element of a vector, and an additional `searched` parameter
/// given by the user.
///
/// `beg` and `end` are boundries of indexes of where the search
/// will be performed, `<beg,end)`.
VEC_SEARCH_RESULT vec_find_first(Vector *vec, bool (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched) {
    if (vec == NULL) { return VSR_INVALID_VEC; }
    if (comp == NULL) { return VSR_INVALID_COMP; }
    if (searched == NULL) { return VSR_INVALID_SEARCHED; }
    if (vec->len < end || vec->len < beg) { return VSR_OUT_OF_BOUNDS; }
    if (beg >= end) { return VSR_INVALID_BOUNDS; }

    for (size_t x = beg; x < end; x++) {
        if (comp(vec_get_unchecked(vec, x), searched)) {
            if (index != NULL) { *index = x; }
            return VSR_OK;
        }
    }

    return VSR_NOT_FOUND;
}

// Checks if `len`, `cap` and `size` of a vector are equal
bool vec_is_partial_eq_unchecked(Vector *vec1, Vector *vec2) {
    return (
        vec1->cap == vec2->cap
        && vec1->len == vec2->len
        && vec1->size == vec2->size
    );
}

bool vec_is_partial_eq(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result) {
    if (vec1 == NULL) { if (result != NULL) { *result = VER_INVALID_SOURCE; }; return false; }
    if (vec2 == NULL) { if (result != NULL) { *result = VER_INVALID_DESTINATION; }; return false; }
    *result = VER_OK;
    return vec_is_partial_eq_unchecked(vec1, vec2);
}

// Checks if `len`, `cap`, `size` and `data` pointer of a vector are equal
bool vec_is_eq_unchecked(Vector *vec1, Vector *vec2) {
    return (
        vec_is_partial_eq_unchecked(vec1, vec2)
        && vec1->data == vec2->data
    );
}

bool vec_is_eq(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result) {
    if (vec1 == NULL) { if (result != NULL) { *result = VER_INVALID_SOURCE; }; return false; }
    if (vec2 == NULL) { if (result != NULL) { *result = VER_INVALID_DESTINATION; }; return false; }
    return vec_is_partial_eq_unchecked(vec1, vec2);
}

// Checks if `len`, `cap`, `size` and `data` the vectors are pointing to are equal.
// The pointers might differ, only the similiarity of the data being held is checked.
bool vec_is_eq_deep_unchecked(Vector *vec1, Vector *vec2) {
    if (!vec_is_partial_eq_unchecked(vec1, vec2)) { return false; }

    return (memcmp(vec1->data, vec2->data, vec1->size * vec1->len) == 0);
}

bool vec_is_eq_deep(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result) {
    if (vec1 == NULL) { if (result != NULL) { *result = VER_INVALID_SOURCE; }; return false; }
    if (vec2 == NULL) { if (result != NULL) { *result = VER_INVALID_DESTINATION; }; return false; }
    // TODO: Handle NULL `data` pointer
    return vec_is_eq_deep_unchecked(vec1, vec2);
}

// Copies the given 
Vector vec_copy_unchecked(Vector *vec) {
    Vector copy;

    vec_init(&copy, vec->size, vec->data, vec->len);
    vec_reserve_unchecked(&copy, vec->cap);

    return copy;
}

VEC_COPY_RESULT vec_copy(Vector *source_vec, Vector *destination_vec) {
    if (source_vec == NULL) { return VCR_INVALID_SOURCE; }
    if (destination_vec == NULL) { return VCR_INVALID_DESTINATION; }

    *destination_vec = vec_copy_unchecked(source_vec);

    if (!vec_is_partial_eq_unchecked(source_vec, destination_vec)) { return VCR_UNKNOWN; }
    return VCR_OK;
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

            // TODO: Full error handling
            switch (vec_binary_search(&line, vbsc_rf, line.cap/2, line.cap, &index, NULL)) {
                case VSR_OK: { vec_reserve_unchecked(&line, line.cap - 2); goto line_read; }
                case VSR_NOT_FOUND: { break; }
                case VSR_INVALID_BOUNDS: { printf("ERROR: Invalid input."); break; }
                case VSR_OUT_OF_BOUNDS: { printf("ERROR: Out of bounds."); break; }
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
    if (vec == NULL) { fprintf(stderr, "%sWARNING: Could not print `vector` because the value passed was NULL.%s", CMD_ESC_YELLOW, CMD_ESC_RESET); return; }
    printf("{ len: %lld, cap: %lld, size: %lld, data: ", vec->len, vec->cap, vec->size);

    if (vec->data != NULL) {
        printf("%p", vec->data);
    } else {
        printf("NULL");
    }

    printf(" }");
}

static inline void p_vec_fprint_int(FILE *file, void *data) { fprintf(file, "%d", *(int*)data); }
static inline void p_vec_fprint_string(FILE *file, void *data) { fprintf(file, "\"%s\"", (char*)data); }

void p_vec_fprint(FILE *file, Vector *vec, P_VEC_PRINT_TYPE type) {
    void (*print_func)(FILE*, void*);
    switch (type) {
        // TODO: Implement the rest of the types
        case PVPT_STRING: { print_func = p_vec_fprint_string; break; }
        case PVPT_INT: { print_func = p_vec_fprint_int; break; }
    }

    fprintf(file, "[");
    for (int i = 0; i < vec->len - 1; i++) {
        print_func(file, vec_get(vec, i));
        fprintf(file, ", ");
    }
    print_func(file, vec_get(vec, vec->len-1));
    fprintf(file, "]");
}

void p_vec_print(Vector *vec, P_VEC_PRINT_TYPE type) {
    p_vec_fprint(stdout, vec, type);
}

// TODO: `p_vec_print_custom` for custom types
