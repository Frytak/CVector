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

    // Test 1: All zero values
    // Everything should be 0/NULL
    printf("\tTest 1: ");
    Vector vec1 = vec_new(0, NULL, 0);

    if (vec1.cap == 0 && vec1.len == 0 && vec1.data == NULL && vec1.size == 0)
    { passed(); } else { result = 1; failed(result); }
    vec_drop(&vec1);
    printf("\n");

    // Test 2: Amount without data and size
    // Everything should be 0/NULL
    printf("\tTest 2: ");
    Vector vec2 = vec_new(0, NULL, 16);

    if (vec2.cap == 0 && vec2.len == 0 && vec2.data == NULL && vec2.size == 0)
    { passed(); } else { result = 2; failed(result); }
    vec_drop(&vec2);
    printf("\n");

    // Test 3: Size without data and amount
    // Everything should be 0/NULL besides size
    printf("\tTest 3: ");
    Vector vec3 = vec_new(DATA1_SIZE, NULL, 0);

    if (vec3.cap == 0 && vec3.len == 0 && vec3.data == NULL && vec3.size == DATA1_SIZE)
    { passed(); } else { result = 3; failed(result); }
    vec_drop(&vec3);
    printf("\n");

    // Test 4: Amount without data
    // `len` should be 0 as we didn't insert any data
    printf("\tTest 4: ");
    Vector vec4 = vec_new(DATA1_SIZE, NULL, DATA1_LEN);

    if (vec4.cap == 8 && vec4.len == 0 && vec4.data == NULL && vec4.size == DATA1_SIZE)
    { passed(); } else { result = 4; failed(result); }
    vec_drop(&vec4);
    printf("\n");

    // Test 5: Correct data
    printf("\tTest 5: ");
    Vector vec5 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);

    bool contains_all = true;
    for (size_t i = 0; i < vec5.len; i++) {
        if (*(int*)vec_get_unchecked(&vec5, i) != DATA1[i]) { contains_all = false; }
    }
    if (vec5.cap == 8 && vec5.len == DATA1_LEN && vec5.data != NULL && vec5.size == DATA1_SIZE && contains_all)
    { passed(); } else { result = 5; failed(result); }
    vec_drop(&vec5);
    printf("\n");


    return result;
}
