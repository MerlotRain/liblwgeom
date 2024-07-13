#ifndef __MATHSE_H__
#define __MATHSE_H__

enum GeometryType {
    /* Invalid geometry type */
    GEOMETRY_TYPE_NONE = -1,
    /// a point
    GEOMETRY_TYPE_POINT,
    /// a linestring
    GEOMETRY_TYPE_LINESTRING,
    /// a linear ring (linestring with 1st point == last point)
    GEOMETRY_TYPE_LINEARRING,
    /// a polygon
    GEOMETRY_TYPE_POLYGON,
    /// a collection of points
    GEOMETRY_TYPE_MULTIPOINT,
    /// a collection of linestrings
    GEOMETRY_TYPE_MULTI_LINESTRING,
    /// a collection of polygons
    GEOMETRY_TYPE_MULTI_POLYGON,
    /// a collection of heterogenous geometries
    GEOMETRY_TYPE_GEOMETRY_COLLECTION
};

#endif //__MATHSE_H__