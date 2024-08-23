/*****************************************************************************/
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <mg.h>
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
    struct mg_object *obj = mg_read(GEOMETRY_IO_WKT, p, strlen(p));
    CU_ASSERT(obj != NULL);
    CU_ASSERT(mg_dim_g(obj) == 0);
    CU_ASSERT(mg_dim_c(obj) == 2);
    CU_ASSERT(mg_point_n(obj) == 1);
    mg_free_object(obj);
}

void io_wkt_Test2(void)
{
    char p[] = "LINESTRING (30 10, 10 30, 40 40)";
    struct mg_object *obj = mg_read(GEOMETRY_IO_WKT, p, strlen(p));
    CU_ASSERT(obj != NULL);
    CU_ASSERT(mg_dim_g(obj) == 1);
    CU_ASSERT(mg_dim_c(obj) == 2);
    CU_ASSERT(mg_point_n(obj) == 3);
    mg_free_object(obj);
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