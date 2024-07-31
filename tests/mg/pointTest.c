/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include "mg.h"

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

void mg_point_Test1(void)
{
    struct mg_point p1 = {.x = 10.0, .y = 0};
    struct mg_point p2 = {.x = 10.0, .y = 10.0};
    struct mg_point p3 = {.x = 0.0, .y = 10.0};
    struct mg_point p4 = {.x = -10.0, .y = 10.0};
    struct mg_point p5 = {.x = -10.0, .y = 0};
    struct mg_point p6 = {.x = -10.0, .y = -0.1};
    struct mg_point p7 = {.x = -10.0, .y = -10.0};
    CU_ASSERT_EQUAL(0.0, mg_angle(p1));
    CU_ASSERT_EQUAL(M_PI / 4.0, mg_angle(p2));
    CU_ASSERT_EQUAL(M_PI / 2.0, mg_angle(p3));
    CU_ASSERT_EQUAL(0.75 * M_PI, mg_angle(p4));
    CU_ASSERT_EQUAL(M_PI, mg_angle(p5));
    CU_ASSERT_EQUAL(-3.131592987, mg_angle(p6));
    CU_ASSERT_EQUAL(-0.75 * M_PI, mg_angle(p7));
}

int mg_pointSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("mg-point", init_point_suite, clean_point_suite);

    if (NULL == CU_add_test(pSuite, "point Test1", mg_point_Test1)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}