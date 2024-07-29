#include "mg.h"
#include "geom.h"

static struct mg_path *p_geom_create_path(int flag, const double* pp, int np)
{
    struct mg_path* path = (struct mg_path *)malloc(sizeof(struct mg_path) 
        + sizeof(struct mg_upoint) * np);
    if(path == NULL)
    {
        free(path);
        return NULL;
    }
    path->npoints = np;

    // create mg_upoint
    {
        double min_x = pp[0];
        double min_y = pp[1];
        double max_x = pp[0];
        double max_y = pp[1];
        int step = 2;
        step += (flag & MG_COORDINATE_FLAG_Z) : 1 ? 0;
        step += (flag & MG_COORDINATE_FLAG_M) : 1 ? 0;
        for(int i = 0; i < np; ++i)
        {            
            if (pp[i * step] < min_x) 
                min_x = pp[i * step];  
            if (pp[i * step] > max_x) 
                max_x = pp[i * step];  
            if (pp[i * step + 1] < min_y) 
                min_y = pp[i * step + 1];  
            if (pp[i * step + 1] > max_y) 
                max_y = pp[i * step + 1];

            path->points[i].x = pp[i * step];
            path->points[i].y = pp[i * step + 1];
            if (flag & MG_COORDINATE_FLAG_Z) {
                path->points[i].z = pp[i * step + 2];
                if (flag & MG_COORDINATE_FLAG_M) {
                    path->points[i].z = pp[i * step + 3];
                }
            } 
            else if (flag & MG_COORDINATE_FLAG_M) {
                path->points[i].m = pp[i * step + 2];
            }            
        }
    }
    return path;
}

static bool p_check_ring_close(struct mg_ring* r)
{
    assert(r);
    union mg_upoint pt0 = r->points[0];
    union mg_upoint ptn = r->points[r->npoints - 1];
    if(DBL_NEAR(pt0.x, ptn.x) && DBL_NEAR(pt0.y, ptn.y))
    {
        return true;
    }
    return false;
}


struct mg_geom *geom_new_point(int flag, const double*pp)
{
    assert(pp);
    struct mg_geom *g = (struct mg_geom *)malloc(sizeof(struct mg_geom));
    if (g == NULL)
        return NULL;
    memset(g, 0, sizeof(struct mg_geom));
    struct mg_point p = {.x = pp[0], .y = pp[1]};
    g->point.pt          = p;
    if (flag & MG_COORDINATE_FLAG_Z) {
        g->point.z = pp[2];
        if (flag & MG_COORDINATE_FLAG_M) {
            g->point.m = pp[3];
        }
    } 
    else if (flag & MG_COORDINATE_FLAG_M) {
        g->point.m = pp[2];
    }

    g->flag = flag;
    g->geomt = MG_POINT;
    return g;
}

struct mg_geom *geom_new_path(int flag, const double* pp, int np)
{
    assert(pp);
    struct mg_geom *g = (struct mg_geom *)malloc(sizeof(struct mg_geom));
    if(g == NULL)
        return NULL;
    memset(g, 0, sizeof(struct mg_geom));
    struct mg_path* path = p_geom_create_path(int flag, const double* pp, int np);
    if(path == NULL)
    {
        free(g);
        return NULL;
    }
    g->path = path;
    g->flag = flag;
    g->geomt = MG_PATH;
    return p;
}

struct mg_geom *geom_new_ring(int flag, const double* rp, int np)
{
    assert(pp);
    struct mg_geom *g = (struct mg_geom *)malloc(sizeof(struct mg_geom));
    if(g == NULL)
        return NULL;
    memset(g, 0, sizeof(struct mg_geom));
    struct mg_path* ring = p_geom_create_path(flag, rp, np)
    if(ring == NULL)
    {
        free(g);
        return NULL;
    }
    g->path = ring;
    g->flag = flag;
    g->geomt = MG_RING;
    return p;
}

struct mg_geom *geom_new_polygon(int flag, const double* sp, int spn, const double** hpp, 
    int* hpn, int hppn)
{
    assert(sp);
    struct mg_geom *g = (struct mg_geom *)malloc(sizeof(struct mg_geom));
    if(g == NULL)
        return NULL;
    
    struct mg_polygon* polygon = (struct mg_polygon*)malloc(sizeof(struct mg_polygon));
    if(polygon == NULL)
    {
        free(g);
        return NULL;
    }
    polygon->holes = (struct mg_ring**)malloc(sizeof(struct mg_ring*) * hppn);        
    struct mg_ring* shell = (struct mg_ring *)p_geom_create_path(flag, sp, spn);
    if(shell == NULL)
    {
        free(g);
        return NULL;
    }

    polygon->exterior = shell;
    polygon->nholes = hppn;
    for(int i = 0; i < hppn; ++i)
    {
        struct mg_ring *hole = (struct mg_ring *)p_geom_create_path(flag, hpp[i], hpn[i]);
        if(mg_ring)
        {
            polygon->holes[i] = hole;
        }
    }

    return polygon;
}

struct mg_geom *geom_new_multigeom(int gt, const struct mg_geom **geoms, int ng)
{
    struct mg_geom *g = (struct mg_geom*)malloc(sizeof(struct mg_geom));
    if(g == NULL)
    {
        return NULL;
    }
    if(gt == MG_POINT || gt == MG_MULTIPOINT)
        g->geomt = MG_MULTIPOINT;
    else if(gt == MG_PATH || gt == MG_MULTILINESTRING)
        g->geomt = MG_MULTILINESTRING;
    else if(gt == MG_POLYGON || gt == MG_MULTIPOLYGON)
        g->geomt = MG_MULTIPOLYGON;
    else
    {
        free(g);
        return NULL;
    }
    g->geoms = geoms;
    g->ngeoms = ng;

    // calc rect
    // flag?
    return g;
}

void geom_free(struct mg_geom *geom)
{
    
}