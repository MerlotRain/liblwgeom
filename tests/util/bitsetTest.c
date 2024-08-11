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

#include "bitset.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

struct bitset *bs;

int init_bitset_suite(void)
{
    return 0;
}

int clean_bitset_suite(void)
{
    return 0;
}

void mg_bitset_Test1(void)
{
}

int mg_bitsetSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("mg-bitset", init_bitset_suite, clean_bitset_suite);

    if (NULL == CU_add_test(pSuite, "bitset Test1", mg_bitset_Test1)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}