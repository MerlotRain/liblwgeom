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

#ifndef GRAPH_H
#define GRAPH_H

#ifdef __cplusplus
extern "C" {
#endif

/// @brief undirected graph node
struct ung_node {
    int id;
};

/// @brief undirected graph edge
struct ung_edge {
    int id;   ///< id
    int from; ///< from node id
    int to;   ///< to node id
};

/// @brief undirected graph
struct un_graph {
    double *pp;             ///< graph point coordinates
    int num_nodes;          ///< number of nodes
    struct ung_node *nodes; ///< nodes
    int num_edges;          ///< number of edges
    struct ung_edge *edges; ///< edges
};

struct un_graph *graph_delaunay(double *pp, int num_points);

#ifdef __cplusplus
}
#endif

#endif