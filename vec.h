#include <stdint.h>
#include <stdio.h>
#define _FRYTAK_VEC

#include <corecrt.h>
#include <stdbool.h>

typedef struct {
    bool PRINT_WITH_COLORS;
    bool FPRINT_WITH_COLORS;
    uint8_t TIMSORT_RUN_SIZE;
} VecConfig;

extern VecConfig VEC_CONFIG;

bool vec_cg_print_colors();
void vec_cs_print_colors(bool set);

bool vec_cg_fprint_colors();
void vec_cs_fprint_colors(bool set);

uint8_t vec_cg_timsort_run_size();
void vec_cs_timsort_run_size(uint8_t set);

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
    VSR_OK,

    // The value doesn't exist in the specified boundries
    VSR_NOT_FOUND,

    // Pointer to `vec` is NULL
    VSR_INVALID_VEC,

    // Pointer to `comp` is NULL
    VSR_INVALID_COMP,

    // Pointer to `searched` is NULL
    VSR_INVALID_SEARCHED,

    // `beg` or/and `end` are larger than the vectors' length
    VSR_OUT_OF_BOUNDS,

    // `beg` is bigger than `end`
    VSR_INVALID_BOUNDS,
} VEC_SEARCH_RESULT;

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

typedef enum {
    // No errors, value removed and data shifted
    VRER_OK,

    // Pointer to `vec` is NULL
    VRER_INVALID_VEC,

    // Pointer to `vec->data` is NULL
    VRER_INVALID_VEC_DATA,

    // Provided index is bigger than the vectors' length
    VRER_OUT_OF_BOUNDS,
} VEC_REMOVE_RESULT;

typedef enum {
    // No errors, value removed and data shifted
    VRERR_OK,

    // Pointer to `vec` is NULL
    VRERR_INVALID_VEC,

    // Pointer to `vec->data` is NULL
    VRERR_INVALID_VEC_DATA,

    // `beg` is bigger than `end`
    VRERR_INVALID_BOUNDS,

    // Provided `beg` or/and `end` are bigger than the vectors' length
    VRERR_OUT_OF_BOUNDS,
} VEC_REMOVE_RANGE_RESULT;

typedef enum {
    // No errors, values removed and data shifted
    VRENRR_OK,

    // Pointer to `vec` is NULL
    VRENRR_INVALID_VEC,

    // Pointer to `vec->data` is NULL
    VRENRR_INVALID_VEC_DATA,

    // Pointer to `ranges` is NULL
    VRENRR_INVALID_RANGES,

    // Given ranges are not normalized
    VRENRR_NON_NORMALIZED_RANGES,
} VEC_REMOVE_NORMALIZED_RANGES_RESULT;

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
    // No errors, data inserted successfully
    VISR_OK,

    // Invalid pointer to vector
    VISR_INVALID_VEC,

    // Invalid pointer to data of a vector
    VISR_INVALID_VEC_DATA,

    // Invalid pointer to data
    VISR_INVALID_DATA,

    // Index is larger than vectors length.
    // It can be between <0,vec.len>.
    VISR_OUT_OF_BOUNDS,
} VEC_INSERT_RESULT;

typedef enum {
    VPT_U8, // Format specifier: `%hhu`.
    VPT_I8, // Format specifier: `%hhi`.
    VPT_U16, // Format specifier: `%hu`.
    VPT_I16, // Format specifier: `%hi`.
    VPT_U32, // Format specifier: `%u`.
    VPT_I32, // Format specifier: `%i`.
    VPT_U64, // Format specifier: `%llu`.
    VPT_I64, // Format specifier: `%lli`.
    VPT_FLOAT, // Format specifier: `%f`.
    VPT_DOUBLE, // Format specifier: `%lf`.
    VPT_CHAR, // Format specifier: `'%c'` (all non printable ASCII characters are turned into their decimal representation with a `\` at the beggining).
    VPT_STRING, // Format specifier: `"%s"` (all non printable ASCII characters are turned into their decimal representation with a `\` at the beggining).
    // Format specifier: `"%s"` (all non printable ASCII characters are turned into their decimal representation with a `\` at the beggining).
    // A string directly kept in the vector.
    VPT_STRING_STATIC,
    VPT_POINTER, // Format specifier: `%p`.
    VPT_VECTOR, // Prints information about the contained vectors.
} VEC_PRINT_TYPE;

/// A Vector consisting of:
///   - len: amount of elements that the vector currently holds
///   - cap: amount of elements that the vector can currently hold in total (before growing in size)
///   - size: size in bytes of one element contained in the Vector
///   - data: pointer to the data on the heap
///
/// This implementation grows the capacity if it's not enough by a factor of 2.
typedef struct Vector {
    size_t len;
    size_t cap;
    size_t size;
    void *data;
} Vector;
extern const Vector VEC_VARIADIC_END;

void _vec_insertion_sort(Vector *vec);

size_t _vec_get_p2_cap(size_t amount);

