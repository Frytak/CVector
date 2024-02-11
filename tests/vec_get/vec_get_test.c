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

    // Test 1: `vec_get_unchecked`
    printf("\tTest 1: ");
    Vector vec1 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    bool got_all1 = true;
    for (size_t i = 0; i < vec1.len; i++) {
        if (*(int*)vec_get_unchecked(&vec1, i) != DATA1[i]) { got_all1 = false; };
    }
    if (got_all1) { passed(); } else { result = 1; failed(result); }
    vec_drop(&vec1);
    printf("\n");

    // Test 2: successfull `vec_get`
    printf("\tTest 2: ");
    Vector vec2 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    bool got_all2 = true;
    for (size_t i = 0; i < vec2.len; i++) {
        if (*(int*)vec_get(&vec2, i) != DATA1[i]) { got_all2 = false; };
    }
    if (got_all2) { passed(); } else { result = 2; failed(result); }
    vec_drop(&vec2);
    printf("\n");

    // Test 3: out of bounds `vec_get`
    printf("\tTest 3: ");
    Vector vec3 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    if (vec_get(&vec3, DATA1_SIZE + 3) == NULL) { passed(); } else { result = 3; failed(result); }
    vec_drop(&vec3);
    printf("\n");

    return result;
}
