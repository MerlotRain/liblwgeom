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
#include "../testbase.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

double s_ellipseTest_tolerance = 0.000001;

int init_ellipse_suite(void)
{
    s_ellipseTest_tolerance = nv_tolerance(s_ellipseTest_tolerance);
    return 0;
}

int clean_ellipse_suite(void)
{
    nv_tolerance(s_ellipseTest_tolerance);
    return 0;
}

void algorithm_ellipse_Test1(void)
{
    printf("void nv_ellipse_Test1(void)\n");
    struct nv_point tc = {.x = 0, .y = 0};
    struct nv_ellipse ellipse;
    int n = 0;
    struct nv_point points[2];
    points[0].x = -5;
    points[0].y = 0;
    points[1].x = 5;
    points[1].y = 0;
    nv_construct_circle((const struct nv_point *)&points,
                        NV_CONSTRUCT_CIRCLE_2P, &ellipse, &n);
    CU_ASSERT(ellipse.major == 5);
    CU_ASSERT(TEST_POINT_EQUAL(ellipse.center, tc, s_ellipseTest_tolerance));
}

void algorithm_ellipse_Test2(void)
{
    struct nv_ellipse ellipse = {
        .center = {.x = 0, .y = 0}, .major = 5, .minor = 2, .azimuth = 0};

    struct nv_geobject *obj =
        nv_stroke_ellipse(ellipse, 24); // gdim = 2, nseg = 4
    CU_ASSERT(nv_point_n(obj) == 5);
}

int algorithm_ellipseSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("algorithm-ellipse", init_ellipse_suite,
                          clean_ellipse_suite);

    if (NULL == CU_add_test(pSuite, "ellipse Test1", algorithm_ellipse_Test1) ||
        NULL == CU_add_test(pSuite, "ellipse Test2", algorithm_ellipse_Test2)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}