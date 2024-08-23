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