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


struct mg_object *geom_create_single(int gdim, int pn, int cdim,
                                     const double *pp, int flag)
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
            geom_free(obj);
            return NULL;
        }
        memcpy(obj->pp, pp, sizeof(double) * pn * cdim);
    }
    return obj;
}

struct mg_object *geom_create_multi(int gdim, int snum, struct mg_object **subs)
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

void pri_geom_free(struct mg_object *g)
{
    assert(g);
    if (g->flag != 0) {
        free(g->pp);
    }
    free(g);
}

void geom_free(struct mg_object *g)
{
    assert(g);
    if (g->ngeoms == 1) {
        pri_geom_free(g);
    }
    else {
        for (int i = 0; i < g->ngeoms; ++i) {
            struct mg_object *sub = g->objects[i];
            if (sub == NULL)
                continue;
            geom_free(sub);
        }
    }
}