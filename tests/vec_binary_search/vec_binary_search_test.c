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
    Vector vec = vec_new(DATA4_SIZE, (int*)DATA4, DATA4_LEN);

    // Test 1: exists in vec
    start_test(1);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 2: doesn't exist in vec
    start_test(2);
    int value = 120;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, &value);

    end_test(
        sresult == VBSR_NOT_FOUND,
        NULL
    );

    // Test 3: exists on left edge
    start_test(3);
    searched_index = 0;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 4: exists on right edge
    start_test(4);
    searched_index = DATA4_LEN-1;
    sresult = vec_binary_search(&vec, vbsc_int, 0, DATA4_LEN, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 5: exists in boundry
    start_test(5);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 6: doesn't exist in boundry
    start_test(6);
    searched_index = 8;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    end_test(
        sresult == VBSR_NOT_FOUND,
        NULL
    );

    // Test 7: exists on left edge of boundry
    start_test(7);
    searched_index = 2;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 8: exists on right edge of boundry
    start_test(8);
    searched_index = 5;
    sresult = vec_binary_search(&vec, vbsc_int, 2, 6, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        vec_drop_single(&vec)
    );

    // TODO: Error handling test

    return result;
}
