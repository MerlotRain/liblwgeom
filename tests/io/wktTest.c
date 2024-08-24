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
#include <string.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

int init_wkt_suite(void)
{
    return 0;
}

int clean_wkt_suite(void)
{
    return 0;
}

void io_wkt_Test1(void)
{
    char p[] = "POINT (30 10)";
    struct nv_geobject *obj = nv_geo_read(NV_GEOMETRY_IO_WKT, p, strlen(p));
    CU_ASSERT(obj != NULL);
    CU_ASSERT(nv_geo_dim_g(obj) == 0);
    CU_ASSERT(nv_geo_dim_c(obj) == 2);
    CU_ASSERT(nv_geo_point_n(obj) == 1);
    nv_geo_free(obj);
}

void io_wkt_Test2(void)
{
    char p[] = "LINESTRING (30 10, 10 30, 40 40)";
    struct nv_geobject *obj = nv_geo_read(NV_GEOMETRY_IO_WKT, p, strlen(p));
    CU_ASSERT(obj != NULL);
    CU_ASSERT(nv_geo_dim_g(obj) == 1);
    CU_ASSERT(nv_geo_dim_c(obj) == 2);
    CU_ASSERT(nv_geo_point_n(obj) == 3);
    nv_geo_free(obj);
}

int io_wktSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("io-wkt", init_wkt_suite, clean_wkt_suite);

    if (NULL == CU_add_test(pSuite, "wkt Test1", io_wkt_Test1) ||
        NULL == CU_add_test(pSuite, "wkt Test2", io_wkt_Test2)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}