/*****************************************************************************/
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

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