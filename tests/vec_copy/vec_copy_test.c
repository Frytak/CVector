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
    printf("\tTest 1: ");
    vec = vec_new(0, NULL, 0);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: amount only
    printf("\tTest 2: ");
    vec = vec_new(0, NULL, DATA1_LEN);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: size only
    printf("\tTest 3: ");
    vec = vec_new(DATA1_SIZE, NULL, 0);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 4: no data
    printf("\tTest 4: ");
    vec = vec_new(DATA1_SIZE, NULL, DATA1_LEN);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 5: no amount
    printf("\tTest 5: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, 0);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 6: everything provided
    printf("\tTest 6: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_partial_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 7: reserved unusual capacity
    printf("\tTest 7: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_reserve_unchecked(&vec, 7);
    vec_copied = vec_copy_unchecked(&vec);

    if (vec_is_partial_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 8: error handling
    printf("\tTest 8: ");
    vec = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    VEC_COPY_RESULT error1 = vec_copy(NULL, NULL);
    VEC_COPY_RESULT error2 = vec_copy(&vec, NULL);
    VEC_COPY_RESULT ok = vec_copy(&vec, &vec_copied);

    if (error1 == VCR_INVALID_SOURCE && error2 == VCR_INVALID_DESTINATION && ok == VCR_OK && vec_is_partial_eq_unchecked(&vec, &vec_copied))
    { passed(); } else { failed(&result); }
    printf("\n");

    return result;
}
