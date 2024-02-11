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

int vec_push_test() {
    int result = 0;
    
    // Test 1: `unchecked` - enough capacity for additional element
    printf("\tTest 1: ");
    Vector vec1 = vec_new(DATA2_SIZE, (void*)&DATA2, DATA2_LEN);
    int data1 = 43;
    vec_push_unchecked(&vec1, (void*)&data1);

    if (vec1.len == DATA1_LEN + 1 && vec1.cap == 8 && *(int*)vec_get_unchecked(&vec1, DATA1_LEN) == data1) { passed(); } else { result = 1; failed(result); }
    vec_drop(&vec1);
    printf("\n");

    // Test 2: not enough capacity for additional elements
    //printf("\tTest 2: ");
    //Vector vec2 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    //int data2[] = {43, -94};
    //vec_push(&vec2, (void*)&data2[0]);
    //vec_push(&vec2, (void*)&data2[1]);

    //if (vec2.len == DATA1_LEN + 2 && vec2.cap == 16 && *(int*)vec_get_unchecked(&vec2, DATA1_LEN) == data2[0] && *(int*)vec_get_unchecked(&vec2, DATA1_LEN + 1) == data2[1]) { passed(); } else { result = 2; failed(result); }
    //vec_drop(&vec2);
    //printf("\n");

    // Test 4: Multiple elements at once without enough capacity
    printf("\tTest 4: ");
    Vector vec4 = vec_new(DATA1_SIZE, (void*)&DATA1, DATA1_LEN);
    int data4[] = {43, -94, 12, 87};

    vec_push_multi_unchecked(&vec4, (void*)&data4, 4);

    bool contains_all4 = true;
    for (int i = DATA1_LEN; i < DATA1_LEN + 4; i++) {
        if (*(int*)vec_get_unchecked(&vec4, i) != data4[i - DATA1_LEN]) { contains_all4 = false; }
    }

    if (vec4.len == DATA1_LEN + 4 && vec4.cap == 16 && contains_all4)
    { passed(); } else { result = 2; failed(result); }

    vec_drop(&vec4);
    printf("\n");

    return result;
}
