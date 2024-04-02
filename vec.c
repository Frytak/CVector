#include <corecrt.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <vadefs.h>
#include "vec.h"

// TODO: Better documentation
// TODO: Fix tests
//     - No full error handling
//     - Some test are lacking

VecConfig VEC_CONFIG = {
    false, // bool PRINT_WITH_COLORS
    false, // bool FPRINT_WITH_COLORS
    32, // uint8_t TIMSORT_RUN_SIZE
};

bool vec_cg_print_colors() { return VEC_CONFIG.PRINT_WITH_COLORS; }
void vec_cs_print_colors(bool set) { VEC_CONFIG.PRINT_WITH_COLORS = set; }

bool vec_cg_fprint_colors() { return VEC_CONFIG.FPRINT_WITH_COLORS; }
void vec_cs_fprint_colors(bool set) { VEC_CONFIG.FPRINT_WITH_COLORS = set; }

uint8_t vec_cg_timsort_run_size() { return VEC_CONFIG.TIMSORT_RUN_SIZE; }
void vec_cs_timsort_run_size(uint8_t set) { VEC_CONFIG.TIMSORT_RUN_SIZE = set; }

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
void vec_drop_single(Vector *vec) {
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
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
VEC_RESULT vec_drop_single_s(Vector *vec) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    vec_drop_single(vec);
    return VECR_OK;
}

/// Drops the `data` pointer for each vector.
/// The last vector must be `VEC_VARIADIC_END`, that's why this
/// function shouldn't be used directly, use the provided `vec_drop` macro.
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
void _vec_drop(Vector *vec, ...) {
    vec_drop_single(vec);

    va_list args;
    va_start(args, vec);
    
    while (true) {
        Vector *arg = va_arg(args, Vector*);
        if (arg != NULL) {
            if (vec_is_eq(arg, (Vector*)&VEC_VARIADIC_END)) { break; }
        }
        vec_drop_single(arg);
    }

    va_end(args);
}

