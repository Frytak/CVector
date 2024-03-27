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

int vec_insert_test() {
    int result = 0;
    size_t index = 0;
    int data = 69;
    Vector vec;

    // Test 1: insert at the beggining
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec_insert(&vec, index, &data);

    end_test(
        vec.size == DATA2_SIZE && vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, index+1), DATA2+index, vec.size*(DATA2_LEN-index)) == 0)
        && *(int*)vec_get_s(&vec, index) == data,
        vec_drop_single_s(&vec)
    );

    // Test 2: insert at the end
    start_test(2);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    index = vec.len;
    vec_insert(&vec, index, &data);

    end_test(
        vec.size == DATA2_SIZE && vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.size*DATA2_LEN) == 0)
        && *(int*)vec_get_s(&vec, index) == data,
        vec_drop_single_s(&vec)
    );

    // Test 3: insert at index 3
    start_test(3);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    index = 3;
    vec_insert(&vec, index, &data);

    end_test(
        vec.size == DATA2_SIZE && vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.size*index) == 0)
        && (memcmp(vec_get_s(&vec, index+1), DATA2+index, vec.size*(DATA2_LEN-index)) == 0)
        && *(int*)vec_get_s(&vec, index) == data,
        vec_drop_single_s(&vec)
    );

    // Test 4: error handling
    start_test(4);
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    Vector uninitialized_vec = vec_new(0, NULL, 0);
    index = 7;

    VEC_RESULT error1 = vec_insert_s(NULL, index, &data);
    VEC_RESULT error2 = vec_insert_s(&uninitialized_vec, index, &data);
    VEC_RESULT error3 = vec_insert_s(&vec, index, NULL);
    VEC_RESULT error4 = vec_insert_s(&vec, vec.len+12, &data);
    VEC_RESULT ok = vec_insert_s(&vec, index, &data);

    end_test(
        vec.size == DATA2_SIZE && vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && vec.data != NULL
        && (memcmp(vec_get_s(&vec, 0), DATA2, vec.size*index) == 0)
        && (memcmp(vec_get_s(&vec, index+1), DATA2+index, vec.size*(DATA2_LEN-index)) == 0)
        && *(int*)vec_get_s(&vec, index) == data
        && error1 == VECR_NULL_VEC
        && error2 == VECR_NULL_VEC_DATA
        && error3 == VECR_NULL_DATA
        && error4 == VECR_OUT_OF_BOUNDS
        && ok == VECR_OK,
        vec_drop_single_s(&vec)
    );

    return result;
}
