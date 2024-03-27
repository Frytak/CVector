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

int vec_copy_test() {
    int result = 0;
    Vector vec;
    Vector vec_copied;

    // Test 1: all 0/NULL
    start_test(1);
    vec = vec_new(0, NULL, 0);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_eq(&vec, &vec_copied),
        NULL
    );

    // Test 2: amount only
    start_test(2);
    vec = vec_new(0, NULL, DATA1_LEN);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_eq(&vec, &vec_copied),
        NULL
    );

    // Test 3: size only
    start_test(3);
    vec = vec_new(DATA1_SIZE, NULL, 0);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_eq(&vec, &vec_copied),
        NULL
    );

    // Test 4: no data
    start_test(4);
    vec = vec_new(DATA1_SIZE, NULL, DATA1_LEN);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_partial_eq_s(&vec, &vec_copied),
        vec_drop_s(&vec, &vec_copied)
    );

    // Test 5: no amount
    start_test(5);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, 0);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_eq(&vec, &vec_copied),
        NULL
    );

    // Test 6: everything provided
    start_test(6);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_partial_eq_s(&vec, &vec_copied),
        vec_drop_s(&vec, &vec_copied)
    );

    // Test 7: reserved unusual capacity
    start_test(7);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_reserve(&vec, 7);
    vec_copied = vec_copy_s(&vec);

    end_test(
        vec_is_partial_eq_s(&vec, &vec_copied),
        vec_drop_s(&vec, &vec_copied)
    );

    // Test 8: error handling
    start_test(8);
    vec = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    VEC_RESULT error1 = vec_copy_into_s(NULL, NULL);
    VEC_RESULT error2 = vec_copy_into_s(&vec, NULL);
    VEC_RESULT ok = vec_copy_into_s(&vec, &vec_copied);

    end_test(
        error1 == VECR_NULL_SOURCE && error2 == VECR_NULL_DESTINATION && ok == VECR_OK && vec_is_partial_eq_s(&vec, &vec_copied),
        vec_drop_s(&vec, &vec_copied)
    );

    return result;
}
