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

#ifndef LWGEOM_ORDINATE_H
#define LWGEOM_ORDINATE_H

#include <stdint.h>

enum {
    LWORDINATE_VALUE_X = 1,
    LWORDINATE_VALUE_Y = 2,
    LWORDINATE_VALUE_Z = 4,
    LWORDINATE_VALUE_M = 8
};

typedef struct {
    uint8_t value;
    int changeAllowed; ///< allow change
} lwgeom_ordinate;

lwgeom_ordinate lwgeom_ordinate_XY();
void lwgeom_ordinate_setZ(lwgeom_ordinate *o, int v);
void lwgeom_ordinate_setM(lwgeom_ordinate *o, int v);

#endif /* LWGEOM_ORDINATE_H */