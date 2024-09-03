#include "geom-internal.h"
#include "bitset.h"

struct nv__simplify_rdp {
    int np;                  ///< number of points
    double *pp;              ///< points
    struct nv__bitset *usef; ///< useful points
};

static void nv__simplify_section(int i, int j, struct nv__simplify_rdp *intrdp)
{
}

static struct nv_geom *nv__simplify_line(const struct nv_geom *obj)
{
    if (obj == NULL)
        return NULL;

    if (obj->npoints < 3)
        return nv_prop_geo_clone(obj);

    struct nv__simplify_rdp *intrdp = (struct nv__simplify_rdp *)nv__malloc(
        sizeof(struct nv__simplify_rdp) + obj->npoints * sizeof(uint8_t));
    if (intrdp == NULL)
        return NULL;
    intrdp->np = obj->npoints;
    intrdp->pp = obj->pp;
    intrdp->usef = nv__bitset_new(obj->npoints);
    if (intrdp->usef == NULL) {
        nv__free(intrdp);
        return NULL;
    }

    nv__simplify_section(0, obj->npoints - 1, intrdp);

    return NULL;
}

struct nv_geom *nv_prop_geo_simpily_douglaspeucker(const struct nv_geom *obj)
{
    if (obj == NULL)
        return NULL;

    return NULL;
}
