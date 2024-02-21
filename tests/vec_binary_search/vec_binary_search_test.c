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

int vec_binary_search_test() {
    int result = 0;
    size_t index = -1;
    size_t searched_index = -1;
    VEC_BINARY_SEARCH_RESULT sresult = -1;
    Vector vec;

    // Test 1: exists in vec
    printf("\tTest 1: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: doesn't exist in vec
    printf("\tTest 2: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    int value = 120;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, &value);

    if (sresult == VBSR_NOT_FOUND)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: exists on left edge
    printf("\tTest 3: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 0;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 4: exists on right edge
    printf("\tTest 4: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = DATA4_LEN-1;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 5: exists in boundry
    printf("\tTest 5: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 6: doesn't exist in boundry
    printf("\tTest 6: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 8;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    if (sresult == VBSR_NOT_FOUND)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 7: exists on left edge of boundry
    printf("\tTest 7: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 2;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 8: exists on right edge of boundry
    printf("\tTest 8: ");
    vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    if (index == searched_index && sresult == VBSR_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // TODO: Error handling test

    return result;
}
