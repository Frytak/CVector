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
    printf("\tTest 1: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    vec_drop_single_unchecked(&vec1);
    if (vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: `vec_drop_unchecked` - Drop three valid vectors
    printf("\tTest 2: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    vec_drop_unchecked(&vec1, &vec2, &vec3);
    if (
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && vec2.len == 0 && vec2.cap == 0 && vec2.size == 0 && vec2.data == NULL
        && vec3.len == 0 && vec3.cap == 0 && vec3.size == 0 && vec3.data == NULL
    )
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: `vec_drop_single` - Error handling
    printf("\tTest 3: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    VEC_DROP_RESULT error1 = vec_drop_single(NULL);
    VEC_DROP_RESULT ok1 = vec_drop_single(&vec1);
    VEC_DROP_RESULT error2 = vec_drop_single(&vec1);
    if (
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && error1 == VDR_INVALID_VEC && error2 == VDR_INVALID_VEC_DATA && ok1 == VDR_OK
    )
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 4: `vec_drop` - Error handling
    printf("\tTest 4: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    size_t error3_index = 10;
    VEC_DROP_RESULT error3 = vec_drop(&error3_index, NULL, &vec2, &vec3);

    size_t error4_index = 10;
    VEC_DROP_RESULT error4 = vec_drop(&error4_index, &vec1, &vec2, NULL);

    size_t error5_index = 10;
    VEC_DROP_RESULT error5 = vec_drop(&error5_index, &vec1, &vec2, &vec3);
    vec_drop_single_unchecked(&vec3);

    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    vec3 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    size_t ok2_index = 10;
    VEC_DROP_RESULT ok2 = vec_drop(&ok2_index, &vec1, &vec2, &vec3);

    if (
        vec1.len == 0 && vec1.cap == 0 && vec1.size == 0 && vec1.data == NULL
        && vec2.len == 0 && vec2.cap == 0 && vec2.size == 0 && vec2.data == NULL
        && vec3.len == 0 && vec3.cap == 0 && vec3.size == 0 && vec3.data == NULL
        && error3_index == 0 && error3 == VDR_INVALID_VEC
        && error4_index == 2 && error4 == VDR_INVALID_VEC
        && error5_index == 0 && error5 == VDR_INVALID_VEC_DATA
        && ok2_index == 10 && ok2 == VDR_OK
    )
    { passed(); } else { failed(&result); }
    printf("\n");

    return result;
}
