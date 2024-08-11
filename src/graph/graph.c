#include "graph.h"
#include <stddef.h>
#include <stdlib.h>

struct un_graph *graph_delaunay(double *pp, int num_points)
{
    struct un_graph *delanuay =
        (struct un_graph *)malloc(sizeof(struct un_graph));
    if (delanuay == NULL)
        return NULL;

    delanuay->nodes =
        (struct ung_node *)malloc(num_points * sizeof(struct ung_node));
    if (delanuay->nodes == NULL) {
        free(delanuay);
        return NULL;
    }
    delanuay->pp = pp;
    delanuay->num_nodes = num_points;
    delanuay->edges = NULL;
    delanuay->num_edges = 0;

    return delanuay;
}