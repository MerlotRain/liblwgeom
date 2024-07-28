/*****************************************************************************/
/*  Math Spatial Engine - Open source 2D geometry algorithm library          */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef RTREE_H
#define RTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mathse.h>

// rtree_new returns a new rtree
//
// Returns NULL if the system is out of memory.
EXTERN struct rtree *rtree_new(void);

// rtree_free frees an rtree
EXTERN void rtree_free(struct rtree *tr);

// rtree_clone makes an instant copy of the btree.
//
// This operation uses shadowing / copy-on-write.
EXTERN struct rtree *rtree_clone(struct rtree *tr);

// rtree_set_item_callbacks sets the item clone and free callbacks that will be
// called internally by the rtree when items are inserted and removed.
//
// These callbacks are optional but may be needed by programs that require
// copy-on-write support by using the rtree_clone function.
//
// The clone function should return true if the clone succeeded or false if the
// system is out of memory.
EXTERN void rtree_set_item_callbacks(
    struct rtree *tr, bool (*clone)(const void *item, void **into, void *udata),
    void (*free)(const void *item, void *udata));

// rtree_set_udata sets the user-defined data.
//
// This should be called once after rtree_new() and is only used for
// the item callbacks as defined in rtree_set_item_callbacks().
EXTERN void rtree_set_udata(struct rtree *tr, void *udata);

// rtree_insert inserts an item into the rtree.
//
// This operation performs a copy of the data that is pointed to in the second
// and third arguments. The R-tree expects a rectangle, which is two arrays of
// doubles. The first N values as the minimum corner of the rect, and the next
// N values as the maximum corner of the rect, where N is the number of
// dimensions.
//
// When inserting points, the max coordinates is optional (set to NULL).
//
// Returns false if the system is out of memory.
EXTERN bool rtree_insert(struct rtree *tr, const double *min, const double *max,
                         const void *data);

// rtree_search searches the rtree and iterates over each item that intersect
// the provided rectangle.
//
// Returning false from the iter will stop the search.
EXTERN void rtree_search(const struct rtree *tr, const double *min,
                         const double *max,
                         bool (*iter)(const double *min, const double *max,
                                      const void *data, void *udata),
                         void *udata);

// rtree_scan iterates over every item in the rtree.
//
// Returning false from the iter will stop the scan.
EXTERN void rtree_scan(const struct rtree *tr,
                       bool (*iter)(const double *min, const double *max,
                                    const void *data, void *udata),
                       void *udata);

// rtree_count returns the number of items in the rtree.
EXTERN size_t rtree_count(const struct rtree *tr);

// rtree_delete deletes an item from the rtree.
//
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a binary comparison of its data to the provided
// data. The first item that is found is deleted.
//
// Returns false if the system is out of memory.
EXTERN bool rtree_delete(struct rtree *tr, const double *min, const double *max,
                         const void *data);

// rtree_delete_with_comparator deletes an item from the rtree.
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a comparison of its data to the provided data using
// a compare function. The first item that is found is deleted.
//
// Returns false if the system is out of memory.
EXTERN bool rtree_delete_with_comparator(
    struct rtree *tr, const double *min, const double *max, const void *data,
    int (*compare)(const void *a, const void *b, void *udata), void *udata);

// rtree_opt_relaxed_atomics activates memory_order_relaxed for all atomic
// loads. This may increase performance for single-threaded programs.
// Optionally, define RTREE_NOATOMICS to disbale all atomics.
EXTERN void rtree_opt_relaxed_atomics(struct rtree *tr);

#ifdef __cplusplus
}
#endif

#endif