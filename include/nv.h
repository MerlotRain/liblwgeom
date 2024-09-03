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

#ifndef NV_H
#define NV_H

#ifdef __cpluscplus
extern "C" {
#endif

#include "nv/version.h"
#include "nv/logging.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#define NV_TRUE 1
#define NV_FALSE 0

#define NV_GEOM_POINT       0
#define NV_GEOM_LINE        1
#define NV_GEOM_POLY        2
#define NV_GEOM_COLLECTION  3

extern unsigned int nv_version(void);
extern const char *nv_version_string(void);

typedef void *(*nv_malloc_func)(size_t size);
typedef void *(*nv_realloc_func)(void *ptr, size_t size);
typedef void *(*nv_calloc_func)(size_t count, size_t size);
typedef void (*nv_free_func)(void *ptr);

extern int uv_replace_allocator(nv_malloc_func malloc_func,
                                   nv_realloc_func realloc_func,
                                   nv_calloc_func calloc_func,
                                   nv_free_func free_func);

extern int nv_error_code();
extern char *nv_error_messgae();


/* POINT2D */

struct nv_point2d {
    double x;
    double y;
};


extern double nv_angle(const struct nv_point2d p0);
extern double nv_angle2(const struct nv_point2d p0, const struct nv_point2d p1);
extern int nv_acute(const struct nv_point2d p0, const struct nv_point2d p1,
                        const struct nv_point2d p2);
extern int nv_obtuse(const struct nv_point2d p0, const struct nv_point2d p1,
                         const struct nv_point2d p2);
extern double nv_angle_between(const struct nv_point2d tip1,
                                  const struct nv_point2d tail,
                                  const struct nv_point2d tip2);
extern double nv_interior_angle(const struct nv_point2d p0,
                                   const struct nv_point2d p1,
                                   const struct nv_point2d p2);
extern int nv_angle_bisector(const struct nv_point2d A,
                                 const struct nv_point2d B,
                                 const struct nv_point2d C,
                                 const struct nv_point2d D, struct nv_point2d *p,
                                 double *angle);
extern double nv_dis_point_to_segment(const struct nv_point2d p,
                                         const struct nv_point2d A,
                                         const struct nv_point2d B);
extern double nv_dis_point_to_perpendicular(const struct nv_point2d p,
                                               const struct nv_point2d A,
                                               const struct nv_point2d B);
extern void
nv_segment_intersection(const struct nv_point2d p1, const struct nv_point2d p2,
                        const struct nv_point2d p3, const struct nv_point2d p4,
                        struct nv_point2d *pin, int *intersection);

/* ----------------------------------- Box ---------------------------------- */

struct nv_box {
    struct nv_point2d min;
    struct nv_point2d max;
};

extern int nv_box_intersects(const struct nv_box env1,
                                 const struct nv_box env2);
extern struct nv_box nv_box_intersection(const struct nv_box env1,
                                            const struct nv_box env2);
extern struct nv_box nv_box_union(const struct nv_box env1,
                                     const struct nv_box env2);
extern struct nv_geobject *nv_box_stroke(struct nv_box e, int gdim);

/* ----------------------------- geometry object ---------------------------- */

struct nv_geobject;

extern struct nv_geobject* nv_geo_point(const double* pp, int hasz, int hasm);
extern struct nv_geobject* nv_geo_line(uint32_t npoints, const double* points, int hasz, int hasm);
extern struct nv_geobject* nv_geo_poly(const struct nv_geobject* shell, uint32_t nholes, const struct nv_geobject** holes);
extern struct nv_geobject* nv_geo_create_mpoint(char hasz, char hasm);
extern struct nv_geobject* nv_geo_create_mline(char hasz, char hasm);
extern struct nv_geobject* nv_geo_create_mpoly(char hasz, char hasm);
extern struct nv_geobject* nv_geo_create_collection(uint8_t type, char hasz, char hasm);
extern struct nv_geobject* nv_geo_create_collection2(uint8_t type, uint32_t ngeoms, struct nv_geobject* geoms);

extern struct nv_geobject* nv_geo_mpoint_add_point(struct nv_geobject* mobj, struct nv_geobject* obj);
extern struct nv_geobject* nv_geo_mline_add_line(struct nv_geobject* mobj, struct nv_geobject* obj);
extern struct nv_geobject* nv_geo_mpoly_add_poly(struct nv_geobject* mobj, struct nv_geobject* obj);
extern struct nv_geobject* nv_geo_collection_add_geom(struct nv_geobject* mobj, struct nv_geobject* obj);

extern void nv_geo_free(struct nv_geobject *obj);
extern int nv_geo_dim_c(const struct nv_geobject *obj);
extern int nv_geo_dim_g(const struct nv_geobject *obj);
extern int nv_geo_sub_n(const struct nv_geobject *obj);
extern struct nv_geobject *nv_geo_sub_at(const struct nv_geobject *obj,
                                            int i);
extern int nv_geo_point_n(const struct nv_geobject *obj);

enum {
    NV_GEOMETRY_IO_WKT,     /* geometry IO type wkt */
    NV_GEOMETRY_IO_WKB,     /* geometry IO type wkb */
    NV_GEOMETRY_IO_WKB_HEX, /* geometry IO type wkb hex */
    NV_GEOMETRY_IO_GEOJSON, /* geometry IO type geojson */
    NV_GEOMETRY_IO_EWKT,    /* geometry IO type ewkt */
    NV_GEOMETRY_IO_EWKB,    /* geometry IO type ewkb */
    NV_GEOMETRY_IO_KML,     /* geometry IO type kml */
    NV_GEOMETRY_IO_GML2,    /* geometry IO type gml2 */
    NV_GEOMETRY_IO_GML3     /* geometry IO type gml3 */
};

struct nv_sdo_geometry {
    int sdo_gtype;
    int sdo_srid;
    int sdo_elem_count;
    int *sdo_elem_info;
    int sdo_ord_count;
    double *sdo_ordinates;
};

extern struct nv_geobject *nv_geo_read(int flag, const char *data,
                                          size_t len);
extern int nv_geo_write(int flag, const struct nv_geobject *obj, char **data,
                           size_t *len);
extern struct nv_geobject *nv_geo_read_ora(const struct nv_sdo_geometry sdo,
                                              int flag);
extern int nv_geo_write_ora(const struct nv_geobject *obj,
                               struct nv_sdo_geometry *sdo);

struct nv_i4;

struct nv_reader2;

extern struct nv_geobject *nv_i4_object(struct nv_i4 *i4);
extern void nv_i4_propProp(const struct nv_i4 *i4, int *propSize,
                              int **prop);
extern int nv_i4_prop_value(const struct nv_i4 *i4, size_t index);
extern struct nv_reader2 *nv_reader2_init(size_t size);
extern void nv_reader2_free(struct nv_reader2 *reader);
extern void nv_reader2_push(struct nv_reader2 *reader,
                               const struct nv_geobject *obj, int propSize,
                               int *prop);
extern struct nv_i4 *nv_reader2_iterator(struct nv_reader2 *writer);

/* --------------------------- geometry algorithm --------------------------- */

enum {

    /* Calculate the length of a geometry */
    NV_GEOMETRY_PROP_VALUE_LENGTH,
    /* Calculate the width of a geometry */
    NV_GEOMETRY_PROP_VALUE_WIDTH,
    /* Calculate the height of a geometry */
    NV_GEOMETRY_PROP_VALUE_HEIGHT,
    /* Calculate the area of a geometry */
    NV_GEOMETRY_PROP_VALUE_AREA,
};

enum {
    /* Get a clone of the geometry */
    NV_GEOMETRY_PROP_GEO_CLONE,
    /* Get the marker point of the geometry, which must be inside the geometry
     */
    NV_GEOMETRY_PROP_GEO_LABEL,
    /* Get the center point of the geometry */
    NV_GEOMETRY_PROP_GEO_CENTER,
    /* Get the centroid point of the geometry */
    NV_GEOMETRY_PROP_GEO_CENTROID,
    /* Get the envelope of the geometry */
    NV_GEOMETRY_PROP_GEO_ENVELOPE,
    /* Get the oriented minimum envelope of the geometry */
    NV_GEOMETRY_PROP_GEO_ORIENTED_ENVELOPE,
    /* Get the minimum envelope of the geometry */
    NV_GEOMETRY_PROP_GEO_ENVELOPE_CIRCLE,
    /* Get the inner minimum rectangle of the geometry */
    NV_GEOMETRY_PROP_GEO_INNER_RECT,
    /* Get the inner minimum square of the geometry */
    NV_GEOMETRY_PROP_GEO_INNER_SQUARE,
    /* Get the inner minimum circle of the geometry */
    NV_GEOMETRY_PROP_GEO_INNER_CIRCLE,
    /* Get the smallest convex polygon that contains all the points int the
     * geometry */
    NV_GEOMETRY_PROP_GEO_CONVEX_HULL,
    /* Get the simplified geometry */
    GEOMETRY_PROP_GEO_SIMPLIFY,
    /* Geometric simplification that preserves the original geometric topology
     */
    NV_GEOMETRY_PROP_GEO_TOPOLOGY_PRESERVING_SIMPLIFY,
    /* Get the boundary of the geometry */
    NV_GEOMETRY_PROP_GEO_BOUNDARY

};

enum {

    /* Delete all duplicate points, and pass in the duplicate point judgment
     * threshold (double) in para */
    NV_GEOMETRY_MODIFY_REMOVE_REPEAT,
    /* Douglas compression algorithm, para is the compression algorithm
     * threshold (double) */
    NV_GEOMETRY_MODIFY_DOUGLAS,
    /* Reverse the order of the points in the geometry, para is null */
    NV_GEOMETRY_MODIFY_REVERSE
};

enum {
    /* Computes the result of union two geometries */
    NV_GEOMETRY_COMBINE_UNION,
    /* Computes the result of the intersection of two geometries */
    NV_GEOMETRY_COMBINE_INTERSECT,
    /* Computes the result of the difference of two geometries*/
    NV_GEOMETRY_COMBINE_DIFFERENCE,
    /* Calculate the result of the difference between two geometric symmetries
     */
    NV_GEOMETRY_COMBINE_SYM_DIFFERENCE

};

enum {

    /* Two geometries do not fit this relationship */
    NV_GEOMETRY_RELATION_RESULT_UNFIT = -1,
    /* The two geometries do not satisfy this relationship */
    NV_GEOMETRY_RELATION_RESULT_FALSE,
    /* Two geometries satisfy this relationship */
    NV_GEOMETRY_RELATION_RESULT_TRUE
};

enum {
    /* Two geometries is disjoint each other */
    NV_GEOMETRY_SPR_IS_DISJOINT,
    /* Two geometries is intersect each other */
    NV_GEOMETRY_SPR_IS_INTERSECT,
    /* Two geometries is contain each other */
    NV_GEOMETRY_SPR_IS_CONTAIN,
    /* Two geometries is cross each other */
    NV_GEOMETRY_SPR_IS_CROSS,
    /* Two geometries is equal */
    NV_GEOMETRY_SPR_IS_EQUAL,
    /* Two geometries is touch each other */
    NV_GEOMETRY_SPR_IS_TOUCH,
    /* Two geometries is overlap each other */
    NV_GEOMETRY_SPR_IS_OVERLAP,
    /* Two geometries is within each other */
    NV_GEOMETRY_SPR_IS_WITHIN
};

enum {
    /* Check geometry is simple */
    NV_GEOMETRY_CHECK_1_SIMPLE,
    /* Check geometry is close */
    NV_GEOMETRY_CHECK_1_CLOSE,
    /* Check geometry is good direction */
    NV_GEOMETRY_CHECK_1_GOOD_DIRECTION,
    /* Check geometry is self cross */
    NV_GEOMETRY_CHECK_1_SELF_CROSS,
    /* Check geometry has repeat point */
    NV_GEOMETRY_CHECK_1_REPEAT,
    /* Check line is lap */
    NV_GEOMETRY_CHECK_1_LAP
};

enum {
    /* Check geometry repeat points */
    NV_GEOMETRY_CHECK_2_REPEAT_POINT = 0x01,
    /* Check geometry isolate points */
    NV_GEOMETRY_CHECK_2_ISOLATE_POINT = 0x02,
    /* Check geometry pseudo endpoints */
    NV_GEOMETRY_CHECK_2_PSEUDO_ENDPOINT = 0x04
};

extern double nv_tolerance(double tol);
extern double nv_prop_value(const struct nv_geobject *obj, int mode);
extern struct nv_geobject *nv_prop_geo(const struct nv_geobject *obj,
                                          int mode);

extern void nv_prop_geo2(const struct nv_geobject *obj, int mode,
                            double *paras);
extern int nv_left_right(const struct nv_geobject *obj, double *xy);
extern int nv_ccw(const double *pp, int npoints, int cdim);
extern void nv_vertex_convex(const struct nv_geobject *obj, int index,
                                int *convex);
extern void nv_building_regularization(double *xy, int np);
extern void nv_kmeans(struct nv_reader2 *P, int n, struct nv_reader2 **W);

/* ---------------------------------- RTree --------------------------------- */

extern struct nv_rtree *nv_rtree_new(void);
extern void nv_rtree_free(struct nv_rtree *tr);
extern struct nv_rtree *nv_rtree_clone(struct nv_rtree *tr);
extern void nv_rtree_set_item_callbacks(
    struct nv_rtree *tr,
    int (*clone)(const void *item, void **into, void *udata),
    void (*free)(const void *item, void *udata));
extern void nv_rtree_set_udata(struct nv_rtree *tr, void *udata);
extern int nv_rtree_insert(struct nv_rtree *tr, const double *min,
                               const double *max, const void *data);
extern void
nv_rtree_search(const struct nv_rtree *tr, const double *min, const double *max,
                int (*iter)(const double *min, const double *max,
                             const void *data, void *udata),
                void *udata);
extern void nv_rtree_scan(const struct nv_rtree *tr,
                             int (*iter)(const double *min, const double *max,
                                          const void *data, void *udata),
                             void *udata);
extern size_t nv_rtree_count(const struct nv_rtree *tr);
extern int nv_rtree_delete(struct nv_rtree *tr, const double *min,
                               const double *max, const void *data);
extern int nv_rtree_delete_with_comparator(
    struct nv_rtree *tr, const double *min, const double *max, const void *data,
    int (*compare)(const void *a, const void *b, void *udata), void *udata);
extern void nv_rtree_opt_relaxed_atomics(struct nv_rtree *tr);

/* ---------------------------------- NMEA ---------------------------------- */

#define NMEA_MAXSAT    (12)
#define NMEA_SATINPACK (4)
#define NMEA_NSATPACKS (NMEA_MAXSAT / NMEA_SATINPACK)
/**
 * Information about satellite
 * \see nmeaSATINFO
 * \see nmeaGPGSV
 */
struct nmeaSATELLITE {
    int id;      ///< Satellite PRN number
    int in_use;  ///< Used in position fix
    int elv;     ///< Elevation in degrees, 90 maximum
    int azimuth; ///< Azimuth, degrees from true north, 000 to 359
    int sig;     ///< Signal, 00-99 dB
};
struct nmeaTIME {
    int year; ///< Years since 1900
    int mon;  ///< Months since January - [0,11]
    int day;  ///< Day of the month - [1,31]
    int hour; ///< Hours since midnight - [0,23]
    int min;  ///< Minutes after the hour - [0,59]
    int sec;  ///< Seconds after the minute - [0,59]
    int msec; ///< Thousandths part of second - [0,999]
};
/**
 * \brief Get time now to nmeaTIME structure
 */
void nmea_time_now(struct nmeaTIME *t);
/**
 * NMEA packets type which parsed and generated by library
 */
enum nmeaPACKTYPE {
    GPNON = 0x0000, //!< Unknown packet type.
    GPGGA = 0x0001, //!< GGA - Essential fix data which provide 3D location and
                    //!< accuracy data.
    GPGSA = 0x0002, //!< GSA - GPS receiver operating mode, SVs used for
                    //!< navigation, and DOP values.
    GPGSV = 0x0004, //!< GSV - Number of SVs in view, PRN numbers, elevation,
                    //!< azimuth & SNR values.
    GPRMC = 0x0008, //!< RMC - Recommended Minimum Specific GPS/TRANSIT Data.
    GPVTG = 0x0010, //!< VTG - Actual track made good and speed over ground.
    GPGST = 0x0012, //!< GST - GPS Pseudorange Noise Statistics
    HCHDG = 0x0020, //!< HDG - Heading, Deviation and Variation
    HCHDT = 0x0100, //!< HDT - Heading reference to true north
};

/**
 * GGA packet information structure (Global Positioning System Fix Data)
 */
struct nmeaGPGGA {
    struct nmeaTIME utc; //!< UTC of position (just time)
    double lat;          //!< Latitude in NDEG - [degree][min].[sec/60]
    char ns;             //!< [N]orth or [S]outh
    double lon;          //!< Longitude in NDEG - [degree][min].[sec/60]
    char ew;             //!< [E]ast or [W]est
    int sig; //!< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential,
             //!< 3 = Sensitive)
    int satinuse;   //!< Number of satellites in use (not those in view)
    double HDOP;    //!< Horizontal dilution of precision
    double elv;     //!< Antenna altitude above/below mean sea level (geoid)
    char elv_units; //!< [M]eters (Antenna height unit)
    double diff; //!< Geoidal separation (Diff. between WGS-84 earth ellipsoid
                 //!< and mean sea level. '-' = geoid is below WGS-84 ellipsoid)
    char diff_units; //!< [M]eters (Units of geoidal separation)
    double dgps_age; //!< Time in seconds since last DGPS update
    int dgps_sid;    //!< DGPS station ID number
};
/**
 * GST packet information structure (GPS Pseudorange Noise Statistics)
 */
struct nmeaGPGST {
    struct nmeaTIME utc; //!< UTC of position fix
    double rms_pr; //!< RMS value of the pseudorange residuals; Includes carrier
                   //!< phase residuals during periods of RTK (float) and RTK
                   //!< (fixed) processing
    double
        err_major; //!< Error ellipse semi-major axis 1 sigma error, in meters
    double
        err_minor;  //!< Error ellipse semi-minor axis 1 sigma error, in meters
    double err_ori; //!< Error ellipse orientation, degrees from true north
    double sig_lat; //!< Latitude 1 sigma error, in meters
    double sig_lon; //!< Longitude 1 sigma error, in meters
    double sig_alt; //!< Height 1 sigma error, in meters
};
/**
 * GSA packet information structure (Satellite status)
 */
struct nmeaGPGSA {
    char fix_mode; //!< Mode (M = Manual, forced to operate in 2D or 3D; A =
                   //!< Automatic, 3D/2D)
    int fix_type; //!< Type, used for navigation (1 = Fix not available; 2 = 2D;
                  //!< 3 = 3D)
    int sat_prn[NMEA_MAXSAT]; //!< PRNs of satellites used in position fix (null
                              //!< for unused fields)
    double PDOP;              //!< Dilution of precision
    double HDOP;              //!< Horizontal dilution of precision
    double VDOP;              //!< Vertical dilution of precision
    char pack_type;           //!< P=GPS, N=generic, L=GLONASS
} nmeaGPGSA;
/**
 * GSV packet information structure (Satellites in view)
 */
struct nmeaGPGSV {
    int pack_count; ///< Total number of messages of this type in this cycle
    int pack_index; ///< Message number
    int sat_count;  ///< Total number of satellites in view
    char pack_type; ///< P=GPS - S=SBas - N=generic - L=GLONAS - A=GALILEO -
                    ///< B=BEIDOU - Q=QZSS
    struct nmeaSATELLITE sat_data[NMEA_SATINPACK];
};
/**
 * RMC packet information structure (Recommended Minimum sentence C)
 */
struct nmeaGPRMC {
    struct nmeaTIME utc; ///< UTC of position
    char status;         //!< Status (A = active or V = void)
    double lat;          //!< Latitude in NDEG - [degree][min].[sec/60]
    char ns;             //!< [N]orth or [S]outh
    double lon;          //!< Longitude in NDEG - [degree][min].[sec/60]
    char ew;             //!< [E]ast or [W]est
    double speed;        //!< Speed over the ground in knots
    double direction;    //!< Track angle in degrees True
    double declination; //!< Magnetic variation degrees (Easterly var. subtracts
                        //!< from true course)
    char declin_ew;     //!< [E]ast or [W]est
    char mode;          //!< Mode indicator of fix type (A = autonomous, D =
               //!< differential, E = estimated, N = not valid, S = simulator)
    char navstatus; //!< NMEA v4.1 - Navigation Status type (S = Safe, C =
                    //!< Caution, U = Unsafe, V = Navigational status not valid)
};
/**
 * VTG packet information structure (Track made good and ground speed)
 */
struct nmeaGPVTG {
    double dir; //!< True track made good (degrees)
    char dir_t; //!< Fixed text 'T' indicates that track made good is relative
                //!< to true north
    double dec; //!< Magnetic track made good
    char dec_m; //!< Fixed text 'M'
    double spn; //!< Ground speed, knots
    char spn_n; //!< Fixed text 'N' indicates that speed over ground is in knots
    double spk; //!< Ground speed, kilometers per hour
    char spk_k; //!< Fixed text 'K' indicates that speed over ground is in
                //!< kilometers/hour
};
/**
 * HDT packet information structure (Heading from True North)
 */
struct nmeaGPHDT {
    double heading; //!< Heading in degrees
    char t_flag; //!< Fixed text 'T' indicates that heading is relative to true
                 //!< north
};
/**
 * HCHDG packet information structure (magnetic heading)
 */
struct nmeaHCHDG {
    double mag_heading;   ///< Magnetic sensor heading (degrees)
    double mag_deviation; ///< Magnetic deviation (degrees)
    char ew_deviation;    ///< [E]ast or [W]est
    double mag_variation; ///< Magnetic variation (degrees)
    char ew_variation;    ///< [E]ast or [W]est
};
/**
 * HDT packet information structure (Heading, )
 */
struct nmeaHCHDT {
    double direction; ///< Heading respect to true north (degrees)
    char t_flag;      ///< Static text [T]
};

extern void nmea_zero_GPGGA(struct nmeaGPGGA *pack);
extern void nmea_zero_GPGST(struct nmeaGPGST *pack);
extern void nmea_zero_GPGSA(struct nmeaGPGSA *pack);
extern void nmea_zero_GPGSV(struct nmeaGPGSV *pack);
extern void nmea_zero_GPRMC(struct nmeaGPRMC *pack);
extern void nmea_zero_GPVTG(struct nmeaGPVTG *pack);

/**
 * Position data in fractional degrees or radians
 */
struct nmeaPOS {
    double lat; ///< Latitude
    double lon; ///< Longitude
};

/**
 * Information about all satellites in view
 * \see nmeaINFO
 * \see nmeaGPGSV
 */
struct nmeaSATINFO {
    int inuse;  ///< Number of satellites in use (not those in view)
    int inview; ///< Total number of satellites in view
    struct nmeaSATELLITE sat[NMEA_MAXSAT]; ///< Satellites information
};
/**
 * Summary GPS information from all parsed packets,
 * used also for generating NMEA stream
 * \see nmea_parse
 * \see nmea_GPGGA2info, nmea_...2info
 */
struct nmeaINFO {

    int smask; //!< Mask specifying types of packages from which data have been
               //!< obtained

    struct nmeaTIME utc; //!< UTC of position

    int sig; //!< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential,
             //!< 3 = Sensitive)
    int fix; //!< Operating mode, used for navigation (1 = Fix not available; 2
             //!< = 2D; 3 = 3D)

    double PDOP; //!< Position Dilution Of Precision
    double HDOP; //!< Horizontal Dilution Of Precision
    double VDOP; //!< Vertical Dilution Of Precision

    double lat; //!< Latitude in NDEG - +/-[degree][min].[sec/60]
    double lon; //!< Longitude in NDEG - +/-[degree][min].[sec/60]
    double
        elv; //!< Antenna altitude above/below mean sea level (geoid) in meters
    double speed;       //!< Speed over the ground in kilometers/hour
    double direction;   //!< Track angle in degrees True
    double declination; //!< Magnetic variation degrees (Easterly var. subtracts
                        //!< from true course)
    double rms_pr; //!< RMS value of the pseudorange residuals; includes carrier
                   //!< phase residuals during periods of RTK (float) and RTK
                   //!< (fixed) processing
    double
        err_major; //!< Error ellipse semi-major axis 1 sigma error, in meters
    double
        err_minor;  //!< Error ellipse semi-minor axis 1 sigma error, in meters
    double err_ori; //!< Error ellipse orientation, degrees from true north
    double sig_lat; //!< Latitude 1 sigma error, in meters
    double sig_lon; //!< Longitude 1 sigma error, in meters
    double sig_alt; //!< Height 1 sigma error, in meters

    struct nmeaSATINFO satinfo; //!< Satellites information
};

void nmea_zero_INFO(struct nmeaINFO *info);

extern int nmea_pack_type(const char *buff, int buff_sz);
extern int nmea_find_tail(const char *buff, int buff_sz, int *res_crc);

extern int nmea_parse_GPGGA(const char *buff, int buff_sz,
                               struct nmeaGPGGA *pack);
extern int nmea_parse_GPGSA(const char *buff, int buff_sz,
                               struct nmeaGPGSA *pack);
extern int nmea_parse_GPGSV(const char *buff, int buff_sz,
                               struct nmeaGPGSV *pack);
extern int nmea_parse_GPRMC(const char *buff, int buff_sz,
                               struct nmeaGPRMC *pack);
extern int nmea_parse_GPVTG(const char *buff, int buff_sz,
                               struct nmeaGPVTG *pack);
extern int nmea_parse_HCHDG(const char *buff, int buff_sz,
                               struct nmeaHCHDG *pack);
extern int nmea_parse_HCHDT(const char *buff, int buff_sz,
                               struct nmeaHCHDT *pack);
extern int nmea_parse_GPGST(const char *buff, int buff_sz,
                               struct nmeaGPGST *pack);
extern int nmea_parse_GPHDT(const char *buff, int buff_sz,
                               struct nmeaGPHDT *pack);

extern void nmea_GPGGA2info(struct nmeaGPGGA *pack, struct nmeaINFO *info);
extern void nmea_GPGST2info(struct nmeaGPGST *pack, struct nmeaINFO *info);
extern void nmea_GPGSA2info(struct nmeaGPGSA *pack, struct nmeaINFO *info);
extern void nmea_GPGSV2info(struct nmeaGPGSV *pack, struct nmeaINFO *info);
extern void nmea_GPRMC2info(struct nmeaGPRMC *pack, struct nmeaINFO *info);
extern void nmea_GPVTG2info(struct nmeaGPVTG *pack, struct nmeaINFO *info);

/*
 * degree VS radian
 */
extern double nmea_degree2radian(double val);
extern double nmea_radian2degree(double val);

/*
 * NDEG (NMEA degree)
 */
extern double nmea_ndeg2degree(double val);
extern double nmea_degree2ndeg(double val);

extern double nmea_ndeg2radian(double val);
extern double nmea_radian2ndeg(double val);

/*
 * DOP
 */
extern double nmea_calc_pdop(double hdop, double vdop);
extern double nmea_dop2meters(double dop);
extern double nmea_meters2dop(double meters);

/*
 * positions work
 */
extern void nmea_info2pos(const struct nmeaINFO *info, struct nmeaPOS *pos);
extern void nmea_pos2info(const struct nmeaPOS *pos, struct nmeaINFO *info);

extern double nmea_distance(const struct nmeaPOS *from_pos,
                               const struct nmeaPOS *to_pos);

extern double nmea_distance_ellipsoid(const struct nmeaPOS *from_pos,
                                         const struct nmeaPOS *to_pos,
                                         double *from_azimuth,
                                         double *to_azimuth);

extern int nmea_move_horz(const struct nmeaPOS *start_pos,
                             struct nmeaPOS *end_pos, double azimuth,
                             double distance);

extern int nmea_move_horz_ellipsoid(const struct nmeaPOS *start_pos,
                                       struct nmeaPOS *end_pos, double azimuth,
                                       double distance, double *end_azimuth);

/* Direction north */
#define GEOHASH_DIRECTION_NORTH      0
/* Direction east */
#define GEOHASH_DIRECTION_EAST       1
/* Direction west */
#define GEOHASH_DIRECTION_WEST       2
/* Direction south */
#define GEOHASH_DIRECTION_SOUTH      3
/* Direction south west */
#define GEOHASH_DIRECTION_SOUTH_WEST 4
/* Direction south east */
#define GEOHASH_DIRECTION_SOUTH_EAST 5
/* Direction north west */
#define GEOHASH_DIRECTION_NORTH_WEST 6
/* Direction north east */
#define GEOHASH_DIRECTION_NORTH_EAST 7

typedef struct geohash_bits {
    uint64_t bits;
    uint8_t step;
} geohash_bits;

typedef struct geohash_neighbors {
    geohash_bits north;
    geohash_bits east;
    geohash_bits west;
    geohash_bits south;
    geohash_bits south_west;
    geohash_bits south_east;
    geohash_bits north_west;
    geohash_bits north_east;
} geohash_neighbors;

typedef struct geohash_range {
    double max;
    double min;
} geohash_range;

typedef struct geohash_area {
    geohash_bits hash;
    geohash_range longitude;
    geohash_range latitude;
} geohash_area;

extern void get_geohash_range(geohash_range *long_range,
                                 geohash_range *lat_range);
extern int geohash_encode(const geohash_range *long_range,
                             const geohash_range *lat_range, double longitude,
                             double latitude, uint8_t step, geohash_bits *hash);
extern int geohash_encode_type(double longitude, double latitude,
                                  uint8_t step, geohash_bits *hash);
extern int geohash_encode_WGS84(double longitude, double latitude,
                                   uint8_t step, geohash_bits *hash);

extern int geohash_decode(const geohash_range long_range,
                             const geohash_range lat_range,
                             const geohash_bits hash, geohash_area *area);
extern int geohash_decode_type(const geohash_bits hash, geohash_area *area);
extern int geohash_decode_WGS84(const geohash_bits hash, geohash_area *area);
extern int geohash_decode_area_longlat(const geohash_area *area, double *xy);
extern int geohash_decode_longlat_type(const geohash_bits hash, double *xy);
extern int geohash_decode_longlat_WGS84(const geohash_bits hash, double *xy);

extern void geohash_query_neighbors(const geohash_bits *hash,
                                       geohash_neighbors *neighbors);

struct mm_texture {
    char *name;
    char *path;
};

struct mm_material {
    /* Material name */
    char *name;

    /* Parameters */
    float Ka[3]; /* Ambient */
    float Kd[3]; /* Diffuse */
    float Ks[3]; /* Specular */
    float Ke[3]; /* Emission */
    float Kt[3]; /* Transmittance */
    float Ns;    /* Shininess */
    float Ni;    /* Index of refraction */
    float Tf[3]; /* Transmission filter */
    float d;     /* Disolve (alpha) */
    int illum;   /* Illumination model */

    /* Set for materials that don't come from the associated mtllib */
    int fallback;

    /* Texture map indices in fastObjMesh textures array */
    unsigned int map_Ka;
    unsigned int map_Kd;
    unsigned int map_Ks;
    unsigned int map_Ke;
    unsigned int map_Kt;
    unsigned int map_Ns;
    unsigned int map_Ni;
    unsigned int map_d;
    unsigned int map_bump;
};




/* ---------------------------- Ellipse & Cricle ---------------------------- */

/// nv_ ellipse is used to describe an ellipse or circle.
/// Before V1.0, it would be treated as a regular geometric shape and
/// temporarily not included in the unified management of the nv_geom model,
/// while providing relevant algorithms for circles or ellipses.
struct nv_ellipse {
    struct nv_point2d center;
    double major;
    double minor;
    double azimuth;
};

enum {

    /// The eccentricity of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_ECCENTRICITY = 0,
    /// The area of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_AREA = 1,
    /// The perimeter of the ellipse. - double
    NV_ELLIPSE_PROP_VALUE_PERIMETER = 2,
    /// Two foci of the ellipse. The axes are oriented by the azimuth and are on
    /// the semi-major axis. - nv_point[2]
    NV_ELLIPSE_PROP_VALUE_FOCI = 4,
    /// The distance between the center and each foci. - double
    NV_ELLIPSE_PROP_FOCUS_DISTANCE = 8
};

enum {
    /// Two points form a circle, and the line segment between these two points
    /// is the diameter of the circle
    NV_CONSTRUCT_CIRCLE_2P,
    /// Three points form a circle, and these three points are on the circle
    NV_CONSTRUCT_CIRCLE_3P,
    /// To construct a circle with three tangent lines, six points need to be
    /// passed in. These six points form three straight lines, which can
    /// generate 0-2 circles. They are also the inscribed circles of a triangle
    NV_CONSTRUCT_CIRCLE_ICT
};

extern void nv_construct_circle(const struct nv_point2d *p, int t,
                                   struct nv_ellipse *es, int *n);
extern void nv_ellipse_prop_value(const struct nv_ellipse ell, int flags,
                                     double *values);
extern struct nv_geobject *nv_ellipse_stroke(struct nv_ellipse e,
                                                uint32_t param);

/* ----------------------------------- Arc ---------------------------------- */

struct nv_arc {
    struct nv_point2d start;
    struct nv_point2d along;
    struct nv_point2d end;
};

extern struct nv_geobject *nv_arc_stroke(struct nv_arc arc,
                                            double maxAngleStepSizeDegress);

#ifdef __cpluscplus
}
#endif

#endif