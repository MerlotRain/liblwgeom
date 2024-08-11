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
#include "CUnit/CUError.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

struct bitset *bs;

int init_bitset_suite(void)
{
    bs = bitset_new(20); // 8 8 8 8
    bitset_set(bs, 1);   // 0
    bitset_set(bs, 3);   // 0
    bitset_set(bs, 5);   // 0
    bitset_set(bs, 17);  // 0
    return CUE_SUCCESS;
}

int clean_bitset_suite(void)
{
    bitset_free(bs);
    return CUE_SUCCESS;
}

void mg_bitset_Test1(void)
{
    CU_ASSERT(bitset_test(bs, 1) == true);
    CU_ASSERT(bitset_test(bs, 3) == true);
    CU_ASSERT(bitset_test(bs, 5) == true);
    CU_ASSERT(bitset_test(bs, 17) == true);
}

void mg_bitset_Test2(void)
{
    CU_ASSERT(bitset_test(bs, 2) == false);
    CU_ASSERT(bitset_test(bs, 10) == false);
    CU_ASSERT(bitset_test(bs, 15) == false);
    CU_ASSERT(bitset_test(bs, 20) == false);
}

void mg_bitset_Test3(void)
{
    CU_ASSERT(bitset_count(bs) == 4);
    bitset_flip(bs, 1);
    CU_ASSERT(bitset_test(bs, 1) == false);
    CU_ASSERT(bitset_count(bs) == 3);
}

void mg_bitset_Test4(void)
{
    bitset_clear(bs, 2);
    CU_ASSERT(bitset_count(bs) == 3);
}

int mg_bitsetSuite()
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("util-bitset", init_bitset_suite, clean_bitset_suite);

    if (NULL == CU_add_test(pSuite, "bitset Test1", mg_bitset_Test1) ||
        NULL == CU_add_test(pSuite, "bitset Test2", mg_bitset_Test2) ||
        NULL == CU_add_test(pSuite, "bitset Test3", mg_bitset_Test3) ||
        NULL == CU_add_test(pSuite, "bitset Test4", mg_bitset_Test4)) {
        return CU_get_error();
    }
    return CUE_SUCCESS;
}