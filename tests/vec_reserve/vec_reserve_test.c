#include <string.h>
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

int vec_reserve_test() {
    int result = 0;
    size_t new_cap;
    Vector vec;

    // Test 1: reserve memory that will be the next power of two for a valid vector (8 -> 16)
    start_test(1);
    vec = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);
    new_cap = 16;
    vec_reserve(&vec, new_cap);

    end_test(
        vec.len == DATA1_LEN && vec.cap == new_cap && vec.size == DATA1_SIZE && vec.data != NULL
        && (memcmp(DATA1, vec.data, DATA1_SIZE * DATA1_LEN) == 0),
        NULL
    );

    // Test 2: reserve memory in different quantity than a power of two for the same vector (16 -> 19)
    start_test(2);
    new_cap = 19;
    vec_reserve(&vec, new_cap);

    end_test(
        vec.len == DATA1_LEN && vec.cap == new_cap && vec.size == DATA1_SIZE && vec.data != NULL
        && (memcmp(DATA1, vec.data, DATA1_SIZE * DATA1_LEN) == 0),
        NULL
    );

    // Test 3: reserve less memory than the currents vector length for the same vector (19 -> 3)
    start_test(3);
    new_cap = 3;
    vec_reserve(&vec, new_cap);

    end_test(
        vec.len == new_cap && vec.cap == new_cap && vec.size == DATA1_SIZE && vec.data != NULL
        && (memcmp(DATA1, vec.data, DATA1_SIZE * new_cap) == 0),
        NULL
    );

    // Test 4: reserve no memory for the same vector (3 -> 0)
    start_test(4);
    new_cap = 0;
    vec_reserve(&vec, new_cap);

    end_test(
        vec.len == new_cap && vec.cap == new_cap && vec.size == DATA1_SIZE && vec.data == NULL,
        NULL
    );

    // Test 5: reserve some memory for the same vector (currently NULL data vector) (0 -> 8)
    start_test(5);
    new_cap = 8;
    vec_reserve(&vec, 8);

    end_test(
        vec.len == 0 && vec.cap == new_cap && vec.size == DATA1_SIZE && vec.data != NULL,
        NULL
    );

    // Test 6: error handling
    start_test(6);
    VEC_RESERVE_RESULT error1 = vec_reserve_s(NULL, 13);
    VEC_RESERVE_RESULT ok = vec_reserve_s(&vec, 13);

    end_test(
        error1 == VRR_INVALID_VEC && ok == VRR_OK,
        vec_drop_single_s(&vec)
    );

    return result;
}
