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
    start_test(1);
    vec = vec_new(0, NULL, 0);

    end_test(
        vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == 0,
        NULL
    );

    // Test 2: amount only
    start_test(2);
    vec = vec_new(0, NULL, DATA1_LEN);

    end_test(
        vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == 0,
        NULL
    );

    // Test 3: size only
    start_test(3);
    vec = vec_new(DATA1_SIZE, NULL, 0);

    end_test(
        vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == DATA1_SIZE,
        NULL
    );

    // Test 4: no data
    start_test(4);
    vec = vec_new(DATA1_SIZE, NULL, DATA1_LEN);

    end_test(
        vec.cap == DATA1_CAP && vec.len == 0 && vec.data != NULL && vec.size == DATA1_SIZE,
        NULL
    );

    // Test 5: no amount
    start_test(5);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, 0);

    end_test(
        vec.cap == 0 && vec.len == 0 && vec.data == NULL && vec.size == DATA1_SIZE,
        NULL
    );

    // Test 6: everything provided
    start_test(6);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    bool contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; }
    }

    end_test(
        vec.cap == DATA1_CAP && vec.len == DATA1_LEN && vec.data != NULL && vec.size == DATA1_SIZE && contains_all,
        vec_drop_single(&vec)
    );

    // Test 7: error handling
    start_test(7);
    VEC_INIT_RESULT error1 = vec_init(NULL, DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    VEC_INIT_RESULT ok = vec_init(&vec, DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    end_test(
        error1 == VIR_INVALID_VEC && ok == VIR_OK,
        vec_drop_single(&vec)
    );

    return result;
}
