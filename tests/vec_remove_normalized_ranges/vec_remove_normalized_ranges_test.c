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
    vec_remove_normalized_ranges_unchecked(&vec, ranges1, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), ((void*)DATA2)+vec.size, vec.len * vec.size) == 0),
        vec_drop_single(&vec)
    );

    // Test 2: remove last value
    start_test(2);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges2[] = {
        vec.len-1, vec.len,
    };
    vec_remove_normalized_ranges_unchecked(&vec, ranges2, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, vec.len * vec.size) == 0),
        vec_drop_single(&vec)
    );

    // Test 3: remove 4th value
    start_test(3);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges3[] = {
        3, 4,
    };
    vec_remove_normalized_ranges_unchecked(&vec, ranges3, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, vec.size*3) == 0)
        && (memcmp(vec_get_unchecked(&vec, 3), ((void*)DATA2)+(vec.size*4), vec.size*5) == 0),
        vec_drop_single(&vec)
    );

    // Test 4: remove first and last value
    start_test(4);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges4[] = {
        0, 1,
        vec.len-1, vec.len,
    };
    vec_remove_normalized_ranges_unchecked(&vec, ranges4, 2);

    end_test(
        vec.len == DATA2_LEN-2 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), ((void*)DATA2)+vec.size, vec.size*(vec.len-2)) == 0),
        vec_drop_single(&vec)
    );

    // Test 5: remove from 1st to 3rd and from 6th to 9th value
    start_test(5);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    size_t ranges5[] = {
        0, 4,
        5, vec.len,
    };
    p_vec_info(&vec); p_vec_print(&vec, PVPT_INT); printf("\n");
    vec_remove_normalized_ranges_unchecked(&vec, ranges5, 2);
    p_vec_info(&vec); p_vec_print(&vec, PVPT_INT); printf("\n");

    end_test(
        vec.len == DATA2_LEN-2 && vec.size == DATA2_SIZE && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), ((void*)DATA2)+(vec.size*3), vec.size*2) == 0),
        vec_drop_single(&vec)
    );

    return result;
}
