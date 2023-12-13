#ifndef MATH_SE_H
#define MATH_SE_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */
/*                                geometry type                               */
/* -------------------------------------------------------------------------- */

#define GEOMETRY_TYPE_NONE 0       // 无效几何类型
#define GEOMETRY_TYPE_POINT 1      // 单点
#define GEOMETRY_TYPE_PATH 2       // 单段线
#define GEOMETRY_TYPE_POLYGON 3    // 多边形
#define GEOMETRY_TYPE_COLLECTION 4 // 集合
#define GEOMETRY_TYPE_MULTIPOINT 5 // 复合点、多点
#define GEOMETRY_TYPE_POLYLINE 6   // 多段折线
#define GEOMETRY_TYPE_ENVELOPE 7   // 矩形
#define GEOMETRY_TYPE_LINE 8       // 两点线段
#define GEOMETRY_TYPE_CIRCLEARC 9  // 三点圆弧
#define GEOMETRY_TYPE_RING 10      // 单圈


#ifdef __cplusplus
}
#endif

#endif // MATH_SE_H