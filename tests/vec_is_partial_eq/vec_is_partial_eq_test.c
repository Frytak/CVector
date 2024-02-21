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

int vec_is_partial_eq_test() {
    int result = 0;
    Vector vec1;
    Vector vec2;

    // Test 1: same vector
    printf("\tTest 1: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    if (vec_is_partial_eq_unchecked(&vec1, &vec1))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: different vector, difference `data` pointer
    printf("\tTest 2: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_copy_unchecked(&vec1);

    if (vec_is_partial_eq_unchecked(&vec1, &vec2))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: different vector, different `len`
    printf("\tTest 3: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_copy_unchecked(&vec1);
    vec2.len = 4;

    if (!vec_is_partial_eq_unchecked(&vec1, &vec2))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 4: different vector, different `cap`
    printf("\tTest 4: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_copy_unchecked(&vec1);
    vec_reserve_unchecked(&vec2, 7);

    if (!vec_is_partial_eq_unchecked(&vec1, &vec2))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 5: different vector, different `size`
    printf("\tTest 5: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    if (!vec_is_partial_eq_unchecked(&vec1, &vec2))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 6: different vector, different `data`
    printf("\tTest 6: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    *(int*)vec_get_unchecked(&vec2, 0) = 0;

    if (vec_is_partial_eq_unchecked(&vec1, &vec2))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 7: error handling
    printf("\tTest 7: ");
    vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    VEC_EQ_RESULT error1; vec_is_partial_eq(NULL, &vec2, &error1);
    VEC_EQ_RESULT error2; vec_is_partial_eq(&vec1, NULL, &error2);
    VEC_EQ_RESULT ok; vec_is_partial_eq(&vec1, &vec2, &ok);

    if (error1 == VER_INVALID_SOURCE && error2 == VER_INVALID_DESTINATION && ok == VER_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    return result;
}
