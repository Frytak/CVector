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

int vec_remove_range_test() {
    int result = 0;
    Vector vec;

    // Test 1: remove elements <1,4>
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, 0, 4);

    end_test(
        vec.len == DATA2_LEN-4 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), ((void*)DATA2+(vec.size*4)), (vec.len-3) * vec.size) == 0),
        vec_drop_single(&vec)
    );

    // Test 2: remove elements <6,9>
    start_test(2);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, 5, vec.len);

    end_test(
        vec.len == DATA2_LEN-4 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, (vec.len-4) * vec.size) == 0),
        vec_drop_single(&vec)
    );

    // Test 3: remove elements <3,7>
    start_test(3);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, 2, 7);

    end_test(
        vec.len == DATA2_LEN-5 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, vec.size*2) == 0)
        && (memcmp(vec_get_unchecked(&vec, 2), ((void*)DATA2+(vec.size*7)), vec.size*2) == 0),
        vec_drop_single(&vec)
    );

    // Test 4: remove elements <1,1>
    start_test(4);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, 0, 1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), ((void*)DATA2+(vec.size)), vec.size * vec.len) == 0),
        vec_drop_single(&vec)
    );

    // Test 5: remove elements <9,9>
    start_test(5);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, vec.len-1, vec.len);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, vec.size * vec.len-1) == 0),
        vec_drop_single(&vec)
    );

    // Test 6: remove elements <3,3>
    start_test(6);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove_range_unchecked(&vec, 2, 3);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_unchecked(&vec, 0), DATA2, vec.size*2) == 0)
        && (memcmp(vec_get_unchecked(&vec, 2), ((void*)DATA2+(vec.size*3)), vec.size*6) == 0),
        vec_drop_single(&vec)
    );

    // Test 7: error handling
    start_test(7);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);

    VEC_REMOVE_RANGE_RESULT error1 = vec_remove_range(NULL, 0, 3);
    VEC_REMOVE_RANGE_RESULT error2 = vec_remove_range(&vec, 6, 3);
    VEC_REMOVE_RANGE_RESULT error3 = vec_remove_range(&vec, vec.len+9, vec.len+18);
    VEC_REMOVE_RANGE_RESULT ok = vec_remove_range(&vec, 0, 3);
    drop_test_vecs(vec_drop_single(&vec));
    VEC_REMOVE_RANGE_RESULT error4 = vec_remove_range(&vec, 0, 3);

    end_test(
        error1 == VRERR_INVALID_VEC
        && error2 == VRERR_INVALID_BOUNDS
        && error3 == VRERR_OUT_OF_BOUNDS
        && error4 == VRERR_INVALID_VEC_DATA
        && ok == VRERR_OK,
        NULL
    );

    return result;
}
