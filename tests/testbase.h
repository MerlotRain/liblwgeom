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

#ifndef TESTBASE_H
#define TESTBASE_H

#ifdef __cplusplus
extern "C" {
#endif

/// @brief calculate point distance
#define TEST_POINTDISTANCE(x0, y0, x1, y1) \
    sqrt(pow((x0) - (x1), 2) + pow((y0) - (y1), 2))
#define TEST_POINTDISTANCE2(A, B)      TEST_POINTDISTANCE((A).x, (A).y, (B).x, (B).y)

/// @brief check point A, B is equal
#define TEST_POINT_EQUAL(A, B, tol)    (TEST_POINTDISTANCE2((A), (B)) < (tol))

/// @brief check double value is equal
#define TEST_DOUBLE_NEARES(A, tol)     (fabs((A)) < (tol))

/// @brief check double value is equal
#define TEST_DOUBLE_NEARES2(A, B, tol) (fabs((A) - (B)) < (tol))

#ifdef __cplusplus
}
#endif

#endif