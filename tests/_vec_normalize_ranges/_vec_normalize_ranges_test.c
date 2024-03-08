#include <string.h>
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

// TODO: more info next to the implementation of the function
int _vec_normalize_ranges_test() {
    int result = 0;
    Vector vec;

    // Test 1: already normalized ranges, nothing should change
    start_test(1);
    size_t ranges1[] = {
        0, 1,
        2, 4,
        6, 9,
        12, 45,
        47, 49
    };

    Vector normalized_ranges1 = _vec_normalize_ranges(ranges1, (sizeof(ranges1)/sizeof(ranges1[0]))/2);

    end_test(
        normalized_ranges1.len == 10 && normalized_ranges1.size == sizeof(ranges1[0]) && normalized_ranges1.data != NULL
        && (memcmp(ranges1, normalized_ranges1.data, sizeof(ranges1)) == 0),
        vec_drop_single(&normalized_ranges1)
    );

    // Test 2: out of order ranges, otherwise normalized
    start_test(2);
    size_t ranges2[] = {
        3, 10,
        0, 3,
        28, 30,
        12, 14,
    };

    Vector normalized_ranges2 = _vec_normalize_ranges(ranges2, (sizeof(ranges2)/sizeof(ranges2[0]))/2);

    end_test(
        normalized_ranges2.len == sizeof(ranges2)/sizeof(ranges2[0]) && normalized_ranges2.size == sizeof(ranges2[0]) && normalized_ranges2.data != NULL
        && *(size_t*)vec_get(&normalized_ranges2, 0) == ranges2[2] && *(size_t*)vec_get(&normalized_ranges2, 1) == ranges2[3]
        && *(size_t*)vec_get(&normalized_ranges2, 2) == ranges2[0] && *(size_t*)vec_get(&normalized_ranges2, 3) == ranges2[1]
        && *(size_t*)vec_get(&normalized_ranges2, 4) == ranges2[6] && *(size_t*)vec_get(&normalized_ranges2, 5) == ranges2[7]
        && *(size_t*)vec_get(&normalized_ranges2, 6) == ranges2[4] && *(size_t*)vec_get(&normalized_ranges2, 7) == ranges2[5],
        vec_drop_single(&normalized_ranges2)
    );

    // Test 3: overlaping beg and end
    start_test(3);
    size_t ranges3[] = {
        0, 3,
        3, 10,
        10, 12,
        24, 28,
        28, 30,
    };

    Vector normalized_ranges3 = _vec_normalize_ranges(ranges3, (sizeof(ranges3)/sizeof(ranges3[0]))/2);

    end_test(
        normalized_ranges3.len == 2 && normalized_ranges3.size == sizeof(ranges3[0]) && normalized_ranges3.data != NULL
        && *(size_t*)vec_get(&normalized_ranges3, 0) == ranges3[0] && *(size_t*)vec_get(&normalized_ranges3, 1) == ranges3[5]
        && *(size_t*)vec_get(&normalized_ranges3, 2) == ranges3[6] && *(size_t*)vec_get(&normalized_ranges3, 3) == ranges3[9],
        vec_drop_single(&normalized_ranges3)
    );

    return result;
}
