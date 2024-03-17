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

int vec_remove_test() {
    int result = 0;
    Vector vec;

    // Test 1: remove the first element
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove(&vec, 0);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), ((void*)DATA2+vec.size), vec.len * vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 2: remove the last element
    start_test(2);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove(&vec, vec.len-1);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.len * vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 3: remove the 4th element
    start_test(3);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_remove(&vec, 3);

    end_test(
        vec.len == DATA2_LEN-1 && vec.cap == DATA2_CAP && vec.size == DATA2_SIZE && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.size*3) == 0)
        && (memcmp(vec_get_s(&vec, 3), ((void*)DATA2+(vec.size*4)), (vec.len-3) * vec.size) == 0),
        vec_drop_single_s(&vec)
    );

    // Test 4: error handling
    start_test(4);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    VEC_REMOVE_RESULT error1 = vec_remove_s(NULL, 0);
    VEC_REMOVE_RESULT error2 = vec_remove_s(&vec, vec.len+4);
    VEC_REMOVE_RESULT ok = vec_remove_s(&vec, 2);
    drop_test_vecs(vec_drop_single_s(&vec));
    VEC_REMOVE_RESULT error3 = vec_remove_s(&vec, 0);

    end_test(
        error1 == VRER_INVALID_VEC
        && error2 == VRER_OUT_OF_BOUNDS
        && error3 == VRER_INVALID_VEC_DATA
        && ok == VRER_OK,
        NULL
    );

    return result;
}
