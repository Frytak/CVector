#include <string.h>
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

int vec_remove_normalized_ranges_test() {
    int result = 0;
    Vector vec;

    // Test 1: remove first value
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges1[] = {
        0, 1,
    };
    vec_remove_normalized_ranges(&vec, ranges1, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), ((void*)DATA2)+vec.size, vec.len * vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 2: remove last value
    start_test(2);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges2[] = {
        vec.len-1, vec.len,
    };
    vec_remove_normalized_ranges(&vec, ranges2, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.len * vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 3: remove 4th value
    start_test(3);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges3[] = {
        3, 4,
    };
    vec_remove_normalized_ranges(&vec, ranges3, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.size*3) == 0)
        && (memcmp(vec_get_s(&vec, 3), ((void*)DATA2)+(vec.size*4), vec.size*5) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 4: remove first and last value
    start_test(4);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges4[] = {
        0, 1,
        vec.len-1, vec.len,
    };
    vec_remove_normalized_ranges(&vec, ranges4, 2);

    end_test(
        vec.len == DATA2_LEN-2 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), ((void*)DATA2)+vec.size, vec.size*(vec.len-2)) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 5: remove from 1st to 3rd and from 6th to 9th value (all inclusive)
    start_test(5);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges5[] = {
        0, 3,
        5, vec.len,
    };
    vec_remove_normalized_ranges(&vec, ranges5, 2);

    end_test(
        vec.len == DATA2_LEN-7 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), ((void*)DATA2)+(vec.size*3), vec.size*2) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 6: remove the 2nd, from 4th to 6th and from 7th to 8th value (all inclusive)
    start_test(6);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges6[] = {
        1, 2,
        3, 6,
        6, 8,
    };
    vec_remove_normalized_ranges(&vec, ranges6, 3);

    end_test(
        vec.len == DATA2_LEN-6 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), (void*)DATA2, vec.size) == 0)
        && (memcmp(vec_get_s(&vec, 1), ((void*)DATA2)+(vec.size*2), vec.size) == 0)
        && (memcmp(vec_get_s(&vec, 2), ((void*)DATA2)+(vec.size*8), vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 7: error handling
    start_test(7);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    Vector uninitialized_vec = vec_new(0, NULL, 0);
    size_t ranges7error1[] = { 1, 2, 2, 6, 6, 8, };
    size_t ranges7error2[] = { 1, 2, 6, 9, 3, 4, };
    size_t ranges7ok[] = { 1, 2, 3, 4, 6, 9, };

    VEC_REMOVE_NORMALIZED_RANGES_RESULT error1 = vec_remove_normalized_ranges_s(NULL, ranges7ok, 3);
    VEC_REMOVE_NORMALIZED_RANGES_RESULT error2 = vec_remove_normalized_ranges_s(&vec, NULL, 3);
    VEC_REMOVE_NORMALIZED_RANGES_RESULT error3 = vec_remove_normalized_ranges_s(&vec, ranges7error1, 3);
    VEC_REMOVE_NORMALIZED_RANGES_RESULT error4 = vec_remove_normalized_ranges_s(&vec, ranges7error2, 3);
    VEC_REMOVE_NORMALIZED_RANGES_RESULT error5 = vec_remove_normalized_ranges_s(&uninitialized_vec, ranges7ok, 3);
    VEC_REMOVE_NORMALIZED_RANGES_RESULT ok = vec_remove_normalized_ranges_s(&vec, ranges7ok, 3);

    end_test(
        error1 == VRENRR_INVALID_VEC
        && error2 == VRENRR_INVALID_RANGES
        && error3 == VRENRR_NON_NORMALIZED_RANGES && error4 == VRENRR_NON_NORMALIZED_RANGES
        && error5 == VRENRR_INVALID_VEC_DATA
        && ok == VRENRR_OK
        && (memcmp(vec_get_s(&vec, 0), (void*)DATA2, vec.size) == 0)
        && (memcmp(vec_get_s(&vec, 1), ((void*)DATA2)+(vec.size*2), vec.size) == 0)
        && (memcmp(vec_get_s(&vec, 2), ((void*)DATA2)+(vec.size*4), vec.size*2) == 0),
        vec_drop_single_s(&vec)
    );

    return result;
}
