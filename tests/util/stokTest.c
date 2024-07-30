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