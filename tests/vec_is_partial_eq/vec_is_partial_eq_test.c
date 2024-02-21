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
    Vector vec1 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    Vector vec2;

    // Test 1: same vector
    start_test(1);
    end_test(
        vec_is_partial_eq_unchecked(&vec1, &vec1),
        NULL
    );

    // Test 2: different vector, difference `data` pointer
    start_test(2);
    vec2 = vec_copy_unchecked(&vec1);

    end_test(
        vec_is_partial_eq_unchecked(&vec1, &vec2),
        NULL
    );

    // Test 3: different vector, different `len`
    start_test(3);
    vec2.len = 4;

    end_test(
        !vec_is_partial_eq_unchecked(&vec1, &vec2),
        vec_drop_single(&vec2)
    );

    // Test 4: different vector, different `cap`
    start_test(4);
    vec2 = vec_copy_unchecked(&vec1);
    vec_reserve_unchecked(&vec2, 7);

    end_test(
        !vec_is_partial_eq_unchecked(&vec1, &vec2),
        vec_drop_single(&vec2)
    );

    // Test 5: different vector, different `size`
    start_test(5);
    vec2 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    end_test(
        !vec_is_partial_eq_unchecked(&vec1, &vec2),
        vec_drop_single(&vec2)
    );

    // Test 6: different vector, different `data`
    start_test(6);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    *(int*)vec_get_unchecked(&vec2, 0) = 0;

    end_test(
        vec_is_partial_eq_unchecked(&vec1, &vec2),
        vec_drop_single(&vec2)
    );

    // Test 7: error handling
    start_test(7);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    VEC_EQ_RESULT error1; vec_is_partial_eq(NULL, &vec2, &error1);
    VEC_EQ_RESULT error2; vec_is_partial_eq(&vec1, NULL, &error2);
    VEC_EQ_RESULT ok; vec_is_partial_eq(&vec1, &vec2, &ok);

    end_test(
        error1 == VER_INVALID_SOURCE && error2 == VER_INVALID_DESTINATION && ok == VER_OK,
        vec_drop(NULL, &vec1, &vec2)
    );

    return result;
}
