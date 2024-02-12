#ifndef _FRYTAK_VEC
#include "../vec.h"
#endif

#ifndef _FRYTAK_VEC_TEST
#include "test.h"
#endif

#include "./vec_drop/vec_drop_test.h"
#include "./vec_new/vec_new_test.h"
#include "./vec_get/vec_get_test.h"
#include "./vec_push/vec_push_test.h"
#include "./vec_push_multi/vec_push_multi_test.h"

#include <stdio.h>

void passed() { printf("%sPASSED%s", GREEN, RESET); }
int failed(int code) { printf("%sFAILED(%d)%s", RED, code, RESET); return code; }

int test_func(char name[], int (*func)()) {
    printf("%s: \n", name);
    int result = func();
    printf("\tEnd result: ");
    if (!result) { passed(); return 0; } else { return failed(result); }
}

int main() {
    int result = 0;
    result += test_func("vec_drop", vec_drop_test); printf("\n");
    result += test_func("vec_new", vec_new_test); printf("\n");
    result += test_func("vec_get", vec_get_test); printf("\n");
    result += test_func("vec_push", vec_push_test); printf("\n");
    result += test_func("vec_push_multi", vec_push_multi_test); printf("\n");
    printf("\nThe overall result of the tests: ");
    if (result == 0) { passed(); } else { failed(0); }
}
