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
    Vector vec;

    // Test 1: `unchecked` - should contain identical data
    printf("\tTest 1: ");
    vec = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    bool contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; };
    }

    if (contains_all)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: `unchecked` - shouldn't contain identical data
    printf("\tTest 2: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    *(int*)vec.data = 9999;

    contains_all = true;
    for (size_t i = 0; i < DATA1_LEN; i++) {
        if (*(int*)vec_get_unchecked(&vec, i) != DATA1[i]) { contains_all = false; };
    }

    if (!contains_all)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: error handling
    printf("\tTest 3: ");
    vec = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    void *error1 = vec_get(NULL, 0);
    void *error2 = vec_get(&vec, DATA1_LEN + 3);
    void *ok = vec_get(&vec, 0);
    vec.data = NULL;
    void *error3 = vec_get(&vec, 0);

    if (error1 == NULL && error2 == NULL && ok != NULL && error3 == NULL)
    { passed(); } else { failed(&result); }
    printf("\n");

    vec_drop(&vec);
    return result;
}
