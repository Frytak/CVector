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
        vec_is_partial_eq(&vec1, &vec1),
        NULL
    );

    // Test 2: different vector, difference `data` pointer
    start_test(2);
    vec2 = vec_copy_s(&vec1);

    end_test(
        vec_is_partial_eq(&vec1, &vec2),
        NULL
    );

    // Test 3: different vector, different `len`
    start_test(3);
    vec2.len = 4;

    end_test(
        !vec_is_partial_eq(&vec1, &vec2),
        vec_drop_single_s(&vec2)
    );

    // Test 4: different vector, different `cap`
    start_test(4);
    vec2 = vec_copy_s(&vec1);
    vec_reserve(&vec2, 7);

    end_test(
        !vec_is_partial_eq(&vec1, &vec2),
        vec_drop_single_s(&vec2)
    );

    // Test 5: different vector, different `size`
    start_test(5);
    vec2 = vec_new(DATA5_SIZE, (void*)DATA5, DATA5_LEN);

    end_test(
        !vec_is_partial_eq(&vec1, &vec2),
        vec_drop_single_s(&vec2)
    );

    // Test 6: different vector, different `data`
    start_test(6);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    *(int*)vec_get_s(&vec2, 0) = 0;

    end_test(
        vec_is_partial_eq(&vec1, &vec2),
        vec_drop_single_s(&vec2)
    );

    // Test 7: error handling
    start_test(7);
    vec2 = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    end_test(
        !vec_is_partial_eq_s(NULL, &vec2) && !vec_is_partial_eq_s(&vec2, NULL) && vec_is_partial_eq_s(&vec2, &vec2),
        vec_drop_s(&vec1, &vec2)
    );

    return result;
}
