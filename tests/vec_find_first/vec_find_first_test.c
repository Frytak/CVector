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
    VEC_SEARCH_RESULT sresult = -1;
    Vector vec = vec_new(DATA2_SIZE, (void*)DATA2, DATA2_LEN);

    // Test 1: exists in vec
    start_test(1);
    searched_index = 2;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        NULL
    );

    // Test 2: doesn't exist in vec
    start_test(2);
    int value = 34;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&value);

    end_test(
        sresult == VSR_NOT_FOUND,
        NULL
    );

    // Test 3: exists on left edge
    start_test(3);
    searched_index = 0;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        NULL
    );

    // Test 4: exists on right edge
    start_test(4);
    searched_index = DATA2_LEN-1;
    sresult = vec_find_first(&vec, vc_int, 0, DATA2_LEN, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        NULL
    );

    // Test 5: exists in boundry
    start_test(5);
    searched_index = 4;
    sresult = vec_find_first(&vec, vc_int, 2, DATA2_LEN-2, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        NULL
    );

    // Test 6: doesn't exist in boundry
    start_test(6);
    searched_index = 6;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    end_test(
        sresult == VSR_NOT_FOUND,
        NULL
    );

    // Test 7: exists on left edge of boundry
    start_test(7);
    searched_index = 2;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        NULL
    );

    // Test 8: exists on right edge of boundry
    start_test(8);
    searched_index = 5;
    sresult = vec_find_first(&vec, vc_int, 2, 6, &index, (void*)&DATA2[searched_index]);

    end_test(
        index == searched_index && sresult == VSR_OK,
        vec_drop_single(&vec)
    );

    return result;
}
