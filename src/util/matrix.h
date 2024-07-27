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

#include <stdbool.h>

#define MATRIX_INIT { 1, 0, 0, 1, 0, 0 }
/* Return matrix m11 element */
#define MATRIX_M11(m) ((m)->m_m11)
/* Return matrix m12 element */
#define MATRIX_M12(m) ((m)->m_m12)
/* Return matrix m21 element */
#define MATRIX_M21(m) ((m)->m_m21)
/* Return matrix m22 element */
#define MATRIX_M22(m) ((m)->m_m22)
/* Return matrix dx element */
#define MATRIX_DX(m) ((m)->m_dx)
/* Return matrix dy element */
#define MATRIX_DY(m) ((m)->m_dy)

/* Return matrix scale int x direction */
#define MATRIX_SCALE_X(m) ((m)->m_m11)
/* Return matrix scale int y direction */
#define MATRIX_SCALE_Y(m) ((m)->m_m22)
/* Return matrix rotate angle */
#define MATRIX_ROTATE_ANGLE(m) (asin ((m)->m_m12))
/* Return matrix offset x */
#define MATRIX_OFFSET_X(m) ((m)->m_dx)
/* Return matrix offset y */
#define MATRIX_OFFSET_Y(m) ((m)->m_dy)

/* Affine transformation matrix
 * m11 | m12 | 0
 * m21 | m22 | 0
 * dx  | dy  | 1
 * Typically, after initializing the matrix, the values of each element are
 * as follows [1 0 0] [0 1 0] [0 0 1] The basic formula of affine
 * transformation is nx = m11 * x + m21 * y + dx; ny = m12 * x + m22 * y +
 * dy;
 */
typedef struct se_matrix
{
  union
  {
    float m_m[6];
    struct
    {
      float m_m11, m_m12, m_m21, m_m22, m_dx, m_dy;
    };
  };
} matrix_t;

/* Matrix left multiplication */
void matrix_left_multiply (matrix_t *m, const matrix_t *mat);
/* Matrix (right) multiplication */
void matrix_multiply (matrix_t *m, const matrix_t *mat);
/* Matrix translation transformation */
void matrix_translate (matrix_t *m, const float x, const float y);
/* Matrix scaling transformation */
void matrix_scale (matrix_t *m, const float x, const float y);
/* Matrix rotation transformation */
void matrix_rotate (matrix_t *m, const float angle);
/* Matrix rotation transformation based on a certain center point */
void matrix_rotate2 (matrix_t *m, const float angle, const float x,
                     const float y);
/* matrix shear transformation */
void matrix_shear (matrix_t *m, const float x, const float y);
/* Matrix mirror transformation */
void matrix_mirror (matrix_t *m, const float x, const float y);
/* Matrix inversion matrix */
void matrix_invert (matrix_t *m);
/* Determine whether the matrix can be inverted */
bool matrix_invertible (const matrix_t *m);
/* Determine whether the matrix can be an identity matrix */
bool matrix_identity (const matrix_t *m);

/* Transform point coordinates using a matrix */
void matrix_trans_points (const matrix_t *m, double *points, int count);

#endif