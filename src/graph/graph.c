#include "graph.h"
#include <stddef.h>
#include <nv-common.h>

struct un_graph *graph_delaunay(double *pp, int num_points)
{
    struct un_graph *delanuay =
        (struct un_graph *)nv__malloc(sizeof(struct un_graph));
    if (delanuay == NULL)
        return NULL;

    delanuay->nodes =
        (struct ung_node *)nv__malloc(num_points * sizeof(struct ung_node));
    if (delanuay->nodes == NULL) {
        nv__free(delanuay);
        return NULL;
    }
    delanuay->pp = pp;
    delanuay->num_nodes = num_points;
    delanuay->edges = NULL;
    delanuay->num_edges = 0;

    return delanuay;
}