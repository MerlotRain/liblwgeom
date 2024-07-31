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
        CU_cleanup_registry();
        return CU_get_error();
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
            CU_cleanup_registry();
            return CU_get_error();
        }
    }
    else {
        return 0;
    }
}