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
    printf("\tTest 1: ");
    vec = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    vec_push_unchecked(&vec, (void*)&INSERT_DATA1[0]);

    if (vec.len == DATA2_LEN+1 && vec.cap == DATA2_CAP && *(int*)vec_get_unchecked(&vec, DATA2_LEN) == INSERT_DATA1[0])
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: `unchecked` - not enough capacity for additional elements
    printf("\tTest 2: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_push_unchecked(&vec, (void*)&INSERT_DATA1[0]);
    vec_push_unchecked(&vec, (void*)&INSERT_DATA1[1]);

    if (vec.len == DATA1_LEN+2 && vec.cap == 16 && *(int*)vec_get_unchecked(&vec, DATA1_LEN) == INSERT_DATA1[0] && *(int*)vec_get_unchecked(&vec, DATA1_LEN+1) == INSERT_DATA1[1])
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: error handling
    printf("\tTest 3: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    VEC_PUSH_RESULT error1 = vec_push(NULL, (void*)&INSERT_DATA1[0]);
    VEC_PUSH_RESULT error2 = vec_push(&vec, NULL);
    VEC_PUSH_RESULT ok = vec_push(&vec, (void*)&INSERT_DATA1[0]);
    vec.data = NULL;
    VEC_PUSH_RESULT error3 = vec_push(&vec, (void*)&INSERT_DATA1[0]);

    if (error1 == VPR_INVALID_VEC && error2 == VPR_INVALID_DATA && ok == VPR_OK && error3 == VPR_INVALID_VEC_DATA)
    { passed(); } else { failed(&result); }
    printf("\n");

    return result;
}
