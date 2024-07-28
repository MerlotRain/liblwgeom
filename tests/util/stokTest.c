#include "stok.h"
#include "CUnit/Basic.h"

int init_stok_suite()
{
    return 0;
}

int clean_stok_suite()
{
    return 0;
}

void util_stok_Test1()
{
}

int util_stokSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("util-stok", init_stok_suite, clean_stok_suite);

    if (NULL == CU_add_test(pSuite, "stok Test1", util_stok_Test1)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}