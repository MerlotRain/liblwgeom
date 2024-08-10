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

#ifndef MATRIX_H
#define MATRIX_H

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MATRIX_INIT            {1, 0, 0, 1, 0, 0}
/* Return matrix m11 element */
#define MATRIX_M11(m)          ((m)->m_m11)
/* Return matrix m12 element */
#define MATRIX_M12(m)          ((m)->m_m12)
/* Return matrix m21 element */
#define MATRIX_M21(m)          ((m)->m_m21)
/* Return matrix m22 element */
#define MATRIX_M22(m)          ((m)->m_m22)
/* Return matrix dx element */
#define MATRIX_DX(m)           ((m)->m_dx)
/* Return matrix dy element */
#define MATRIX_DY(m)           ((m)->m_dy)

/* Return matrix scale int x direction */
#define MATRIX_SCALE_X(m)      ((m)->m_m11)
/* Return matrix scale int y direction */
#define MATRIX_SCALE_Y(m)      ((m)->m_m22)
/* Return matrix rotate angle */
#define MATRIX_ROTATE_ANGLE(m) (asin((m)->m_m12))
/* Return matrix offset x */
#define MATRIX_OFFSET_X(m)     ((m)->m_dx)
/* Return matrix offset y */
#define MATRIX_OFFSET_Y(m)     ((m)->m_dy)

/* Affine transformation matrix
 * m11 | m12 | 0
 * m21 | m22 | 0
 * dx  | dy  | 1
 * Typically, after initializing the matrix, the values of each element are
 * as follows [1 0 0] [0 1 0] [0 0 1] The basic formula of affine
 * transformation is nx = m11 * x + m21 * y + dx; ny = m12 * x + m22 * y +
 * dy;
 */
struct matrix {
    union {
        double m_m[6];
        struct {
            double m_m11, m_m12, m_m21, m_m22, m_dx, m_dy;
        };
    };
};

/* Matrix left multiplication */
EXTERN void mtx_left_multiply(struct matrix *m, const struct matrix *mat);
/* Matrix (right) multiplication */
EXTERN void mtx_multiply(struct matrix *m, const struct matrix *mat);
/* Matrix translation transformation */
EXTERN void mtx_translate(struct matrix *m, const double x, const double y);
/* Matrix scaling transformation */
EXTERN void mtx_scale(struct matrix *m, const double x, const double y);
/* Matrix rotation transformation */
EXTERN void mtx_rotate(struct matrix *m, const double angle);
/* Matrix rotation transformation based on a certain center point */
EXTERN void mtx_rotate2(struct matrix *m, const double angle, const double x,
                        const double y);
/* matrix shear transformation */
EXTERN void mtx_shear(struct matrix *m, const double x, const double y);
/* Matrix mirror transformation */
EXTERN void mtx_mirror(struct matrix *m, const double x, const double y);
/* Matrix inversion matrix */
EXTERN void mtx_invert(struct matrix *m);
/* Determine whether the matrix can be inverted */
EXTERN bool mtx_invertible(const struct matrix *m);
/* Determine whether the matrix can be an identity matrix */
EXTERN bool mtx_identity(const struct matrix *m);

#ifdef __cplusplus
}
#endif

#endif