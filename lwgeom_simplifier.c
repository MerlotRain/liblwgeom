#include "./bitset.h"

struct nv__simplify_rdp {
	int np;         ///< number of points
	double *pp;     ///< points
	bitset_t *usef; ///< useful points
};

static void
nv__simplify_section(int i, int j, struct nv__simplify_rdp *intrdp)
{}

static LWGEOM *
nv__simplify_line(const LWGEOM *obj)
{
	if (obj == NULL)
		return NULL;

	if (obj->npoints < 3)
		return nv_prop_geo_clone(obj);

	struct nv__simplify_rdp *intrdp =
	    (struct nv__simplify_rdp *)lwmalloc(sizeof(struct nv__simplify_rdp) + obj->npoints * sizeof(uint8_t));
	if (intrdp == NULL)
		return NULL;
	intrdp->np = obj->npoints;
	intrdp->pp = obj->pp;
	intrdp->usef = nv__bitset_new(obj->npoints);
	if (intrdp->usef == NULL)
	{
		lwfree(intrdp);
		return NULL;
	}

	nv__simplify_section(0, obj->npoints - 1, intrdp);

	return NULL;
}

LWGEOM *
nv_prop_geo_simpily_douglaspeucker(const LWGEOM *obj)
{
	if (obj == NULL)
		return NULL;

	return NULL;
}
