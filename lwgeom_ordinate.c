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

#include "lwgeom_ordinate.h"
#include "liblwgeom_internel.h"

lwgeom_ordinate
lwgeom_ordinate_XY()
{
	lwgeom_ordinate flag;
	flag.value = LWORDINATE_VALUE_X | LWORDINATE_VALUE_Y;
	flag.changeAllowed = LW_TRUE;
	return flag;
}

void
lwgeom_ordinate_setZ(lwgeom_ordinate *o, int v)
{
	if ((o->value & LWORDINATE_VALUE_Z) != v)
	{
		if (o->changeAllowed)
		{
			o->value ^= LWORDINATE_VALUE_Z;
		}
	}
}

void
lwgeom_ordinate_setM(lwgeom_ordinate *o, int v)
{
	if ((o->value & LWORDINATE_VALUE_M) != v)
	{
		if (o->changeAllowed)
		{
			o->value ^= LWORDINATE_VALUE_M;
		}
	}
}