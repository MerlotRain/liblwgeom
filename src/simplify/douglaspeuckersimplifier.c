#include "nvp.h"
#include <bitset.h>

struct int_simplify_rdp {
    int np;              ///< number of points
    double *pp;          ///< points
    struct bitset *usef; ///< useful points
};

static void _simplify_section(int i, int j, struct int_simplify_rdp *intrdp)
{
}

static struct nv_geobject *_simplify_line(const struct nv_geobject *obj)
{
    if (obj == NULL)
        return NULL;

    if (obj->npoints < 3)
        return mg_prop_geo_clone(obj);

    struct int_simplify_rdp *intrdp = (struct int_simplify_rdp *)malloc(
        sizeof(struct int_simplify_rdp) + obj->npoints * sizeof(uint8_t));
    if (intrdp == NULL)
        return NULL;
    intrdp->np = obj->npoints;
    intrdp->pp = obj->pp;
    intrdp->usef = bitset_new(obj->npoints);
    if (intrdp->usef == NULL) {
        free(intrdp);
        return NULL;
    }

    _simplify_section(0, obj->npoints - 1, intrdp);

    return NULL;
}

struct nv_geobject *
mg_prop_geo_simpily_douglaspeucker(const struct nv_geobject *obj)
{
    if (obj == NULL)
        return NULL;

    return NULL;
}
