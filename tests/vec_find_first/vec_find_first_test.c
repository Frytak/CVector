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

int vec_find_first_test() {
    int result = 0;
    size_t index = -1;
    size_t searched_index = -1;
    COMP_FUNC_RET sresult = -1;
    Vector vec;

    // Test 1: exists in vec
    printf("\tTest 1: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 2;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 2: doesn't exist in vec
    printf("\tTest 2: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    int value = 34;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&value);

    if (sresult == CF_NOT_FOUND)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 3: exists on left edge
    printf("\tTest 3: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 0;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 4: exists on right edge
    printf("\tTest 4: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = DATA2_LEN-1;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 5: exists in boundry
    printf("\tTest 5: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 4;
    sresult = vec_find_first(&vec, vc_int, 2, DATA2_LEN-2, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 6: doesn't exist in boundry
    printf("\tTest 6: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 6;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    if (sresult == CF_NOT_FOUND)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 7: exists on left edge of boundry
    printf("\tTest 7: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 2;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    // Test 8: exists on right edge of boundry
    printf("\tTest 8: ");
    vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);
    searched_index = 5;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    if (index == searched_index && sresult == CF_OK)
    { passed(); } else { failed(&result); }
    printf("\n");

    return result;
}
