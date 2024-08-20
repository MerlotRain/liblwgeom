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

#include "mg.h"
#include "mgp.h"

/* ---------------------------- geometry factory ---------------------------- */

struct mg_object *mg_create_single(int gdim, int pn, int cdim, const double *pp,
                                   int flag)
{
    assert(pp);
    struct mg_object *obj =
        (struct mg_object *)malloc(sizeof(struct mg_object));
    if (obj == NULL)
        return NULL;
    memset(obj, 0, sizeof(struct mg_object));

    obj->ngeoms = 1;
    obj->gdim = gdim;
    obj->cdim = cdim;
    obj->npoints = pn;
    obj->flag = flag;
    if (flag == 0) {
        obj->pp = (double *)pp;
    }
    else {
        obj->pp = (double *)malloc(sizeof(double) * pn * cdim);
        if (obj->pp == NULL) {
            mg_free_object(obj);
            return NULL;
        }
        memcpy(obj->pp, pp, sizeof(double) * pn * cdim);
    }
    return obj;
}

struct mg_object *mg_create_multi(int gdim, int snum, struct mg_object **subs)
{
    assert(subs);
    struct mg_object *obj =
        (struct mg_object *)malloc(sizeof(struct mg_object));
    if (obj == NULL)
        return NULL;
    memset(obj, 0, sizeof(struct mg_object));

    obj->gdim = gdim;
    for (int i = 0; i < snum; ++i) {
        struct mg_object *sub = subs[i];
        if (sub == NULL)
            continue;

        // single sub
        if (sub->ngeoms == 1) {
            if (sub->gdim == obj->gdim) {
                obj->ngeoms++;

                struct mg_object **tobjs = (struct mg_object **)realloc(
                    obj->objects, obj->ngeoms * sizeof(struct mg_object *));
                if (tobjs == NULL) {
                    mg_free_object(obj);
                    return NULL;
                }
                obj->objects = tobjs;

                if (obj->objects == NULL) {
                    mg_free_object(obj);
                    return NULL;
                }
                obj->objects[obj->ngeoms - 1] = sub;
            }
        }
        // multi sub
        else {
            for (int j = 0; j < sub->ngeoms; ++j) {
                struct mg_object *ssub = sub->objects[j];
                if (ssub == NULL)
                    continue;
                if (ssub->gdim == obj->gdim) {
                    obj->ngeoms++;

                    struct mg_object **tobjs = (struct mg_object **)realloc(
                        obj->objects, obj->ngeoms * sizeof(struct mg_object *));
                    if (tobjs == NULL) {
                        mg_free_object(obj);
                        return NULL;
                    }
                    obj->objects = tobjs;

                    if (obj->objects == NULL) {
                        mg_free_object(obj);
                        return NULL;
                    }
                    obj->objects[obj->ngeoms - 1] = ssub;
                }
            }
        }
    }

    if (obj->ngeoms == 1) {
        // degenerate
        obj->flag = obj->objects[0]->flag;
        obj->npoints = obj->objects[0]->npoints;
        obj->pp = obj->objects[0]->pp;
        free(obj->objects[0]);
        free(obj->objects);
        obj->objects = NULL;
    }
    return obj;
}

void pri_mg_free_object(struct mg_object *g)
{
    assert(g);
    if (g->flag != 0) {
        free(g->pp);
    }
    free(g);
}

void mg_free_object(struct mg_object *g)
{
    assert(g);
    if (g->ngeoms == 1) {
        pri_mg_free_object(g);
    }
    else {
        for (int i = 0; i < g->ngeoms; ++i) {
            struct mg_object *sub = g->objects[i];
            if (sub == NULL)
                continue;
            pri_mg_free_object(sub);
        }
    }
}

int mg_dim_c(const struct mg_object *obj)
{
    assert(obj);
    return obj->cdim;
}

int mg_dim_g(const struct mg_object *obj)
{
    assert(obj);
    return obj->gdim;
}

int mg_sub_n(const struct mg_object *obj)
{
    assert(obj);
    return obj->ngeoms;
}

struct mg_object *mg_sub_at(const struct mg_object *obj, int i)
{
    assert(obj);
    if (obj->ngeoms == 1) {
        if (i == 0) {
            return (struct mg_object *)obj;
        }
    }
    else {
        if (i < obj->ngeoms) {
            return obj->objects[i];
        }
    }
    return NULL;
}

int mg_point_n(const struct mg_object *obj)
{
    assert(obj);
    if (obj->ngeoms == 1) {
        return obj->npoints;
    }
    else {
        int n = 0;
        for (int i = 0; i < obj->ngeoms; ++i) {
            struct mg_object *sub = obj->objects[i];
            if (sub == NULL)
                continue;
            n += sub->npoints;
        }
        return 0;
    }
}

/* ------------------------------- geometry io ------------------------------ */

struct mg_object *mg_read(int flag, const char *data, size_t len)
{
    assert(data);
    switch (flag) {
    case GEOMETRY_IO_WKT: {
        return mg_read_wkt(data, len);
    }
    case GEOMETRY_IO_WKB: {
        return mg_read_wkb(data, len, false);
    }
    case GEOMETRY_IO_WKB_HEX: {
        return mg_read_wkb(data, len, true);
    }
    case GEOMETRY_IO_GEOJSON: {
        return mg_read_geojson(data, len);
    }
    case GEOMETRY_IO_EWKT: {
        return mg_read_ewkt(data, len);
    }
    case GEOMETRY_IO_EWKB: {
        return mg_read_ewkb(data, len);
    }
    case GEOMETRY_IO_KML: {
        return mg_read_kml(data, len);
    }
    case GEOMETRY_IO_GML2: {
        return mg_read_gml2(data, len);
    }
    case GEOMETRY_IO_GML3: {
        return mg_read_gml3(data, len);
    }
    default:
        break;
    }
    return NULL;
}

