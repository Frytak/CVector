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

int vec_get_test() {
    int result = 0;
    Vector vec = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    // Test 1: `unchecked` - should contain identical data
    start_test(1);

    bool contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; };
    }

    end_test(
        contains_all,
        NULL
    );

    // Test 2: `unchecked` - shouldn't contain identical data
    start_test(2);
    *(int*)vec.data = 9999;

    contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; };
    }

    end_test(
        !contains_all,
        NULL
    );

    // Test 3: error handling
    start_test(3);
    void *error1 = vec_get(NULL, 0);
    void *error2 = vec_get(&vec, DATA1_LEN + 3);
    void *ok = vec_get(&vec, 0);
    drop_test_vecs(vec_drop_single(&vec));
    vec.cap = 8;
    vec.len = 4;
    vec.size = 4;
    // TODO: Test for size = 0
    void *error3 = vec_get(&vec, 0);

    end_test(
        error1 == NULL && error2 == NULL && ok != NULL && error3 == NULL,
        NULL
    );

    return result;
}
