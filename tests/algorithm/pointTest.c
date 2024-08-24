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

#include <nv.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

int init_point_suite(void)
{
    return 0;
}

int clean_point_suite(void)
{
    return 0;
}

void algorithm_point_Test1(void)
{
    struct nv_point p1 = {.x = 10.0, .y = 0};
    struct nv_point p2 = {.x = 10.0, .y = 10.0};
    struct nv_point p3 = {.x = 0.0, .y = 10.0};
    struct nv_point p4 = {.x = -10.0, .y = 10.0};
    struct nv_point p5 = {.x = -10.0, .y = 0};
    struct nv_point p6 = {.x = -10.0, .y = -0.1};
    struct nv_point p7 = {.x = -10.0, .y = -10.0};
    CU_ASSERT_EQUAL(0.0, nv_angle(p1));
    CU_ASSERT_EQUAL(M_PI / 4.0, nv_angle(p2));
    CU_ASSERT_EQUAL(M_PI / 2.0, nv_angle(p3));
    CU_ASSERT_EQUAL(0.75 * M_PI, nv_angle(p4));
    CU_ASSERT_EQUAL(M_PI, nv_angle(p5));
    CU_ASSERT_EQUAL(-3.131592987, nv_angle(p6));
    CU_ASSERT_EQUAL(-0.75 * M_PI, nv_angle(p7));
}

int algorithm_pointSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite =
        CU_add_suite("algorithm-point", init_point_suite, clean_point_suite);

    if (NULL == CU_add_test(pSuite, "point Test1", algorithm_point_Test1)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}