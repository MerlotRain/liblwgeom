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

#ifndef LIBLWGEOM_INTERNEL_H
#define LIBLWGEOM_INTERNEL_H

#include "liblwgeom.h"

#include "lwgeom_log.h"

/**
 * Floating point comparators.
 */
#define LWMAX(a, b) ((a) > (b) ? (a) : (b))
#define LWMIN(a, b) ((a) < (b) ? (a) : (b))

#define FP_IS_ZERO(A)               (fabs(A) <= FP_TOLERANCE)
#define FP_MAX(A, B)                (((A) > (B)) ? (A) : (B))
#define FP_MIN(A, B)                (((A) < (B)) ? (A) : (B))
#define FP_ABS(a)                   ((a) < (0) ? -(a) : (a))
#define FP_EQUALS(A, B)             (fabs((A) - (B)) <= FP_TOLERANCE)
#define FP_NEQUALS(A, B)            (fabs((A) - (B)) > FP_TOLERANCE)
#define FP_LT(A, B)                 (((A) + FP_TOLERANCE) < (B))
#define FP_LTEQ(A, B)               (((A) - FP_TOLERANCE) <= (B))
#define FP_GT(A, B)                 (((A) - FP_TOLERANCE) > (B))
#define FP_GTEQ(A, B)               (((A) + FP_TOLERANCE) >= (B))
#define FP_CONTAINS_TOP(A, X, B)    (FP_LT(A, X) && FP_LTEQ(X, B))
#define FP_CONTAINS_BOTTOM(A, X, B) (FP_LTEQ(A, X) && FP_LT(X, B))
#define FP_CONTAINS_INCL(A, X, B)   (FP_LTEQ(A, X) && FP_LTEQ(X, B))
#define FP_CONTAINS_EXCL(A, X, B)   (FP_LT(A, X) && FP_LT(X, B))
#define FP_CONTAINS(A, X, B)        FP_CONTAINS_EXCL(A, X, B)

#define STR_EQUALS(A, B)  strcmp((A), (B)) == 0
#define STR_IEQUALS(A, B) (strcasecmp((A), (B)) == 0)
#define STR_ISTARTS(A, B) (strncasecmp((A), (B), strlen((B))) == 0)

/*
 * this will change to NaN when I figure out how to
 * get NaN in a platform-independent way
 */
#define NO_VALUE   0.0
#define NO_Z_VALUE NO_VALUE
#define NO_M_VALUE NO_VALUE

// nv-util callback function
typedef void (*DestoryFunc)(void *);
typedef void (*EqualFunc)(const void *, const void *);
typedef void (*HashFunc)(const void *);

// default hash function
size_t lw_str_hash(const void *str);

size_t lw_nearest_pow(size_t v);

int lwbox_intersects(const LWBOX env1, const LWBOX env2);
LWBOX lwbox_intersection(const LWBOX env1, const LWBOX env2);
LWBOX lwbox_union(const LWBOX env1, const LWBOX env2);
LWGEOM *lwbox_stroke(LWBOX e, int gdim);

double lwpoint_angle(const POINT2D p0);
double lwpoint_angle2(const POINT2D p0, const POINT2D p1);
int lwpoint_acute(const POINT2D p0, const POINT2D p1, const POINT2D p2);
int lwpoint_obtuse(const POINT2D p0, const POINT2D p1, const POINT2D p2);
double lwpoint_angle_between(const POINT2D tip1, const POINT2D tail, const POINT2D tip2);
double lwpoint_interior_angle(const POINT2D p0, const POINT2D p1, const POINT2D p2);
int
lwpoint_angle_bisector(const POINT2D A, const POINT2D B, const POINT2D C, const POINT2D D, POINT2D *p, double *angle);
double lwsegment_dis_point_to_segment(const POINT2D p, const POINT2D A, const POINT2D B);
double lwsegment_dis_point_to_perpendicular(const POINT2D p, const POINT2D A, const POINT2D B);
void lwsegment_intersection(const POINT2D p1,
			    const POINT2D p2,
			    const POINT2D p3,
			    const POINT2D p4,
			    POINT2D *pin,
			    int *intersection);

int lw_segment_side(const POINT2D *p1, const POINT2D *p2, const POINT2D *q);
int lw_arc_side(const POINT2D *A1, const POINT2D *A2, const POINT2D *A3, const POINT2D *Q);
int lw_arc_calculate_gbox_cartesian_2d(const POINT2D *A1, const POINT2D *A2, const POINT2D *A3, LWBOX *gbox);
double lw_arc_center(const POINT2D *p1, const POINT2D *p2, const POINT2D *p3, POINT2D *result);
int lw_pt_in_seg(const POINT2D *P, const POINT2D *A1, const POINT2D *A2);
int lw_pt_in_arc(const POINT2D *P, const POINT2D *A1, const POINT2D *A2, const POINT2D *A3);
int lw_arc_is_pt(const POINT2D *A1, const POINT2D *A2, const POINT2D *A3);
double lw_seg_length(const POINT2D *A1, const POINT2D *A2);
double lw_arc_length(const POINT2D *A1, const POINT2D *A2, const POINT2D *A3);

#endif /* LIBLWGEOM_INTERNEL_H */