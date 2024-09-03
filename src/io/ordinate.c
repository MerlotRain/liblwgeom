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

#include "ordinate.h"

struct uv__ordinate uv__ordinate_XY()
{
    struct uv__ordinate flag;
    flag.value = UV__ORDINATE_VALUE_X | UV__ORDINATE_VALUE_Y;
    flag.changeAllowed = NV_TRUE;
    return flag;
}

void uv__ordinate_setZ(struct uv__ordinate *o, int v)
{
    if ((o->value & UV__ORDINATE_VALUE_Z) != v) {
        if (o->changeAllowed) {
            o->value ^= UV__ORDINATE_VALUE_Z;
        }
    }
}

void uv__ordinate_setM(struct uv__ordinate *o, int v)
{
    if ((o->value & UV__ORDINATE_VALUE_M) != v) {
        if (o->changeAllowed) {
            o->value ^= UV__ORDINATE_VALUE_M;
        }
    }
}