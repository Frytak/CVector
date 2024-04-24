#ifndef _FRYTAK_VEC
#include "../../vec.h"
#endif

#ifndef _FRYTAK_VEC_TEST_EXAMPLE_DATA
#include "../example_data/example_data.h"
#endif

#ifndef _FRYTAK_VEC_TEST
#include "../test.h"
#endif

int vec_insertion_sort_test() {
    int result = 0;
    Vector correct = vec_new(DATA1_SIZE, (void*)DATA1_SORTED, DATA1_LEN);
    Vector vec;

    // Test 1
    start_test(1);
    vec = vec_new(DATA1_SIZE, (void*)DATA1, DATA1_LEN);

    vec_insertion_sort(&vec, vbsc_int);

    end_test(
        vec_is_eq_deep(&vec, &correct),
        vec_drop_single_s(&vec)
    );

    return result;
}
