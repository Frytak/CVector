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
    Vector vec;

    // Test 1: Drop a `Vector` with NULL data
    printf("\tTest 1: ");
    vec = vec_new(DATA1_SIZE, NULL, 0);

    vec_drop(&vec);
    if (vec.len == 0 && vec.cap == 0 && vec.data == NULL && vec.size == DATA1_SIZE)
    { passed(); } else { result = 1; failed(result); }

    printf("\n");

    // Test 2: Drop a `Vector` with some data
    printf("\tTest 2: ");
    vec = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_SIZE);

    vec_drop(&vec);
    if (vec.len == 0 && vec.cap == 0 && vec.data == NULL && vec.size == DATA1_SIZE)
    { passed(); } else { result = 2; failed(result); }

    printf("\n");

    vec_drop(&vec);
    return result;
}