int mg_write(int flag, const struct mg_object *obj, char **data, size_t *len)
{
    return 0;
}

/* ------------------------- geometry reader writer ------------------------- */

struct mg_object *mg_i4_object(struct mg_i4 *i4)
{
    assert(i4);
    return i4->obj;
}

void mg_i4_propProp(const struct mg_i4 *i4, int *propSize, int **prop)
{
    assert(i4);
    *propSize = i4->propSize;
    *prop = i4->prop;
}

int mg_i4_prop_value(const struct mg_i4 *i4, size_t index)
{
    assert(i4);
    assert(index < i4->propSize);
    return i4->prop[index];
}

struct mg_reader2 *mg_reader2_new(size_t size)
{
    if (size == 0)
        size = 10;
    struct mg_reader2 *reader =
        (struct mg_reader2 *)malloc(sizeof(struct mg_reader2));
    if (reader == NULL)
        return NULL;
    reader->size = size;
    reader->objs = (struct mg_i4 **)calloc(size, sizeof(struct mg_i4 *));
    if (reader->objs == NULL) {
        free(reader);
        return NULL;
    }
    reader->current = 0;
    reader->index = NULL;

    return reader;
}

void mg_free_reader2(struct mg_reader2 *reader)
{
}

void mg_input_reader(struct mg_reader2 *reader, const struct mg_object *obj,
                     int propSize, int *prop)
{
}

struct mg_i4 *mg_reader2_iterator(struct mg_reader2 *writer)
{
    return NULL;
}

/* -------------------------------- tolerance ------------------------------- */

static double g_tolerance = 0.0001;

double mg_tolerance(double tol)
{
    double tmp = g_tolerance;
    g_tolerance = tol;
    return tmp;
}

double tolerence()
{
    return g_tolerance;
}

/* --------------------------- geometry algorithm --------------------------- */

bool mg_check_single_ring(const double *pp, int npoints, int cdim)
{
    assert(pp);
    // At least 4 points are required to form a ring
    if (npoints < 4) return false;

    double x0 = pp[0];
    double y0 = pp[1];
    double xn = pp[(ptrdiff_t) (npoints * cdim)];
    double yn = pp[(ptrdiff_t) (npoints * cdim + 1)];

    return MG_DOUBLE_NEARES2(x0, xn) && MG_DOUBLE_NEARES2(y0, yn);
}

bool mg_ccw(const double *pp, int npoints, int cdim)
{
    if (!mg_check_single_ring(pp, npoints, cdim))
        return false;

    // The ring must be a convex point at the vertex extreme value, which is the
    // product of the line segments formed by the vertices before and after the
    // point. If it is greater than 0, it means the ring is counterclockwise,
    // and if it is less than 0, it means the ring is clockwise.

    // Flat Ring
    // The Y coordinate of at least one of the adjacent points of the lowest
    // point on the ring is the same as that of the lowest point.

    // of points without closing endpoint
    // Search for the highest point
    double lowY = pp[1];
    int lowIndex = 0;
    for (int i = 0; i < npoints; ++i)
    {
        double ty = pp[(ptrdiff_t) (i * cdim + 1)];
        if (ty < lowY)
        {
            lowIndex = i;
            lowY = ty;
        }
    }

    double x1 = pp[(ptrdiff_t) ((lowIndex - 1) * cdim)];
    double y1 = pp[(ptrdiff_t) ((lowIndex - 1) * cdim + 1)];
    double x2 = pp[(ptrdiff_t) (lowIndex * cdim)];
    double y2 = pp[(ptrdiff_t) (lowIndex * cdim + 1)];
    double x3 = pp[(ptrdiff_t) (((lowIndex + 1) % npoints) * cdim)];
    double y3 = pp[(ptrdiff_t) (((lowIndex + 1) % npoints) * cdim + 1)];

    double r = (x2 - x1) * (y3 - y2) - (y2 - y1) * (x3 - x2);
    return (r > 0) ? true : false; 
}

double mg_prop_value(const struct mg_object *obj, int mode)
{
    assert(obj);
    switch (mode) {
    case GEOMETRY_PROP_VALUE_LENGTH: {
        return mg_prop_length_value(obj);
    }
    case GEOMETRY_PROP_VALUE_WIDTH: {
        return mg_prop_width_value(obj);
    }
    case GEOMETRY_PROP_VALUE_HEIGHT: {
        return mg_prop_height_value(obj);
    }
    case GEOMETRY_PROP_VALUE_AREA: {
        return mg_prop_area_value(obj);
    }
    default:
        return 0;
    }
}

struct mg_object *mg_prop_geo(const struct mg_object *obj, int mode)
{
    assert(obj);
    switch (mode) {
    case GEOMETRY_PROP_GEO_CLONE: {
        return NULL;
    }
    }
    return NULL;
}

void mg_prop_geo2(const struct mg_object *obj, int mode, double *paras)
{
}

int mg_left_right(const struct mg_object *obj, double *xy)
{
    return 0;
}

void mg_vertex_convex(const struct mg_object *obj, int index, int *convex)
{
}
