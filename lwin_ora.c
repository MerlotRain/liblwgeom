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

#include "liblwgeom_internel.h"

typedef struct {
    int sdo_starting_offset;
    int sdo_etype;
    int sdo_interpretation;
} GEOM_SDO_ELEM_INFO;

LWGEOM *lwgeom_read_ora(const LWGEOM_SDO sdo, int flag)
{
    int dimension = 2 > (sdo.sdo_gtype / 1000) ? 2 : (sdo.sdo_gtype / 1000);

    int tt = sdo.sdo_gtype % 100;
    if (tt > 0 && tt < 4) {
        GEOM_SDO_ELEM_INFO *pElemInfo = (GEOM_SDO_ELEM_INFO *)sdo.sdo_elem_info;
        for (int i = 0; i < sdo.sdo_elem_count / 3; i++) {
        }
    }
    else if (tt > 4 && tt < 8) {
        // multi nv_geom
    }
    return NULL;
}
