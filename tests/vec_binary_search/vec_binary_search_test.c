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
        NULL
    );

    // Test 9: last element of vec with boundry only at this element
    start_test(9);
    searched_index = vec.len-1;
    sresult = vec_binary_search(&vec, vbsc_int, vec.len-1, vec.len, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 10: first element of vec with boundry only at this element
    start_test(10);
    searched_index = 0;
    sresult = vec_binary_search(&vec, vbsc_int, 0, 1, &index, (void*)&DATA4[searched_index]);

    end_test(
        index == searched_index && sresult == VBSR_OK,
        NULL
    );

    // Test 11: error handling
    start_test(11);
    searched_index = 1;
    VEC_BINARY_SEARCH_RESULT error1 = vec_binary_search(&vec, vbsc_int, 2, vec.len, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error2 = vec_binary_search(NULL, vbsc_int, 0, vec.len, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error3 = vec_binary_search(&vec, NULL, 0, vec.len, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error4 = vec_binary_search(&vec, vbsc_int, 0, vec.len, &index, NULL);
    VEC_BINARY_SEARCH_RESULT error5 = vec_binary_search(&vec, vbsc_int, 128, 234, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error6 = vec_binary_search(&vec, vbsc_int, vec.len, vec.len+1, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error7 = vec_binary_search(&vec, vbsc_int, vec.len, 2, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT error8 = vec_binary_search(&vec, vbsc_int, vec.len, vec.len, &index, (void*)&DATA4[searched_index]);
    VEC_BINARY_SEARCH_RESULT ok = vec_binary_search(&vec, vbsc_int, 0, vec.len, &index, (void*)&DATA4[searched_index]);

    end_test(
        error1 == VBSR_NOT_FOUND
        && error2 == VBSR_INVALID_VEC
        && error3 == VBSR_INVALID_COMP
        && error4 == VBSR_INVALID_SEARCHED
        && error5 == VBSR_OUT_OF_BOUNDS && error6 == VBSR_OUT_OF_BOUNDS
        && error7 == VBSR_INVALID_BOUNDS && error8 == VBSR_INVALID_BOUNDS
        && ok == VBSR_OK && index == searched_index,
        vec_drop_single(&vec)
    );

    return result;
}