void vec_drop_single(Vector *vec);
void _vec_drop(Vector *vec, ...);
#define vec_drop(...) _vec_drop(__VA_ARGS__, VEC_VARIADIC_END)
VEC_DROP_RESULT vec_drop_single_s(Vector *vec);
VEC_DROP_RESULT _vec_drop_s(Vector *vec, ...);
#define vec_drop_s(...) _vec_drop_s(__VA_ARGS__, VEC_VARIADIC_END)

void vec_reserve(Vector *vec, size_t cap);
VEC_RESERVE_RESULT vec_reserve_s(Vector *vec, size_t cap);

VEC_INIT_RESULT vec_init(Vector *vec, size_t size, void *data, size_t amount);
Vector vec_new(size_t size, void *data, size_t amount);

void *vec_get(Vector *vec, size_t index);
void *vec_get_s(Vector *vec, size_t index);

void vec_force_push(Vector *vec, void *data);
void vec_push(Vector *vec, void *data);
VEC_PUSH_RESULT vec_push_s(Vector *vec, void *data);
void vec_push_multi(Vector *vec, void *data, size_t amount);
VEC_PUSH_RESULT vec_push_multi_s(Vector *vec, void *data, size_t amount);

void vec_insert(Vector *vec, size_t index, void *data);
VEC_INSERT_RESULT vec_insert_s(Vector *vec, size_t index, void *data);

void vec_remove(Vector *vec, size_t index);
VEC_REMOVE_RESULT vec_remove_s(Vector *vec, size_t index);
void vec_remove_range(Vector *vec, size_t beg, size_t end);
VEC_REMOVE_RANGE_RESULT vec_remove_range_s(Vector *vec, size_t beg, size_t end);
void vec_remove_normalized_ranges(Vector *vec, size_t *ranges, size_t amount);
VEC_REMOVE_NORMALIZED_RANGES_RESULT vec_remove_normalized_ranges_s(Vector *vec, size_t *ranges, size_t amount);

VEC_SEARCH_RESULT vec_binary_search(Vector *vec, VEC_BINARY_SEARCH_COMP_RESULT (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched);
VEC_BINARY_SEARCH_COMP_RESULT vbsc_int(void *current_num, void *searched_num);
VEC_BINARY_SEARCH_COMP_RESULT vbsc_rf(void *current_char, void *_);

VEC_SEARCH_RESULT vec_find_first(Vector *vec, bool (*comp)(void *vec_item, void *searched), size_t beg, size_t end, size_t *index, void *searched);
bool vc_int(void *current_num, void *searched_num);
bool vc_char(void *current_num, void *searched_num);

bool vec_is_partial_eq(Vector *vec1, Vector *vec2);
bool vec_is_partial_eq_s(Vector *vec1, Vector *vec2);
bool vec_is_eq(Vector *vec1, Vector *vec2);
bool vec_is_eq_s(Vector *vec1, Vector *vec2);
bool vec_is_data_eq(Vector *vec1, Vector *vec2);
bool vec_is_data_eq_s(Vector *vec1, Vector *vec2);
bool vec_is_eq_deep(Vector *vec1, Vector *vec2);
bool vec_is_eq_deep_s(Vector *vec1, Vector *vec2);

Vector vec_copy(Vector *vec);
Vector vec_copy_s(Vector *vec);
void vec_copy_into(Vector *source_vec, Vector *destination_vec);
VEC_COPY_RESULT vec_copy_into_s(Vector *source_vec, Vector *destination_vec);

void vec_swap(Vector *vec, size_t first_index, size_t second_index);
void vec_swap_s(Vector *vec, size_t first_index, size_t second_index);

void vec_read_ascii_line(Vector *vec, FILE *file);
void vec_read_ascii_line_s(Vector *vec, FILE *file);
errno_t vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize);

#define VEC_CMD_ESC_BLACK "\033[30m"
#define VEC_CMD_ESC_BRIGHT_BLACK "\033[90m"

#define VEC_CMD_ESC_RED "\033[31m"
#define VEC_CMD_ESC_BRIGHT_RED "\033[91m"

#define VEC_CMD_ESC_GREEN "\033[32m"
#define VEC_CMD_ESC_BRIGHT_GREEN "\033[92m"

#define VEC_CMD_ESC_YELLOW "\033[33m"
#define VEC_CMD_ESC_BRIGHT_YELLOW "\033[93m"

#define VEC_CMD_ESC_BLUE "\033[34m"
#define VEC_CMD_ESC_BRIGHT_BLUE "\033[94m"

#define VEC_CMD_ESC_MAGENTA "\033[35m"
#define VEC_CMD_ESC_BRIGHT_MAGENTA "\033[95m"

#define VEC_CMD_ESC_CYAN "\033[36m"
#define VEC_CMD_ESC_BRIGHT_CYAN "\033[96m"

#define VEC_CMD_ESC_WHITE "\033[37m"
#define VEC_CMD_ESC_BRIGHT_WHITE "\033[97m"

#define VEC_CMD_ESC_RESET "\033[0m"

void vec_info(Vector *vec);
void vec_fprint(FILE *file, Vector *vec, VEC_PRINT_TYPE type);
void vec_print(Vector *vec, VEC_PRINT_TYPE type);
