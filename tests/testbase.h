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