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

#ifndef ORDINATE_H
#define ORDINATE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cpluscplus
extern "C" {
#endif

enum {
    UV__ORDINATE_VALUE_X = 1,
    UV__ORDINATE_VALUE_Y = 2,
    UV__ORDINATE_VALUE_Z = 4,
    UV__ORDINATE_VALUE_M = 8
};

struct uv__ordinate {
    uint8_t value;
    bool changeAllowed; ///< allow change
};

struct uv__ordinate uv__ordinate_XY();
void uv__ordinate_setZ(struct uv__ordinate *o, bool v);
void uv__ordinate_setM(struct uv__ordinate *o, bool v);

#ifdef __cpluscplus
}
#endif

#endif