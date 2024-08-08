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

struct mg_object *geom_create_single(int gdim, int pn, int cdim, const double* pp, int flag)
{
    assert(pp);
    struct mg_object* obj = (struct mg_object)malloc(sizeof(struct mg_object));
    if(obj == NULL)
        return NULL;
    memset(obj, 0, sizeof(struct mg_object));

    obj->gdim = gdim;
    obj->ngeoms = 1;
    obj->path = (struct mg_path**)malloc(sizeof(obj->ngeoms * sizeof(struct mg_path*)));
    if(obj->path == NULL)
    {
        geom_free(obj);
        return NULL;
    }
    obj->path->gdim = gdim;
    obj->path->npoints = pn;
    obj->path->flag = flag;
    if(flag == 0)
    {
        obj->path->pp = pp;
    }
    else
    {
        obj->path->pp = (double *)malloc(sizeof(double) * pn * cdim);
        if(obj->path->pp == NULL)
        {
            geom_free(obj);
            return NULL;
        }
        memcpy(obj->path->pp, pp, sizeof(double) * pn * cdim);
    }
    return obj;
}

struct mg_object *geom_create_multi(int dim, int snum, struct mg_object** subs)
{
    assert(subs);
    struct mg_object* obj = (struct mg_object)malloc(sizeof(struct mg_object));
    if(obj == NULL)
        return NULL;
    memset(obj, 0, sizeof(struct mg_object));
    
    obj->gdim = gdim;
    obj->ngeoms = snum;
    obj->objects = subs;
    return obj;      
}

void pri_geom_free(struct mg_object *g)
{
    assert(g);
    if(g->path && path->flag != 0)
    {
        free(g->path->pp);
    }
    free(g->path);
    free(g);
}

void geom_free(struct mg_object* g)
{
    assert(g);
    if(g->ngeoms == 1)
    {
        pri_geom_free(g);
    }
    else
    {
        for(int i = 0; i < g->ngeoms; ++i)
        {
            struct mg_object* sub = g->objects[i];
            if(path == NULL)
                continue;
            geom_free(sub);
        }
    }
}