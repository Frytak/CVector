#define _FRYTAK_VEC_TEST

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

#define start_test(num) printf("\tTest %d: ", num)
#define drop_test_vecs(func) if (func != VDR_OK) { fprintf(stderr, "\t%sERROR: Failed while dropping a vec.%s", RED, RESET); }
#define end_test(conditions, drop) if (conditions) { passed(); } else { failed(&result); }; drop_test_vecs(drop); printf("\n")

void passed();
int failed(int *code);
