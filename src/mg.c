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
    obj->npoints = pn;
    obj->flag = flag;
    if (flag == 0) {
        obj->pp = (double *)pp;
    }
    else {
        obj->pp = (double *)malloc(sizeof(double) * pn * cdim);
        if (obj->pp == NULL) {
            mg_free(obj);
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
                obj->objects = (struct mg_object **)realloc(
                    obj->objects, obj->ngeoms * sizeof(struct mg_object *));
                if (obj->objects == NULL) {
                    free(obj);
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
                    obj->objects = (struct mg_object **)realloc(
                        obj->objects, obj->ngeoms * sizeof(struct mg_object *));
                    if (obj->objects == NULL) {
                        free(obj);
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
    return 0;
}

int mg_dim_g(const struct mg_object *obj)
{
    return 0;
}

int mg_sub_n(const struct mg_object *obj)
{
    return 0;
}

struct mg_object *mg_sub_at(const struct mg_object *obj, int i)
{
    return NULL;
}

int mg_point_n(const struct mg_object *obj)
{
    return 0;
}

/* ------------------------------- geometry io ------------------------------ */

struct mg_object *mg_read(int flag, const char *data, int len)
{
    return NULL;
}

int mg_write(int flag, const struct mg_object *obj, char **data, int len)
{
    return 0;
}

struct mg_object *mg_read_ora(int i_n, const int *i_p, int c_n, int c_dim,
                              const double *c_p, int flag)
{
    return NULL;
}

int mg_write_ora(struct mg_object *obj, int *i_n, int **i_p, int *c_n,
                 int *c_dim, double **c_p)
{
    return 0;
}

/* ------------------------- geometry reader writer ------------------------- */

struct mg_object *mg_i4_object(struct mg_i4 *i4)
{
}

void mg_i4_propProp(const struct mg_i4 *i4, int *propSize, int **prop)
{
}

int mg_i4_prop_value(const struct mg_i4 *i4, int index)
{
    return 0;
}

struct mg_reader2 *mg_create_reader(int size)
{
    return NULL;
}

struct mg_reader2 *mg_create_writer()
{
    return NULL;
}

void mg_free_reader2(struct mg_reader2 *reader)
{
}

void mg_input_reader(struct mg_reader2 *reader, const struct mg_object *obj,
                     int propSize, int *prop)
{
}

struct mg_i4 *mg_output_writer(struct mg_reader2 *writer)
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

double tolerance()
{
    return g_tolerance;
}

/* --------------------------- geometry algorithm --------------------------- */

/// calc geometry length
double mg_prop_length_value(const struct mg_object *obj);
/// calc geometry area
double mg_prop_area_value(const struct mg_object *obj);
/// calc geometry width
double mg_prop_width_value(const struct mg_object *obj);
/// calc geometry height
double mg_prop_height_value(const struct mg_object *obj);

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
}

void mg_prop_geo2(const struct mg_object *obj, int mode, double *paras)
{
}
