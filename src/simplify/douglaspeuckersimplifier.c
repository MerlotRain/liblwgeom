#include "nv-common.h"
#include <bitset.h>

struct int_simplify_rdp {
    int np;              ///< number of points
    double *pp;          ///< points
    struct bitset *usef; ///< useful points
};

static void nv__simplify_section(int i, int j, struct int_simplify_rdp *intrdp)
{
}

static struct nv_geobject *nv__simplify_line(const struct nv_geobject *obj)
{
    if (obj == NULL)
        return NULL;

    if (obj->npoints < 3)
        return nv_prop_geo_clone(obj);

    struct int_simplify_rdp *intrdp = (struct int_simplify_rdp *)nv__malloc(
        sizeof(struct int_simplify_rdp) + obj->npoints * sizeof(uint8_t));
    if (intrdp == NULL)
        return NULL;
    intrdp->np = obj->npoints;
    intrdp->pp = obj->pp;
    intrdp->usef = bitset_new(obj->npoints);
    if (intrdp->usef == NULL) {
        nv__free(intrdp);
        return NULL;
    }

    nv__simplify_section(0, obj->npoints - 1, intrdp);

    return NULL;
}

struct nv_geobject *
nv_prop_geo_simpily_douglaspeucker(const struct nv_geobject *obj)
{
    if (obj == NULL)
        return NULL;

    return NULL;
}
