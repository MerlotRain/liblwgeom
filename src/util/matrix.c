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

#include "matrix.h"
#include <math.h>

void mtx_left_multiply(struct matrix *m, const struct matrix *mat)
{
    struct matrix mx = MATRIX_INIT;
    mx.m_m[0] = mat->m_m[0];
    mx.m_m[1] = mat->m_m[1];
    mx.m_m[2] = mat->m_m[2];
    mx.m_m[3] = mat->m_m[3];
    mx.m_m[4] = mat->m_m[4];
    mx.m_m[5] = mat->m_m[5];
    mtx_multiply(&mx, m);
    *m = mx;
}

void mtx_multiply(struct matrix *m, const struct matrix *mat)
{
    double mm[6] = {m->m_m[0], m->m_m[1], m->m_m[2],
                    m->m_m[3], m->m_m[4], m->m_m[5]};

    m->m_m11 = mm[0] * mat->m_m11 + mm[1] * mat->m_m21;
    m->m_m12 = mm[0] * mat->m_m12 + mm[1] * mat->m_m22;
    m->m_m21 = mm[2] * mat->m_m11 + mm[3] * mat->m_m21;
    m->m_m22 = mm[2] * mat->m_m12 + mm[3] * mat->m_m22;
    m->m_dx = mm[4] * mat->m_m11 + mm[5] * mat->m_m21 + mat->m_dx;
    m->m_dy = mm[4] * mat->m_m12 + mm[5] * mat->m_m22 + mat->m_dy;
}

void mtx_translate(struct matrix *m, const double x, const double y)
{
    m->m_dx += x;
    m->m_dy += y;
}

void mtx_scale(struct matrix *m, const double x, const double y)
{
    m->m_m11 *= x;
    m->m_m21 *= x;
    m->m_dx *= x;
    m->m_m12 *= y;
    m->m_m22 *= y;
    m->m_dy *= y;
}

void mtx_rotate(struct matrix *m, const double angle)
{
    mtx_rotate2(m, angle, 0, 0);
}

void mtx_rotate2(struct matrix *m, const double angle, const double x,
                 const double y)
{

    double ca = cos(angle);
    double sa = sin(angle);

    double mm[6] = {m->m_m[0], m->m_m[1], m->m_m[2],
                    m->m_m[3], m->m_m[4], m->m_m[5]};

    m->m_m11 = mm[0] * ca - mm[1] * sa;
    m->m_m12 = mm[0] * sa + mm[1] * ca;
    m->m_m21 = mm[2] * ca - mm[3] * sa;
    m->m_m22 = mm[2] * sa + mm[3] * ca;
    m->m_dx = mm[4] * ca - mm[5] * sa + x - x * ca + y * sa;
    m->m_dy = mm[4] * sa + mm[5] * ca + y - x * sa - y * ca;
}

void mtx_shear(struct matrix *m, const double x, const double y)
{
    double mm[6] = {m->m_m[0], m->m_m[1], m->m_m[2],
                    m->m_m[3], m->m_m[4], m->m_m[5]};

    m->m_m11 = mm[0] + mm[1] * x;
    m->m_m12 = mm[0] * y + mm[1];
    m->m_m21 = mm[2] + mm[3] * x;
    m->m_m22 = mm[2] * y + mm[3];
    m->m_dx = mm[4] + mm[5] * x;
    m->m_dy = mm[4] * y + mm[5];
}

void mtx_mirror(struct matrix *m, const double x, const double y)
{
    double mm[6] = {m->m_m[0], m->m_m[1], m->m_m[2],
                    m->m_m[3], m->m_m[4], m->m_m[5]};

    m->m_m11 = -mm[0];
    m->m_m22 = -mm[3];
    m->m_dx = mm[4] + 2 * x;
    m->m_dy = mm[5] + 2 * y;
}

void mtx_invert(struct matrix *m)
{
    if (!mtx_invertible(m))
        return;

    double mm[6] = {m->m_m[0], m->m_m[1], m->m_m[2],
                    m->m_m[3], m->m_m[4], m->m_m[5]};

    double D = mm[0] * mm[3] - mm[1] * mm[2];
    m->m_m11 = mm[3] / D;
    m->m_m12 = -mm[1] / D;
    m->m_m21 = -mm[2] / D;
    m->m_m22 = mm[0] / D;
    m->m_dx = (mm[2] * mm[5] - mm[3] * mm[4]) / D;
    m->m_dy = (mm[1] * mm[4] - mm[0] * mm[5]) / D;
}

bool mtx_invertible(const struct matrix *m)
{
    if (m->m_m[0] * m->m_m[3] - m->m_m[1] * m->m_m[2] != 0)
        return true;
    return false;
}

bool mtx_identity(const struct matrix *m)
{
    return m->m_m11 == 1 && m->m_m12 == 0 && m->m_m21 == 0 && m->m_m22 == 1 &&
           m->m_dx == 0 && m->m_dy == 0;
}