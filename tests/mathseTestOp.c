/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <CUnit/Basic.h>
#include <CUnit/TestDB.h>

#include <stddef.h>

#include "mathseTestUtil.h"

int main(int argc, char **argv)
{
    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    int rs = register_suite();
    if (rs != CUE_SUCCESS)
        return CU_get_error();

    if (argc == 1) {
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        int fc = CU_count_all_failures(CU_get_registry());
        CU_cleanup_registry();
        return fc;
    }
    else if (argc == 2) {
        char *sutie = argv[1];
        CU_pSuite pSuite = CU_get_suite(sutie);
        if (!pSuite) {
            return CU_get_error();
        }
        else {
            CU_basic_set_mode(CU_BRM_VERBOSE);
            CU_basic_run_suite(pSuite);
            int fc = CU_count_suite_failures(pSuite);
            CU_cleanup_registry();
            return fc;
        }
    }
    else {
        return 0;
    }
}