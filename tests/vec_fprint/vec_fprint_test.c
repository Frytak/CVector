#ifndef _FRYTAK_VEC
#include "../../vec.h"
#endif

#ifndef _FRYTAK_VEC_TEST_EXAMPLE_DATA
#include "../example_data/example_data.h"
#endif

#ifndef _FRYTAK_VEC_TEST
#include "../test.h"
#endif

#include <stdio.h>

#define VEC_FPRINT_TEST(testnum, data) \
    start_test(testnum); \
    vec = vec_new(DATA_##data##_SIZE, (void*)DATA_##data, DATA_##data##_LEN); \
    fgetpos(output_file, &line_start); \
    \
    vec_fprint(output_file, &vec, VPT_ ##data); \
    fprintf(output_file, "\n"); \
    \
    fgetpos(output_file, &new_line_start); \
    fsetpos(output_file, &line_start); \
    \
    expected = vec_new(sizeof(char), NULL, 8); \
    output = vec_new(sizeof(char), NULL, 8); \
    vec_read_ascii_line(&expected, expected_file); \
    vec_read_ascii_line(&output, output_file); \
    \
    fsetpos(output_file, &new_line_start); \
    end_test( \
        vec_is_eq_deep(&output, &expected, NULL), \
        vec_drop(NULL, &vec, &expected, &output) \
    );

int vec_fprint_test() {
    int result = 0;
    FILE *output_file = fopen("./vec_fprint/output.txt", "w+");
    FILE *expected_file = fopen("./vec_fprint/expected.txt", "r");
    fpos_t line_start;
    fpos_t new_line_start;
    Vector output;
    Vector expected;
    Vector vec;

    VEC_FPRINT_TEST(1, U8);
    VEC_FPRINT_TEST(2, I8);
    VEC_FPRINT_TEST(3, U16);
    VEC_FPRINT_TEST(4, I16);
    VEC_FPRINT_TEST(5, U32);
    VEC_FPRINT_TEST(6, I32);
    VEC_FPRINT_TEST(7, U64);
    VEC_FPRINT_TEST(8, I64);
    VEC_FPRINT_TEST(9, FLOAT);
    VEC_FPRINT_TEST(10, DOUBLE);
    VEC_FPRINT_TEST(11, CHAR);
    VEC_FPRINT_TEST(12, STRING);
    VEC_FPRINT_TEST(13, STRING_STATIC);
    VEC_FPRINT_TEST(14, POINTER);

    // Vectors
    start_test(15);
    vec = vec_new(DATA_VECTOR_DUMMY_SIZE, (void*)DATA_VECTOR_DUMMY, DATA_VECTOR_DUMMY_LEN);
    fgetpos(output_file, &line_start);

    vec_fprint(output_file, &vec, VPT_VECTOR);
    fprintf(output_file, "\n");

    fgetpos(output_file, &new_line_start);
    fsetpos(output_file, &line_start);

    expected = vec_new(sizeof(char), NULL, 8);
    output = vec_new(sizeof(char), NULL, 8);
    vec_read_ascii_line(&expected, expected_file);
    vec_read_ascii_line(&output, output_file);

    fsetpos(output_file, &new_line_start);
    end_test(
        vec_is_eq_deep(&output, &expected, NULL),
        vec_drop(NULL, &vec, &expected, &output)
    );


    // Repeat the tests with colored output
    VEC_CONFIG.FPRINT_WITH_COLORS = true;
    VEC_FPRINT_TEST(16, U8);
    VEC_FPRINT_TEST(17, I8);
    VEC_FPRINT_TEST(18, U16);
    VEC_FPRINT_TEST(19, I16);
    VEC_FPRINT_TEST(20, U32);
    VEC_FPRINT_TEST(21, I32);
    VEC_FPRINT_TEST(22, U64);
    VEC_FPRINT_TEST(23, I64);
    VEC_FPRINT_TEST(24, FLOAT);
    VEC_FPRINT_TEST(25, DOUBLE);
    VEC_FPRINT_TEST(26, CHAR);
    VEC_FPRINT_TEST(27, STRING);
    VEC_FPRINT_TEST(28, STRING_STATIC);
    VEC_FPRINT_TEST(29, POINTER);

    // Vectors
    start_test(30);
    vec = vec_new(DATA_VECTOR_DUMMY_SIZE, (void*)DATA_VECTOR_DUMMY, DATA_VECTOR_DUMMY_LEN);
    fgetpos(output_file, &line_start);

    vec_fprint(output_file, &vec, VPT_VECTOR);
    fprintf(output_file, "\n");

    fgetpos(output_file, &new_line_start);
    fsetpos(output_file, &line_start);

    expected = vec_new(sizeof(char), NULL, 8);
    output = vec_new(sizeof(char), NULL, 8);
    vec_read_ascii_line(&expected, expected_file);
    vec_read_ascii_line(&output, output_file);

    fsetpos(output_file, &new_line_start);
    end_test(
        vec_is_eq_deep(&output, &expected, NULL),
        vec_drop(NULL, &vec, &expected, &output)
    );

    fclose(output_file);
    fclose(expected_file);
    return result;
}
