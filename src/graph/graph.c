/**
 * Copyright (c) 2023-present Merlot.Rain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
 
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