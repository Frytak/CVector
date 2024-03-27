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

int vec_push_test() {
    int result = 0;
    Vector vec;
    
    // Test 1: `unchecked` - enough capacity for additional element
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    vec_push(&vec, (void*)&INSERT_DATA1[0]);

    end_test(
        vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && *(int*)vec_get_s(&vec, DATA2_LEN) == INSERT_DATA1[0],
        vec_drop_single_s(&vec)
    );

    // Test 2: `unchecked` - not enough capacity for additional elements
    start_test(2);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_push(&vec, (void*)&INSERT_DATA1[0]);
    vec_push(&vec, (void*)&INSERT_DATA1[1]);

    end_test(
        vec.len == DATA1_LEN+2 && vec.cap == 16 && *(int*)vec_get_s(&vec, DATA1_LEN) == INSERT_DATA1[0] && *(int*)vec_get_s(&vec, DATA1_LEN+1) == INSERT_DATA1[1],
        vec_drop_single_s(&vec)
    );

    // Test 3: error handling
    start_test(3);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    VEC_RESULT error1 = vec_push_s(NULL, (void*)&INSERT_DATA1[0]);
    VEC_RESULT error2 = vec_push_s(&vec, NULL);
    VEC_RESULT ok = vec_push_s(&vec, (void*)&INSERT_DATA1[0]);
    drop_test_vecs(vec_drop_single_s(&vec));
    VEC_RESULT error3 = vec_push_s(&vec, (void*)&INSERT_DATA1[0]);

    end_test(
        error1 == VECR_NULL_VEC && error2 == VECR_NULL_DATA && ok == VECR_OK && error3 == VECR_NULL_VEC_DATA,
        NULL
    );

    return result;
}
