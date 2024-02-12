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

int vec_new_test() {
    int result = 0;
    Vector vec;

    // Test 1: all 0/NULL
    printf("\tTest 1: ");
    vec = vec_new(0, NULL, 0);

    if (vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == 0)
    { passed(); } else { failed(result = 1); }
    printf("\n");

    // Test 2: amount only
    printf("\tTest 2: ");
    vec = vec_new(0, NULL, DATA1_LEN);

    if (vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == 0)
    { passed(); } else { failed(result = 2); }
    printf("\n");

    // Test 3: size only
    printf("\tTest 3: ");
    vec = vec_new(DATA1_SIZE, NULL, 0);

    if (vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == DATA1_SIZE)
    { passed(); } else { failed(result = 3); }
    printf("\n");

    // Test 4: no data
    printf("\tTest 4: ");
    vec = vec_new(DATA1_SIZE, NULL, DATA1_LEN);

    if (vec.cap == _vec_get_p2_cap(DATA1_LEN) && vec.len == 0 && vec.data == NULL && vec.size == DATA1_SIZE)
    { passed(); } else { failed(result = 4); }
    printf("\n");

    // Test 5: no amount
    printf("\tTest 5: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, 0);

    if (vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == DATA1_SIZE)
    { passed(); } else { failed(result = 5); }
    printf("\n");

    // Test 6: everything provided
    printf("\tTest 6: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    bool contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; }
    }

    if (vec.cap == DATA1_CAP && vec.len == DATA1_LEN && vec.data != NULL && vec.size == DATA1_SIZE && contains_all)
    { passed(); } else { failed(result = 6); }
    printf("\n");

    // Test 7: error handling
    printf("\tTest 7: ");
    VEC_INIT_RESULT error1 = vec_init(NULL, DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    VEC_INIT_RESULT ok = vec_init(&vec, DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    if (error1 == VIR_INVALID_VEC && ok == VIR_OK)
    { passed(); } else { failed(result = 7); }
    printf("\n");

    vec_drop(&vec);
    return result;
}
