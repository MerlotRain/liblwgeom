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

#ifndef MATRIX_H
#define MATRIX_H

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
void mtx_left_multiply(struct matrix *m, const struct matrix *mat);
/* Matrix (right) multiplication */
void mtx_multiply(struct matrix *m, const struct matrix *mat);
/* Matrix translation transformation */
void mtx_translate(struct matrix *m, const double x, const double y);
/* Matrix scaling transformation */
void mtx_scale(struct matrix *m, const double x, const double y);
/* Matrix rotation transformation */
void mtx_rotate(struct matrix *m, const double angle);
/* Matrix rotation transformation based on a certain center point */
void mtx_rotate2(struct matrix *m, const double angle, const double x,
                 const double y);
/* matrix shear transformation */
void mtx_shear(struct matrix *m, const double x, const double y);
/* Matrix mirror transformation */
void mtx_mirror(struct matrix *m, const double x, const double y);
/* Matrix inversion matrix */
void mtx_invert(struct matrix *m);
/* Determine whether the matrix can be inverted */
int mtx_invertible(const struct matrix *m);
/* Determine whether the matrix can be an identity matrix */
int mtx_identity(const struct matrix *m);

#endif