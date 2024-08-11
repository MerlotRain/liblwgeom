#include "mgp.h"
#include "bitset.h"

struct int_simplify_rdp {
    int np;              ///< number of points
    double *pp;          ///< points
    struct bitset *usef; ///< useful points
};

void pri_simplify_section(int i, int j, struct int_simplify_rdp *intrdp)
{
    
}

static struct mg_object *pri_simplify_line(const struct mg_object *obj)
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

    pri_simplify_section(0, obj->npoints - 1, intrdp);

    return NULL;
}

struct mg_object *mg_prop_geo_simpily_RDP(const struct mg_object *obj)
{
    if (obj == NULL)
        return NULL;

    return NULL;
}
