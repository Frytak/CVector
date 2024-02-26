#include <stdio.h>
#define _FRYTAK_VEC

#include <corecrt.h>
#include <stdbool.h>

// Vector binary search comparison function return values
typedef enum {
    // The searched value is to the left
    VBSCR_LEFT,

    // The searched value is to the right
    VBSCR_RIGHT,

    // Found the serched value
    VBSCR_FOUND,
} VEC_BINARY_SEARCH_COMP_RESULT;

// Vector binary search return values
typedef enum {
    // No errors, value was found
    VBSR_OK,

    // The value doesn't exist in the specified boundries
    VBSR_NOT_FOUND,

    // `beg` or `end` are larger than the vector length
    VBSR_OUT_OF_BOUNDS,

    // `beg` is bigger than `end`
    VBSR_INVALID_INPUT,

    // the `comp` function returns a value that is not -1, 0 or 1
    VBSR_COMP_INVALID_OUTPUT,
} VEC_BINARY_SEARCH_RESULT;

// TODO: REVIEW
typedef enum {
    // No errors, value was found
    CF_OK,

    // The value doesn't exist in the specified boundries
    CF_NOT_FOUND,

    // `beg` or `end` are larger than the vector length
    CF_OUT_OF_BOUNDS,

    // `beg` is bigger than `end`
    CF_INVALID_INPUT,

    // the `comp` function should return a value that is boolean (1, 0)
    CF_COMP_INVALID_OUTPUT,
} COMP_FUNC_RET;

// `vec_init` result
typedef enum {
    // No errors, `vec` initialized
    VIR_OK,

    // Pointer to `vec` is NULL
    VIR_INVALID_VEC,
} VEC_INIT_RESULT;

// `vec_push` result
typedef enum {
    // No errors, value inserted
    VPR_OK,

    // Pointer to `vec` is NULL
    VPR_INVALID_VEC,

    // Pointer to `vec->data` is NULL
    VPR_INVALID_VEC_DATA,

    // Pointer to `data` is NULL
    VPR_INVALID_DATA,
} VEC_PUSH_RESULT;

// `vec_reserve` result
typedef enum {
    // No errors, capacity reserved
    VRR_OK,

    // Pointer to `vec` is NULL
    VRR_INVALID_VEC,
} VEC_RESERVE_RESULT;

typedef enum {
    // No errors, copy successful
    VCR_OK,

    // Source vector is invalid
    VCR_INVALID_SOURCE,

    // Destination vector is invalid
    VCR_INVALID_DESTINATION,

    // Unknown error leading to an unsuccessful copy.
    // This error might be caused by data races, cosmic rays or other such stuff.
    // Good luck!
    //
    // WARNING! This error only checks if `len`, `cap` and `size` are copied properly, not the contents.
    VCR_UNKNOWN,
} VEC_COPY_RESULT;

typedef enum {
    // No errors, vectors compared successfully
    VER_OK,

    // First vector is invalid
    VER_INVALID_SOURCE,

    // Second vector is invalid
    VER_INVALID_DESTINATION,
} VEC_EQ_RESULT;

typedef enum {
    // No errors, vector dropped successfully
    VDR_OK,

    // Invalid pointer to vector
    VDR_INVALID_VEC,

    // Invalid pointer to data of a vector, can't be dropped
    VDR_INVALID_VEC_DATA,
} VEC_DROP_RESULT;

typedef enum {
    PVPT_INT,
    PVPT_STRING,
} P_VEC_PRINT_TYPE;


/// A Vector consisting of:
///   - len: amount of elements that the vector currently holds
///   - cap: amount of elements that the vector can currently hold in total (before growing in size)
///   - size: size in bytes of one element contained in the Vector
///   - data: pointer to the data on the heap
///
/// Default `len` of `Vector` is 1 and it grows each time by a factor of 2.
typedef struct Vector {
    size_t len;
    size_t cap;
    size_t size;
    void *data;
} Vector;
extern const Vector VEC_VARIADIC_END;

size_t _vec_get_p2_cap(size_t amount);

void vec_drop_single_unchecked(Vector *vec);
void _vec_drop_unchecked(Vector *vec, ...);
#define vec_drop_unchecked(...) _vec_drop_unchecked(__VA_ARGS__, VEC_VARIADIC_END)
VEC_DROP_RESULT vec_drop_single(Vector *vec);
VEC_DROP_RESULT _vec_drop(size_t *err_index, Vector *vec, ...);
#define vec_drop(err_index, ...) _vec_drop(err_index, __VA_ARGS__, VEC_VARIADIC_END)

void vec_reserve_unchecked(Vector *vec, size_t cap);
VEC_RESERVE_RESULT vec_reserve(Vector *vec, size_t cap);

void _vec_double(Vector *vec);

VEC_INIT_RESULT vec_init(Vector *vec, size_t size, void *data, size_t amount);
Vector vec_new(size_t size, void *data, size_t amount);

void *vec_get_unchecked(Vector *vec, size_t index);
void *vec_get(Vector *vec, size_t index);

void vec_push_unchecked(Vector *vec, void *data);
VEC_PUSH_RESULT vec_push(Vector *vec, void *data);
void vec_push_multi_unchecked(Vector *vec, void *data, size_t amount);
VEC_PUSH_RESULT vec_push_multi(Vector *vec, void *data, size_t amount);
// TODO: Make a macro for pushing multiple elements as a variadic function

VEC_BINARY_SEARCH_RESULT vec_binary_search(Vector *vec, VEC_BINARY_SEARCH_COMP_RESULT (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched);
VEC_BINARY_SEARCH_COMP_RESULT vbsc_int(void *current_num, void *searched_num);

COMP_FUNC_RET vec_find_first(Vector *vec, bool (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched);
bool vc_int(void *current_num, void *searched_num);

bool vec_is_partial_eq_unchecked(Vector *vec1, Vector *vec2);
bool vec_is_partial_eq(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result);
bool vec_is_eq_unchecked(Vector *vec1, Vector *vec2);
bool vec_is_eq(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result);
bool vec_is_eq_deep_unchecked(Vector *vec1, Vector *vec2);
bool vec_is_eq_deep(Vector *vec1, Vector *vec2, VEC_EQ_RESULT *result);

Vector vec_copy_unchecked(Vector *vec);
VEC_COPY_RESULT vec_copy(Vector *source_vec, Vector *destination_vec);

void vec_swap_unchecked(Vector *vec, size_t first_index, size_t second_index);

void vec_swap(Vector *vec, size_t first_index, size_t second_index);

errno_t vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize);

#define CMD_ESC_YELLOW "\033[33m"
#define CMD_ESC_RESET "\033[0m"

void p_vec_info(Vector *vec);
void p_vec_fprint(FILE *file, Vector *vec, P_VEC_PRINT_TYPE type);
void p_vec_print(Vector *vec, P_VEC_PRINT_TYPE type);
