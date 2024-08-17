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

#include "mghelp.h"
#include "mg.h"
#include "../testbase.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

double s_ellipseTest_tolerance = 0.000001;

int init_ellipse_suite(void)
{
    s_ellipseTest_tolerance = mg_tolerance(s_ellipseTest_tolerance);
    return 0;
}

int clean_ellipse_suite(void)
{
    mg_tolerance(s_ellipseTest_tolerance);
    return 0;
}

void mg_ellipse_Test1(void)
{
    printf("void mg_ellipse_Test1(void)\n");
    struct mg_point tc = {.x = 0, .y = 0};
    struct mg_ellipse ellipse;
    int n = 0;
    struct mg_point points[2];
    points[0].x = -5;
    points[0].y = 0;
    points[1].x = 5;
    points[1].y = 0;
    mg_construct_circle((const struct mg_point *)&points,
                        MG_CONSTRUCT_CIRCLE_2P, &ellipse, &n);
    CU_ASSERT(ellipse.major == 5);
    CU_ASSERT(TEST_POINT_EQUAL(ellipse.center, tc, s_ellipseTest_tolerance));
}

void mg_ellipse_Test2(void)
{
    struct mg_ellipse ellipse = {
        .center = {.x = 0, .y = 0}, .major = 5, .minor = 2, .azimuth = 0};

    struct mg_object *obj =
        mg_stroke_ellipse(ellipse, 24); // gdim = 2, nseg = 4
    CU_ASSERT(mg_point_n(obj) == 5);
}

int mg_ellipseSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite =
        CU_add_suite("mg-ellipse", init_ellipse_suite, clean_ellipse_suite);

    if (NULL == CU_add_test(pSuite, "ellipse Test1", mg_ellipse_Test1) ||
        NULL == CU_add_test(pSuite, "ellipse Test2", mg_ellipse_Test2)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}