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

int vec_drop_test() {
    int result = 0;
    Vector vec1;
    Vector vec2;
    Vector vec3;

    // Test 1: `vec_drop_single_unchecked` - Drop a valid vector
    start_test(1);
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    vec_drop_single(&vec1);
    end_test(
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL,
        NULL
    );

    // Test 2: `vec_drop_unchecked` - Drop three valid vectors
    start_test(2);
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    vec_drop(&vec1, &vec2, &vec3);
    end_test(
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && vec2.len == 0 && vec2.cap == 0 && vec2.size == 0 && vec2.data == NULL
        && vec3.len == 0 && vec3.cap == 0 && vec3.size == 0 && vec3.data == NULL,
        NULL
    );

    // Test 3: `vec_drop_single` - Error handling
    start_test(3);
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    VEC_DROP_RESULT error1 = vec_drop_single_s(NULL);
    VEC_DROP_RESULT ok1 = vec_drop_single_s(&vec1);
    VEC_DROP_RESULT error2 = vec_drop_single_s(&vec1);
    end_test(
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && error1 == VDR_INVALID_VEC && error2 == VDR_INVALID_VEC_DATA && ok1 == VDR_OK,
        NULL
    );

    // Test 4: `vec_drop` - Error handling
    start_test(4);
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    VEC_DROP_RESULT error3 = vec_drop_s(&vec1, NULL, &vec3);
    VEC_DROP_RESULT error4 = vec_drop_s(&vec1, &vec2, NULL);
    VEC_DROP_RESULT error5 = vec_drop_s(&vec1, &vec2, &vec3);

    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    VEC_DROP_RESULT ok2 = vec_drop_s(&vec1, &vec2, &vec3);

    end_test(
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && vec2.len == 0 && vec2.cap == 0 && vec2.size == 0 && vec2.data == NULL
        && vec3.len == 0 && vec3.cap == 0 && vec3.size == 0 && vec3.data == NULL
        && error3 == VDR_INVALID_VEC
        && error4 == VDR_INVALID_VEC_DATA
        && error5 == VDR_INVALID_VEC_DATA
        && ok2 == VDR_OK,
        NULL
    );

    return result;
}
