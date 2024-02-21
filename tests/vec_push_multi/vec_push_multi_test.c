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

int vec_push_multi_test() {
    int result = 0;
    Vector vec;
    
    // Test 1: `unchecked` - multiple elements at once with enough capacity
    start_test(1);
    vec = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    vec_push_multi_unchecked(&vec, (void*)&INSERT_DATA1, INSERT_DATA1_LEN);

    bool contains_all = true;
    // Check for the initial data
    for (int i = 0; i < DATA2_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA2[i]) { contains_all = false; }
    }
    // Check for additional data
    for (int i = DATA2_LEN; i < DATA2_LEN + INSERT_DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != INSERT_DATA1[i - DATA2_LEN]) { contains_all = false; }
    }

    end_test(
        vec.len == DATA2_LEN+4 && vec.cap == DATA2_CAP && contains_all,
        NULL
    );

    // Test 2: `unchecked` - multiple elements at once without enough capacity
    start_test(2);
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    vec_push_multi_unchecked(&vec, (void*)&INSERT_DATA1, INSERT_DATA1_LEN);

    contains_all = true;
    // Check for the initial data
    for (int i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; }
    }
    // Check for additional data
    for (int i = DATA1_LEN; i < DATA1_LEN + INSERT_DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != INSERT_DATA1[i - DATA1_LEN]) { contains_all = false; }
    }

    end_test(
        vec.len == DATA1_LEN+INSERT_DATA1_LEN && vec.cap == _vec_get_p2_cap(DATA1_LEN+INSERT_DATA1_LEN) && contains_all,
        NULL
    );

    // TODO: Error handling

    return result;
}