/// Drops the `data` pointer for each vector.
/// The last vector must be `VEC_VARIADIC_END`, that's why this
/// function shouldn't be used directly, use the provided `vec_drop_s` macro.
/// If one of the vectors is invalid the result won't be successfull
/// but all valid vectors will be dropped.
///
/// WARNING! Remember to free nested pointers with allocated
/// data or you'll leak memory!
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`) 
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
VEC_RESULT _vec_drop_s(Vector *vec, ...) {
    VEC_RESULT result = vec_drop_single_s(vec);

    va_list args;
    va_start(args, vec);
    
    for (size_t i = 1; true; i++) {
        Vector *arg = va_arg(args, Vector*);
        if (arg != NULL) {
            if (vec_is_eq(arg, (Vector*)&VEC_VARIADIC_END)) { break; }
        }

        VEC_RESULT result2 = vec_drop_single_s(arg);
        if (result == VECR_OK) {
            result = result2;
        }
    }

    va_end(args);
    return result;
}

/// Reserves a specific capacity for the vector
///
/// WARNING! Remember to free nested pointers with allocated
/// data if you're setting the capacity to `0` or you'll leak memory!
void vec_reserve(Vector *vec, size_t cap) {
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
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
VEC_RESULT vec_reserve_s(Vector *vec, size_t cap) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    vec_reserve(vec, cap);

    return VECR_OK;
}

// Doubles the capacity of a vector, or assigns `1` if the capacity is `0`.
void _vec_reserve_double(Vector *vec) {
    if (vec->cap == 0) {
        vec_reserve(vec, 1);
    } else {
        vec_reserve(vec, vec->cap * 2);
    }
}

// TODO: vec_init and vec_init_s
/// Initializes the vector with the given data if any
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
VEC_RESULT vec_init(Vector *vec, size_t size, void *data, size_t amount) {
    if (vec == NULL) { return VECR_NULL_VEC; }

    // Assign the default data
    vec->len = 0;
    vec->cap = 0;
    vec->size = size;
    vec->data = NULL;

    // If `size` is 0, everything else should be 0/NULL
    if (size == 0) { return VECR_OK; }

    // If the `amount` is 0, `data`, `len` and `cap` should be 0/NULL
    if (amount == 0) { return VECR_OK; }

    // Otherwise, we need to allocate a power of two for the `cap` and set the `len` equal to `amount`
    vec->cap = _vec_get_p2_cap(amount);

    // Allocate the memory
    vec->data = malloc(vec->cap * vec->size);

    // Copy the data if it's valid
    if (data != NULL) {
        vec->len = amount;
        memcpy(vec->data, data, vec->size * amount);
    }

    return VECR_OK;
}

// TODO: vec_new and vec_new_s
/// Create a new `Vector` with the given data
/// The amount will be alligned to set the capacity as a power of 2
Vector vec_new(size_t size, void *data, size_t amount) {
    Vector vec;
    vec_init(&vec, size, data, amount);
    return vec;
}

/// Returns a pointer to the element of the specified index
inline void *vec_get(Vector *vec, size_t index) {
    return vec->data + (vec->size * index);
}

/// Returns a pointer to the element of the specified index or NULL if:
///     - `vec` is NULL
///     - `vec->data` is NULL
///     - `vec->size` is 0
///     - `index` is out of bounds
void *vec_get_s(Vector *vec, size_t index) {
    if (vec == NULL) { return NULL; }
    if (vec->data == NULL) { return NULL; }
    if (vec->size == 0) { return NULL; }
    if (index > vec->len-1) { return NULL; }
    return vec_get(vec, index);
}

/// Pushes the provided data as the last element of the vector.
/// Doesn't check if there is enough capacity and won't increase it.
void vec_force_push(Vector *vec, void *data) {
    vec->len++;
    memcpy(vec_get(vec, vec->len - 1), data, vec->size);
}

/// Pushes the provided data as the last element of the vector.
/// Doesn't check if there is enough capacity and won't increase it.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `data` is not NULL (`VECR_NULL_DATA`)
VEC_RESULT vec_force_push_s(Vector *vec, void *data) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { VECR_NULL_VEC_DATA; }
    if (data == NULL) { return VECR_NULL_DATA; }
    vec_force_push(vec, data);
    return VECR_OK;
}

/// Pushes the provided data as the last element of the vector.
void vec_push(Vector *vec, void *data) {
    vec->len++;

    if (vec->len > vec->cap) {
        _vec_reserve_double(vec);
    }

    memcpy(vec_get(vec, vec->len - 1), data, vec->size);
}

/// Pushes the provided data as the last element of the vector.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `data` is not NULL (`VECR_NULL_DATA`)
VEC_RESULT vec_push_s(Vector *vec, void *data) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (data == NULL) { return VECR_NULL_DATA; }
    vec_push(vec, data);

    return VECR_OK;
}

/// Pushes the provided data as the last elements of the vector.
void vec_push_multi(Vector *vec, void *data, size_t amount) {
    if (amount == 0) { return; }
    vec->len += amount;

    // Double the capacity until it fits
    if (vec->len > vec->cap) {
        vec_reserve(vec, _vec_get_cap(vec->cap, vec->len));
    }

    // Copy the data
    memcpy(vec_get(vec, vec->len - amount), data, vec->size * amount);
}

/// Pushes the provided data as the last elements of the vector.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `data` is not NULL (`VECR_NULL_DATA`)
VEC_RESULT vec_push_multi_s(Vector *vec, void *data, size_t amount) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (data == NULL) { return VECR_NULL_DATA; }
    vec_push_multi(vec, data, amount);

    return VECR_OK;
}

/// Inserts the provided data in the specified index of the vector.
void vec_insert(Vector *vec, size_t index, void *data) {
    // Copy the `data` as it might exist in the vector, we don't want to rewrite it while shifting
    void *value_copy = malloc(vec->size);
    memcpy(value_copy, data, vec->size);

    vec->len++;
    if (vec->cap < vec->len) {
        _vec_reserve_double(vec);
    }

    memcpy(vec_get(vec, index+1), vec_get(vec, index), vec->size*(vec->len-index));
    memcpy(vec_get(vec, index), value_copy, vec->size);
    free(value_copy);
}

/// Inserts the provided data in the specified index of the vector.
///
/// This function ensures that:
///     - `vec` is not NULL
///     - `vec->data` is not NULL
///     - `data` is not NULL
VEC_RESULT vec_insert_s(Vector *vec, size_t index, void *data) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (data == NULL) { return VECR_NULL_DATA; }
    if (index > vec->len) { return VECR_OUT_OF_BOUNDS; }

    vec_insert(vec, index, data);
    return VECR_OK;
}

/// Remove the last element of the vector.
void vec_pop(Vector *vec) {
    vec->len--;
}

/// Remove the last element of the vector.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
VEC_RESULT vec_pop_s(Vector *vec) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    vec_pop(vec);

    return VECR_OK;
}

/// Remove the element at the specified index of the vector and shift
/// the rest of the data to leave no gaps.
void vec_remove(Vector *vec, size_t index) {
    vec->len--;
    if (index == vec->len) { return; }

    memcpy(vec_get(vec, index), vec_get(vec, index+1), vec->size * (vec->len+1 - index+1));
}

/// Remove the element at the specified index of the vector and shift
/// the rest of the data to leave no gaps.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `index` is in bounds (`VECR_OUT_OF_BOUNDS`)
VEC_RESULT vec_remove_s(Vector *vec, size_t index) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (index >= vec->len) { return VECR_OUT_OF_BOUNDS; }

    vec->len--;
    if (index == vec->len) { return VECR_OK; }

    memcpy(vec_get(vec, index), vec_get(vec, index+1), vec->size * (vec->len+1 - index+1));
    return VECR_OK;
}


/// Remove the elements at the specified range of indexes of the vector and
/// shift the rest of the data to leave no gaps.
void vec_remove_range(Vector *vec, size_t beg, size_t end) {
    vec->len -= end - beg;
    if (end == vec->len) { return; }
    memcpy(vec_get(vec, beg), vec_get(vec, end), vec->size * vec->len);
}

/// Remove the elements at the specified range of indexes of the vector and
/// shift the rest of the data to leave no gaps.
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `beg` is smaller than `end` (`VECR_INVALID_BEG`)
///     - `beg` and `end` is in bounds (`VECR_OUT_OF_BOUNDS`)
VEC_RESULT vec_remove_range_s(Vector *vec, size_t beg, size_t end) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (end < beg) { return VECR_INVALID_BEG; }
    if (end > vec->len || beg >= vec->len) { return VECR_OUT_OF_BOUNDS; }

    vec_remove_range(vec, beg, end);
    return VECR_OK;
}

/// Remove the elements at multiple specified ranges of indexes of the vector and
/// shift the rest of the data to leave no gaps. This function assumes that all the
/// ranges are normalized which means that:
///     - beggining of a range is in bounds
///     - end of a range is in bounds
///     - beggining of a range is smaller than its' end
///     - ranges are in order
///     - ranges are not overlaping
///     - each range has at least one index of space left in between, otherwise they should be one range
void vec_remove_normalized_ranges(Vector *vec, size_t *ranges, size_t amount) {
    if (amount == 0) { return; }
    size_t initial_len = vec->len;
    size_t current_data_end = ranges[0];

    for (size_t i = 2; i < amount*2; i+=2) {
        size_t beg = ranges[i-2];
        size_t end = ranges[i-1];
        size_t next_beg = ranges[i];
        vec->len -= end - beg;
        //printf("MHM: %lld o: %lld", end-beg, current_data_end);
        memcpy(vec_get(vec, current_data_end), vec_get(vec, end), vec->size * (next_beg - end));
        current_data_end += next_beg - end;
    }

    size_t beg = ranges[amount*2-2];
    size_t end = ranges[amount*2-1];
    vec->len -= end - beg;
    //printf("```%lld %lld```", end, initial_len);
    if (end == initial_len) { return; }
    //printf("MHM: %lld from: %lld %lld %lld", end-beg, end, beg, current_data_end);
    memcpy(vec_get(vec, current_data_end), vec_get(vec, end), vec->size * (initial_len - end));
}

/// Remove the elements at multiple specified ranges of indexes of the vector and
/// shift the rest of the data to leave no gaps. This function assumes that all the
/// ranges are normalized which means that:
///     - beggining of a range is in bounds
///     - end of a range is in bounds
///     - beggining of a range is smaller than its' end
///     - ranges are in order
///     - ranges are not overlaping
///     - each range has at least one index of space left in between, otherwise they should be one range
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `ranges` is not NULL (`VECR_NULL_RANGE`)
///     - ranges are normalized (`VECR_INVALID_RANGE`)
VEC_RESULT vec_remove_normalized_ranges_s(Vector *vec, size_t *ranges, size_t amount) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (ranges == NULL) { return VECR_NULL_RANGE; }

    // Check if the ranges are normalized
    size_t highest_seen = ranges[0];
    for (size_t i = 1; i < amount*2; i++) {
        if (ranges[i] <= highest_seen) { return VECR_INVALID_RANGE; }
        highest_seen = ranges[i];
    }

    vec_remove_normalized_ranges(vec, ranges, amount);
    return VECR_OK;
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
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `comp` is not NULL (`VECR_NULL_COMP`)
///     - `searched` is not NULL (`VECR_NULL_SEARCHED`)
///     - `beg` is smaller than `end` (`VECR_INVALID_BEG`)
///     - `beg` and `end` is in bounds (`VECR_OUT_OF_BOUNDS`)
VEC_RESULT vec_binary_search(Vector *vec, VEC_RESULT (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (comp == NULL) { return VECR_NULL_COMP; }
    // TODO: This error check shouldn't be here
    if (searched == NULL) { return VECR_NULL_SEARCHED; }
    if (vec->len < end || vec->len < beg) { return VECR_OUT_OF_BOUNDS; }
    if (beg >= end) { return VECR_INVALID_BEG; }
    size_t new_beg = beg;
    size_t new_end = end;

    while (true) {
        size_t middle = (new_end + new_beg) / 2;
        char* current_char = vec->data + (vec->size * middle);

        if (middle == new_end || middle == new_beg) {
            VEC_RESULT result = comp(current_char, searched);
            if (result == VECR_FOUND) { *index = middle; return VECR_OK; }
            return VECR_NOT_FOUND;
        }

        switch (comp(current_char, searched)) {
            case VECR_LEFT: { new_end = middle; break; }
            case VECR_FOUND: { *index = middle; return VECR_OK; }
            case VECR_RIGHT: { new_beg = middle; break; }
        }
    }

    return VECR_NOT_FOUND;
}

//void _vec_insertion_sort(Vector *vec) {
//    for (size_t i = 0; i < vec->len; i++) {
//        for (size_t j = i; j >= 0; j--) {
//            if (*(int*)vec_get_unchecked(vec, j) <= *(int*)vec_get_unchecked(vec, i)) {
//
//            }
//        }
//    }
//}

//void vec_sort_unchecked(Vector *vec) {
//}

/// Comp function of binary search for `ints`
VEC_RESULT vbsc_int(void *current_num, void *searched_num) {
    if (*(int*)current_num > *(int*)searched_num) { return VECR_LEFT; }
    if (*(int*)current_num < *(int*)searched_num) { return VECR_RIGHT; }
    if (*(int*)current_num == *(int*)searched_num) { return VECR_FOUND; }
    return -1;
}

/// Comp function of binary search for end of line in an zeroed out (from the right side) buffer
VEC_RESULT vbsc_rf(void *current_char, void *_) {
    switch (*(char*)current_char) {
        case '\n': { return VECR_FOUND; }
        case 0: { return VECR_LEFT; }
        default: { return VECR_RIGHT; }
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
///
/// This function ensures that:
///     - `vec` is not NULL (`VECR_NULL_VEC`)
///     - `vec->data` is not NULL (`VECR_NULL_VEC_DATA`)
///     - `comp` is not NULL (`VECR_NULL_COMP`)
///     - `searched` is not NULL (`VECR_NULL_SEARCHED`)
///     - `beg` is smaller than `end` (`VECR_INVALID_BEG`)
///     - `beg` and `end` is in bounds (`VECR_OUT_OF_BOUNDS`)
VEC_RESULT vec_find_first(Vector *vec, bool (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched) {
    if (vec == NULL) { return VECR_NULL_VEC; }
    if (vec->data == NULL) { return VECR_NULL_VEC_DATA; }
    if (comp == NULL) { return VECR_NULL_COMP; }
    if (searched == NULL) { return VECR_NULL_SEARCHED; }
    if (vec->len < end || vec->len < beg) { return VECR_OUT_OF_BOUNDS; }
    if (beg >= end) { return VECR_INVALID_BEG; }

    for (size_t x = beg; x < end; x++) {
        if (comp(vec_get(vec, x), searched)) {
            if (index != NULL) { *index = x; }
            return VECR_OK;
        }
    }

    return VECR_NOT_FOUND;
}

/// Checks if `len`, `cap` and `size` of a vector are equal.
bool vec_is_partial_eq(Vector *vec1, Vector *vec2) {
    return (
        vec1->cap == vec2->cap
        && vec1->len == vec2->len
        && vec1->size == vec2->size
    );
}

/// Checks if `len`, `cap` and `size` of a vector are equal.
///
/// This function ensures that:
///     - `vec1` is not NULL
///     - `vec2` is not NULL
///
/// If they are, `false` will be returned.
bool vec_is_partial_eq_s(Vector *vec1, Vector *vec2) {
    if (vec1 == NULL || vec2 == NULL) { return false; }
    return vec_is_partial_eq(vec1, vec2);
}

/// Checks if `len`, `cap`, `size` and `data` pointer of a vector are equal.
bool vec_is_eq(Vector *vec1, Vector *vec2) {
    return (
        vec_is_partial_eq(vec1, vec2)
        && vec1->data == vec2->data
    );
}

/// Checks if `len`, `cap`, `size` and `data` pointer of a vector are equal.
///
/// This function ensures that:
///     - `vec1` is not NULL
///     - `vec2` is not NULL
///
/// If they are, `false` will be returned.
bool vec_is_eq_s(Vector *vec1, Vector *vec2) {
    if (vec1 == NULL || vec2 == NULL) { return false; }
    return vec_is_partial_eq(vec1, vec2);
}

/// Checks if the data inside of the vectors are equal.
bool vec_is_data_eq(Vector *vec1, Vector *vec2) {
    return (memcmp(vec1->data, vec2->data, vec1->size * vec1->len) == 0);
}

/// Checks if the data inside of the vectors are equal.
///
/// This function ensures that:
///     - `vec1` is not NULL
///     - `vec2` is not NULL
///
/// If they are, `false` will be returned.
bool vec_is_data_eq_s(Vector *vec1, Vector *vec2) {
    if (vec1 == NULL || vec2 == NULL) { return false; }
    return (memcmp(vec1->data, vec2->data, vec1->size * vec1->len) == 0);
}

/// Checks if `len`, `cap`, `size` and `data` the vectors are pointing to are equal.
/// The pointers might differ, only the similiarity of the data being held is checked.
bool vec_is_eq_deep(Vector *vec1, Vector *vec2) {
    if (!vec_is_partial_eq(vec1, vec2)) { return false; }
    return vec_is_data_eq(vec1, vec2);
}

/// Checks if `len`, `cap`, `size` and `data` the vectors are pointing to are equal.
/// The pointers might differ, only the similiarity of the data being held is checked.
///
/// This function ensures that:
///     - `vec1` is not NULL
///     - `vec1->data` is not NULL
///     - `vec2` is not NULL
///     - `vec2->data` is not NULL
///
/// If they are, `false` will be returned.
bool vec_is_eq_deep_s(Vector *vec1, Vector *vec2) {
    if (vec1 == NULL || vec2 == NULL) { return false; }
    if (vec1->data == NULL || vec2->data == NULL) { return false; }
    return vec_is_eq_deep(vec1, vec2);
}

/// Makes a copy of the provided vector copying all the data it holds.
Vector vec_copy(Vector *vec) {
    Vector copy;

    vec_init(&copy, vec->size, vec->data, vec->len);
    vec_reserve(&copy, vec->cap);

    return copy;
}

/// Makes a copy of the provided vector copying all the data it holds.
///
/// This function ensures that:
///     - `vec` is not NULL
///
/// If it is, `VEC_VARIADIC_END` will be returned.
Vector vec_copy_s(Vector *vec) {
    if (vec == NULL) { return VEC_VARIADIC_END; }
    return vec_copy(vec);
}

/// Makes a copy of the source vector into the destination vector copying all the data it holds.
void vec_copy_into(Vector *source_vec, Vector *destination_vec) {
    *destination_vec = vec_copy(source_vec);
}

/// Makes a copy of the source vector into the destination vector copying all the data it holds.
///
/// This function ensures that:
///     - `source_vec` is not NULL
///     - `destination_vec` is not NULL
VEC_RESULT vec_copy_into_s(Vector *source_vec, Vector *destination_vec) {
    if (source_vec == NULL) { return VECR_NULL_SOURCE; }
    if (destination_vec == NULL) { return VECR_NULL_DESTINATION; }

    vec_copy_into(source_vec, destination_vec);

    return VECR_OK;
}

/// Swaps the data of two specified indices in the vector.
void vec_swap(Vector *vec, size_t first_index, size_t second_index) {
    // Make a copy of the value at the first index
    void *first_element_copy = malloc(vec->size);
    memcpy(first_element_copy, vec_get_s(vec, first_index), vec->size);

    // Swap the elemnts
    memcpy(vec_get_s(vec, first_index), vec_get_s(vec, second_index), vec->size);
    memcpy(vec_get_s(vec, second_index), first_element_copy, vec->size);
    free(first_element_copy);
}

/// Swaps the data of two specified indices in the vector.
///
/// This function ensures that:
///     - `vec` is not NULL
///     - `vec->data` is not NULL
///     - `first_index` is in bounds
///     - `second_index` is in bounds
void vec_swap_s(Vector *vec, size_t first_index, size_t second_index) {
    if (
        vec == NULL
        || vec->data == NULL
        || first_index >= vec->len
        || second_index >= vec->len
    ) { return; }
    vec_swap(vec, first_index, second_index);
}

/// Reads a line from a specified file (using `fgetc`) pushing each character into the array until `'\n'`.
/// Doesn't push `'\n'` or any additional characters such as `'\0'`.
/// This function doesn't handle files that are already at the `EOF`. Use `vec_read_ascii_line_s` for that.
void vec_read_ascii_line(Vector *vec, FILE *file) {
    int character = EOF;

    do {
        character = fgetc(file);
        if (character == '\n') { return; }
        vec_push(vec, &character);
    } while (character != EOF);
}

// TODO: Errors.
/// Reads a line from a specified file (using `fgetc`) pushing each character into the array until `'\n'`.
/// Doesn't push `'\n'` or any additional characters such as `'\0'`.
///
/// This function ensures that:
///     - `vec` is not NULL
///     - `vec->data` is not NULL
///     - `file` is not NULL
///     - `file` doesn't have EOF indicator set
void vec_read_ascii_line_s(Vector *vec, FILE *file) {
    if (vec == NULL) { return; }
    if (vec->data == NULL) { return; }
    if (file == NULL) { return; }
    if (feof(file) != 0) { return; }

    vec_read_ascii_line(vec, file);
}

// TODO: Remake this function.
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
            vec_reserve(&line, ((line.cap - 2) * 2) + 2);
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
                case VECR_OK: { vec_reserve(&line, line.cap - 2); goto line_read; }
                case VECR_NOT_FOUND: { break; }
                case VECR_INVALID_BEG: { printf("ERROR: Invalid input."); break; }
                case VECR_OUT_OF_BOUNDS: { printf("ERROR: Out of bounds."); break; }
                default: { printf("ERROR: In binary search"); break; }
            }
        }

    line_read:
        line.len = index;
        vec_push_s(vec, &line);
    }

file_read:
    fclose(file);
    return err;
}

// TODO: extract this to a seperate project

/// Checks if the given ASCII character has a visual representation
bool is_char_printable(char c) {
    return !(
        // All ASCII control characters
        c < '\037'

        // Extended ASCII codes that are currently unused
        || c == '\177'
        || c == '\201'
        || c == '\215'
        || c == '\217'
        || c == '\220'
        || c == '\235'
        || c == '\240'
        || c == '\255'
    );
}

#define VEC_FPRINTF_FUNC(name, color, specifier, type) \
    static inline void vec_fprint_##name(FILE *file, void *data) { \
        fprintf(file, specifier, type data); \
    } \
    \
    static inline void vec_fprintc_##name(FILE *file, void *data) { \
        fprintf(file, color specifier VEC_CMD_ESC_RESET, type data); \
    }

#define HAS_COLORED_OUTPUT(assign, funcname) \
    if ((VEC_CONFIG.FPRINT_WITH_COLORS && file != stdout) || (VEC_CONFIG.PRINT_WITH_COLORS && file == stdout)) { \
        assign = vec_fprintc_##funcname; \
    } else { \
        assign = vec_fprint_##funcname; \
    }

VEC_FPRINTF_FUNC(u8, VEC_CMD_ESC_YELLOW, "%hhu", *(uint8_t*));
VEC_FPRINTF_FUNC(i8, VEC_CMD_ESC_YELLOW, "%hhi", *(int8_t*));
VEC_FPRINTF_FUNC(u16, VEC_CMD_ESC_YELLOW, "%hu", *(uint16_t*));
VEC_FPRINTF_FUNC(i16, VEC_CMD_ESC_YELLOW, "%hi", *(int16_t*));
VEC_FPRINTF_FUNC(u32, VEC_CMD_ESC_YELLOW, "%u", *(uint32_t*));
VEC_FPRINTF_FUNC(i32, VEC_CMD_ESC_YELLOW, "%i", *(int32_t*));
VEC_FPRINTF_FUNC(u64, VEC_CMD_ESC_YELLOW, "%llu", *(uint64_t*));
VEC_FPRINTF_FUNC(i64, VEC_CMD_ESC_YELLOW, "%lli", *(int64_t*));
VEC_FPRINTF_FUNC(f, VEC_CMD_ESC_YELLOW, "%f", *(float*));
VEC_FPRINTF_FUNC(lf, VEC_CMD_ESC_YELLOW, "%lf", *(double*));

static inline void vec_fprint_c(FILE *file, void *data) {
    fprintf(file, "\'");

    if (!is_char_printable(*(char*)data)) {
        fprintf(file, "\\%hhu", *(char*)data);
    } else {
        fprintf(file, "%c", *(char*)data);
    }

    fprintf(file, "\'");
}

static inline void vec_fprintc_c(FILE *file, void *data) {
    fprintf(file, VEC_CMD_ESC_GREEN "\'");

    if (!is_char_printable(*(char*)data)) {
        fprintf(file, VEC_CMD_ESC_RED "\\%hhu" VEC_CMD_ESC_GREEN, *(char*)data);
    } else {
        fprintf(file, "%c", *(char*)data);
    }

    fprintf(file, "\'" VEC_CMD_ESC_RESET);
}

static inline void vec_fprint_s(FILE *file, void *data) {
    fprintf(file, "\"");

    for (char *current_char = *(char**)data; *current_char != '\0'; current_char++) {
        if (!is_char_printable(*current_char)) {
            fprintf(file, "\\%hhu", *current_char);
        } else {
            fprintf(file, "%c", *current_char);
        }
    }

    fprintf(file, "\"");
}

static inline void vec_fprintc_s(FILE *file, void *data) {
    fprintf(file, VEC_CMD_ESC_GREEN "\"");

    for (char *current_char = *(char**)data; *current_char != '\0'; current_char++) {
        if (!is_char_printable(*current_char)) {
            fprintf(file, VEC_CMD_ESC_RED "\\%hhu" VEC_CMD_ESC_GREEN, *current_char);
        } else {
            fprintf(file, "%c", *current_char);
        }
    }

    fprintf(file, "\"" VEC_CMD_ESC_RESET);
}

static inline void vec_fprint_ss(FILE *file, void *data) {
    fprintf(file, "\"");

    for (char *current_char = (char*)data; *current_char != '\0'; current_char++) {
        if (!is_char_printable(*current_char)) {
            fprintf(file, "\\%hhu", *current_char);
        } else {
            fprintf(file, "%c", *current_char);
        }
    }

    fprintf(file, "\"");
}

static inline void vec_fprintc_ss(FILE *file, void *data) {
    fprintf(file, VEC_CMD_ESC_GREEN "\"");

    for (char *current_char = (char*)data; *current_char != '\0'; current_char++) {
        if (!is_char_printable(*current_char)) {
            fprintf(file, VEC_CMD_ESC_RED "\\%hhu" VEC_CMD_ESC_GREEN, *current_char);
        } else {
            fprintf(file, "%c", *current_char);
        }
    }

    fprintf(file, "\"" VEC_CMD_ESC_RESET);
}

static inline void vec_fprint_p(FILE *file, void *data) {
    if (*(void**)data == NULL) {
        fprintf(file, "NULL");
    } else {
        fprintf(file, "0x%016llX", *(uintptr_t*)data);
    }
}

static inline void vec_fprintc_p(FILE *file, void *data) {
    if (*(void**)data == NULL) {
        fprintf(file, VEC_CMD_ESC_BRIGHT_WHITE "NULL" VEC_CMD_ESC_RESET);
    } else {
        fprintf(file, VEC_CMD_ESC_BRIGHT_YELLOW "0x%016llX" VEC_CMD_ESC_RESET, *(uintptr_t*)data);
    }
}

static inline void vec_fprint_v(FILE *file, void *data) {
    fprintf(file, "{ len: %lld, cap: %lld, size: %lld, data: ", ((Vector*)data)->len, ((Vector*)data)->cap, ((Vector*)data)->size);

    if (((Vector*)data)->data != NULL) {
        vec_fprint_p(file, &((Vector*)data)->data);
    } else {
        fprintf(file, "NULL");
    }

    fprintf(file, " }");
}

static inline void vec_fprintc_v(FILE *file, void *data) {
    // Len
    fprintf(file, "{ " VEC_CMD_ESC_BRIGHT_BLUE "len" VEC_CMD_ESC_RESET ": ");
    vec_fprintc_u64(file, &((Vector*)data)->len);

    // Cap
    fprintf(file, ", " VEC_CMD_ESC_BRIGHT_BLUE "cap" VEC_CMD_ESC_RESET ": ");
    vec_fprintc_u64(file, &((Vector*)data)->cap);

    // Size
    fprintf(file, ", " VEC_CMD_ESC_BRIGHT_BLUE "size" VEC_CMD_ESC_RESET ": ");
    vec_fprintc_u64(file, &((Vector*)data)->size);
    
    // Data
    fprintf(file, ", " VEC_CMD_ESC_BRIGHT_BLUE "data" VEC_CMD_ESC_RESET ": ");
    vec_fprintc_p(file, &((Vector*)data)->data);

    fprintf(file, " }");
}

void vec_finfo(FILE *file, Vector *vec) {
    vec_fprint_v(file, vec);
}

void vec_info(Vector *vec) {
    vec_finfo(stdout, vec);
}

// TODO: Error handling
void vec_fprint(FILE *file, Vector *vec, VEC_PRINT_TYPE type) {
    void (*print_func)(FILE*, void*);

    switch (type) {
        case VPT_U8: { HAS_COLORED_OUTPUT(print_func, u8); break; }
        case VPT_I8: { HAS_COLORED_OUTPUT(print_func, i8); break; }
        case VPT_U16: { HAS_COLORED_OUTPUT(print_func, u16); break; }
        case VPT_I16: { HAS_COLORED_OUTPUT(print_func, i16); break; }
        case VPT_U32: { HAS_COLORED_OUTPUT(print_func, u32); break; }
        case VPT_I32: { HAS_COLORED_OUTPUT(print_func, i32); break; }
        case VPT_U64: { HAS_COLORED_OUTPUT(print_func, u64); break; }
        case VPT_I64: { HAS_COLORED_OUTPUT(print_func, i64); break; }
        case VPT_FLOAT: { HAS_COLORED_OUTPUT(print_func, f); break; }
        case VPT_DOUBLE: { HAS_COLORED_OUTPUT(print_func, lf); break; }
        case VPT_CHAR: { HAS_COLORED_OUTPUT(print_func, c); break; }
        case VPT_STRING: { HAS_COLORED_OUTPUT(print_func, s); break; }
        case VPT_STRING_STATIC: { HAS_COLORED_OUTPUT(print_func, ss); break; }
        case VPT_POINTER: { HAS_COLORED_OUTPUT(print_func, p); break; }
        case VPT_VECTOR: { HAS_COLORED_OUTPUT(print_func, v); break; }
    }

    fprintf(file, "[");
    for (int i = 0; i < vec->len - 1; i++) {
        print_func(file, vec_get_s(vec, i));
        fprintf(file, ", ");
    }
    print_func(file, vec_get_s(vec, vec->len-1));
    fprintf(file, "]");
}

void vec_print(Vector *vec, VEC_PRINT_TYPE type) {
    vec_fprint(stdout, vec, type);
}

// TODO: `vec_print_custom` for custom types
