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

#include <string.h>
#include <math.h>
#include <nv-common.h>

#define DIMS                2
#define MAXITEMS            64

// used for splits
#define MINITEMS_PERCENTAGE 10
#define MINITEMS            ((MAXITEMS) * (MINITEMS_PERCENTAGE) / 100 + 1)

#ifndef RTREE_NOPATHHINT
#define USE_PATHHINT
#endif

#ifdef RTREE_MAXITEMS
#undef MAXITEMS
#define MAXITEMS RTREE_MAXITEMS
#endif

typedef int rc_t;
static int rc_load(rc_t *ptr, int relaxed)
{
    (void)relaxed; // nothing to do
    return *ptr;
}
static int rc_fetch_sub(rc_t *ptr, int val)
{
    int rc = *ptr;
    *ptr -= val;
    return rc;
}
static int rc_fetch_add(rc_t *ptr, int val)
{
    int rc = *ptr;
    *ptr += val;
    return rc;
}

enum kind {
    LEAF = 1,
    BRANCH = 2,
};

struct rect {
    double min[DIMS];
    double max[DIMS];
};

struct item {
    const void *data;
};

struct node {
    rc_t rc;        // reference counter for copy-on-write
    enum kind kind; // LEAF or BRANCH
    int count;      // number of rects
    struct rect rects[MAXITEMS];
    union {
        struct node *nodes[MAXITEMS];
        struct item datas[MAXITEMS];
    };
};

struct nv_rtree {
    struct rect rect;
    struct node *root;
    size_t count;
    size_t height;
#ifdef USE_PATHHINT
    int path_hint[16];
#endif
    int relaxed;
    void *udata;
    int (*item_clone)(const void *item, void **into, void *udata);
    void (*item_free)(const void *item, void *udata);
};

static inline double min0(double x, double y)
{
    return x < y ? x : y;
}

static inline double max0(double x, double y)
{
    return x > y ? x : y;
}

static int feq(double a, double b)
{
    return !(a < b || a > b);
}

// nv_rtree_set_udata sets the user-defined data.
//
// This should be called once after nv_rtree_new() and is only used for
// the item callbacks as defined in nv_rtree_set_item_callbacks().
void nv_rtree_set_udata(struct nv_rtree *tr, void *udata)
{
    tr->udata = udata;
}

static struct node *node_new(struct nv_rtree *tr, enum kind kind)
{
    struct node *node = (struct node *)lwmalloc(sizeof(struct node));
    if (!node)
        return NULL;
    memset(node, 0, sizeof(struct node));
    node->kind = kind;
    return node;
}

static struct node *node_copy(struct nv_rtree *tr, struct node *node)
{
    struct node *node2 = (struct node *)lwmalloc(sizeof(struct node));
    if (!node2)
        return NULL;
    memcpy(node2, node, sizeof(struct node));
    node2->rc = 0;
    if (node2->kind == BRANCH) {
        for (int i = 0; i < node2->count; i++) {
            rc_fetch_add(&node2->nodes[i]->rc, 1);
        }
    }
    else {
        if (tr->item_clone) {
            int n = 0;
            int oom = LW_FALSE;
            for (int i = 0; i < node2->count; i++) {
                if (!tr->item_clone(node->datas[i].data,
                                    (void **)&node2->datas[i].data,
                                    tr->udata)) {
                    oom = LW_TRUE;
                    break;
                }
                n++;
            }
            if (oom) {
                if (tr->item_free) {
                    for (int i = 0; i < n; i++) {
                        tr->item_free(node2->datas[i].data, tr->udata);
                    }
                }
                lwfree(node2);
                return NULL;
            }
        }
    }
    return node2;
}

static void node_free(struct nv_rtree *tr, struct node *node)
{
    if (rc_fetch_sub(&node->rc, 1) > 0)
        return;
    if (node->kind == BRANCH) {
        for (int i = 0; i < node->count; i++) {
            node_free(tr, node->nodes[i]);
        }
    }
    else {
        if (tr->item_free) {
            for (int i = 0; i < node->count; i++) {
                tr->item_free(node->datas[i].data, tr->udata);
            }
        }
    }
    lwfree(node);
}

#define cow_node_or(rnode, code)                         \
    {                                                    \
        if (rc_load(&(rnode)->rc, tr->relaxed) > 0) {    \
            struct node *node2 = node_copy(tr, (rnode)); \
            if (!node2) {                                \
                code;                                    \
            }                                            \
            node_free(tr, rnode);                        \
            (rnode) = node2;                             \
        }                                                \
    }

static void rect_expand(struct rect *rect, const struct rect *other)
{
    for (int i = 0; i < DIMS; i++) {
        rect->min[i] = min0(rect->min[i], other->min[i]);
        rect->max[i] = max0(rect->max[i], other->max[i]);
    }
}

static double rect_area(const struct rect *rect)
{
    double result = 1;
    for (int i = 0; i < DIMS; i++) {
        result *= (rect->max[i] - rect->min[i]);
    }
    return result;
}

// return the area of two rects expanded
static double rect_unioned_area(const struct rect *rect,
                                const struct rect *other)
{
    double result = 1;
    for (int i = 0; i < DIMS; i++) {
        result *= (max0(rect->max[i], other->max[i]) -
                   min0(rect->min[i], other->min[i]));
    }
    return result;
}

static int rect_contains(const struct rect *rect, const struct rect *other)
{
    int bits = 0;
    for (int i = 0; i < DIMS; i++) {
        bits |= other->min[i] < rect->min[i];
        bits |= other->max[i] > rect->max[i];
    }
    return bits == 0;
}

static int rect_intersects(const struct rect *rect, const struct rect *other)
{
    int bits = 0;
    for (int i = 0; i < DIMS; i++) {
        bits |= other->min[i] > rect->max[i];
        bits |= other->max[i] < rect->min[i];
    }
    return bits == 0;
}

static int rect_onedge(const struct rect *rect, const struct rect *other)
{
    for (int i = 0; i < DIMS; i++) {
        if (feq(rect->min[i], other->min[i]) ||
            feq(rect->max[i], other->max[i])) {
            return LW_TRUE;
        }
    }
    return LW_FALSE;
}

static int rect_equals(const struct rect *rect, const struct rect *other)
{
    for (int i = 0; i < DIMS; i++) {
        if (!feq(rect->min[i], other->min[i]) ||
            !feq(rect->max[i], other->max[i])) {
            return LW_FALSE;
        }
    }
    return LW_TRUE;
}

static int rect_equals_bin(const struct rect *rect, const struct rect *other)
{
    for (int i = 0; i < DIMS; i++) {
        if (rect->min[i] != other->min[i] || rect->max[i] != other->max[i]) {
            return LW_FALSE;
        }
    }
    return LW_TRUE;
}

static int rect_largest_axis(const struct rect *rect)
{
    int axis = 0;
    double nlength = rect->max[0] - rect->min[0];
    for (int i = 1; i < DIMS; i++) {
        double length = rect->max[i] - rect->min[i];
        if (length > nlength) {
            nlength = length;
            axis = i;
        }
    }
    return axis;
}

// swap two rectangles
static void node_swap(struct node *node, int i, int j)
{
    struct rect tmp = node->rects[i];
    node->rects[i] = node->rects[j];
    node->rects[j] = tmp;
    if (node->kind == LEAF) {
        struct item tmp = node->datas[i];
        node->datas[i] = node->datas[j];
        node->datas[j] = tmp;
    }
    else {
        struct node *tmp = node->nodes[i];
        node->nodes[i] = node->nodes[j];
        node->nodes[j] = tmp;
    }
}

struct rect4 {
    double all[DIMS * 2];
};

static void node_qsort(struct node *node, int s, int e, int index)
{
    int nrects = e - s;
    if (nrects < 2) {
        return;
    }
    int left = 0;
    int right = nrects - 1;
    int pivot = nrects / 2;
    node_swap(node, s + pivot, s + right);
    struct rect4 *rects = (struct rect4 *)&node->rects[s];
    for (int i = 0; i < nrects; i++) {
        if (rects[right].all[index] < rects[i].all[index]) {
            node_swap(node, s + i, s + left);
            left++;
        }
    }
    node_swap(node, s + left, s + right);
    node_qsort(node, s, s + left, index);
    node_qsort(node, s + left + 1, e, index);
}

// sort the node rectangles by the axis. used during splits
static void node_sort_by_axis(struct node *node, int axis, int max)
{
    int by_index = max ? DIMS + axis : axis;
    node_qsort(node, 0, node->count, by_index);
}

static void node_move_rect_at_index_into(struct node *from, int index,
                                         struct node *into)
{
    into->rects[into->count] = from->rects[index];
    from->rects[index] = from->rects[from->count - 1];
    if (from->kind == LEAF) {
        into->datas[into->count] = from->datas[index];
        from->datas[index] = from->datas[from->count - 1];
    }
    else {
        into->nodes[into->count] = from->nodes[index];
        from->nodes[index] = from->nodes[from->count - 1];
    }
    from->count--;
    into->count++;
}

static int node_split_largest_axis_edge_snap(struct nv_rtree *tr,
                                             struct rect *rect,
                                             struct node *node,
                                             struct node **right_out)
{
    int axis = rect_largest_axis(rect);
    struct node *right = node_new(tr, node->kind);
    if (!right) {
        return LW_FALSE;
    }
    for (int i = 0; i < node->count; i++) {
        double min_dist = node->rects[i].min[axis] - rect->min[axis];
        double max_dist = rect->max[axis] - node->rects[i].max[axis];
        if (max_dist < min_dist) {
            // move to right
            node_move_rect_at_index_into(node, i, right);
            i--;
        }
    }
    // Make sure that both left and right nodes have at least
    // MINITEMS by moving datas into underflowed nodes.
    if (node->count < MINITEMS) {
        // reverse sort by min axis
        node_sort_by_axis(right, axis, LW_FALSE);
        do {
            node_move_rect_at_index_into(right, right->count - 1, node);
        } while (node->count < MINITEMS);
    }
    else if (right->count < MINITEMS) {
        // reverse sort by max axis
        node_sort_by_axis(node, axis, LW_TRUE);
        do {
            node_move_rect_at_index_into(node, node->count - 1, right);
        } while (right->count < MINITEMS);
    }
    if (node->kind == BRANCH) {
        node_sort_by_axis(node, 0, LW_FALSE);
        node_sort_by_axis(right, 0, LW_FALSE);
    }
    *right_out = right;
    return LW_TRUE;
}

static int node_split(struct nv_rtree *tr, struct rect *rect, struct node *node,
                      struct node **right)
{
    return node_split_largest_axis_edge_snap(tr, rect, node, right);
}

static int node_choose_least_enlargement(const struct node *node,
                                         const struct rect *ir)
{
    int j = 0;
    double jenlarge = INFINITY;
    for (int i = 0; i < node->count; i++) {
        // calculate the enlarged area
        double uarea = rect_unioned_area(&node->rects[i], ir);
        double area = rect_area(&node->rects[i]);
        double enlarge = uarea - area;
        if (enlarge < jenlarge) {
            j = i;
            jenlarge = enlarge;
        }
    }
    return j;
}

static int node_choose(struct nv_rtree *tr, const struct node *node,
                       const struct rect *rect, int depth)
{
#ifdef USE_PATHHINT
    int h = tr->path_hint[depth];
    if (h < node->count) {
        if (rect_contains(&node->rects[h], rect)) {
            return h;
        }
    }
#endif
    // Take a quick look for the first node that contain the rect.
    for (int i = 0; i < node->count; i++) {
        if (rect_contains(&node->rects[i], rect)) {
#ifdef USE_PATHHINT
            tr->path_hint[depth] = i;
#endif
            return i;
        }
    }
    // Fallback to using che "choose least enlargment" algorithm.
    int i = node_choose_least_enlargement(node, rect);
#ifdef USE_PATHHINT
    tr->path_hint[depth] = i;
#endif
    return i;
}

static struct rect node_rect_calc(const struct node *node)
{
    struct rect rect = node->rects[0];
    for (int i = 1; i < node->count; i++) {
        rect_expand(&rect, &node->rects[i]);
    }
    return rect;
}

// node_insert returns LW_FALSE if out of memory
static int node_insert(struct nv_rtree *tr, struct rect *nr, struct node *node,
                       struct rect *ir, struct item item, int depth, int *split)
{
    if (node->kind == LEAF) {
        if (node->count == MAXITEMS) {
            *split = LW_TRUE;
            return LW_TRUE;
        }
        int index = node->count;
        node->rects[index] = *ir;
        node->datas[index] = item;
        node->count++;
        *split = LW_FALSE;
        return LW_TRUE;
    }
    // Choose a subtree for inserting the rectangle.
    int i = node_choose(tr, node, ir, depth);
    cow_node_or(node->nodes[i], return LW_FALSE);
    if (!node_insert(tr, &node->rects[i], node->nodes[i], ir, item, depth + 1,
                     split)) {
        return LW_FALSE;
    }
    if (!*split) {
        rect_expand(&node->rects[i], ir);
        *split = LW_FALSE;
        return LW_TRUE;
    }
    // split the child node
    if (node->count == MAXITEMS) {
        *split = LW_TRUE;
        return LW_TRUE;
    }
    struct node *right;
    if (!node_split(tr, &node->rects[i], node->nodes[i], &right)) {
        return LW_FALSE;
    }
    node->rects[i] = node_rect_calc(node->nodes[i]);
    node->rects[node->count] = node_rect_calc(right);
    node->nodes[node->count] = right;
    node->count++;
    return node_insert(tr, nr, node, ir, item, depth, split);
}

// nv_rtree_new returns a new rtree
//
// Returns NULL if the system is out of memory.
struct nv_rtree *nv_rtree_new(void)
{
    struct nv_rtree *tr = (struct nv_rtree *)lwmalloc(sizeof(struct nv_rtree));
    if (!tr)
        return NULL;
    memset(tr, 0, sizeof(struct nv_rtree));
    return tr;
}

// nv_rtree_set_item_callbacks sets the item clone and free callbacks that will
// be called internally by the rtree when items are inserted and removed.
//
// These callbacks are optional but may be needed by programs that require
// copy-on-write support by using the nv_rtree_clone function.
//
// The clone function should return LW_TRUE if the clone succeeded or LW_FALSE
// if the system is out of memory.
void nv_rtree_set_item_callbacks(struct nv_rtree *tr,
                                 int (*clone)(const void *item, void **into,
                                              void *udata),
                                 void (*free)(const void *item, void *udata))
{
    tr->item_clone = clone;
    tr->item_free = free;
}

// nv_rtree_insert inserts an item into the rtree.
//
// This operation performs a copy of the data that is pointed to in the second
// and third arguments. The R-tree expects a rectangle, which is two arrays of
// doubles. The first N values as the minimum corner of the rect, and the next
// N values as the maximum corner of the rect, where N is the number of
// dimensions.
//
// When inserting points, the max coordinates is optional (set to NULL).
//
// Returns LW_FALSE if the system is out of memory.
int nv_rtree_insert(struct nv_rtree *tr, const double *min, const double *max,
                    const void *data)
{
    // copy input rect
    struct rect rect;
    memcpy(&rect.min[0], min, sizeof(double) * DIMS);
    memcpy(&rect.max[0], max ? max : min, sizeof(double) * DIMS);

    // copy input data
    struct item item;
    if (tr->item_clone) {
        if (!tr->item_clone(data, (void **)&item.data, tr->udata)) {
            return LW_FALSE;
        }
    }
    else {
        memcpy(&item.data, &data, sizeof(void *));
    }

    while (1) {
        if (!tr->root) {
            struct node *new_root = node_new(tr, LEAF);
            if (!new_root) {
                break;
            }
            tr->root = new_root;
            tr->rect = rect;
            tr->height = 1;
        }
        int split = LW_FALSE;
        cow_node_or(tr->root, break);
        if (!node_insert(tr, &tr->rect, tr->root, &rect, item, 0, &split)) {
            break;
        }
        if (!split) {
            rect_expand(&tr->rect, &rect);
            tr->count++;
            return LW_TRUE;
        }
        struct node *new_root = node_new(tr, BRANCH);
        if (!new_root) {
            break;
        }
        struct node *right;
        if (!node_split(tr, &tr->rect, tr->root, &right)) {
            lwfree(new_root);
            break;
        }
        new_root->rects[0] = node_rect_calc(tr->root);
        new_root->rects[1] = node_rect_calc(right);
        new_root->nodes[0] = tr->root;
        new_root->nodes[1] = right;
        tr->root = new_root;
        tr->root->count = 2;
        tr->height++;
    }
    // out of memory
    if (tr->item_free) {
        tr->item_free(item.data, tr->udata);
    }
    return LW_FALSE;
}

// nv_rtree_free frees an rtree
void nv_rtree_free(struct nv_rtree *tr)
{
    if (tr->root) {
        node_free(tr, tr->root);
    }
    lwfree(tr);
}

static int node_search(struct node *node, struct rect *rect,
                       int (*iter)(const double *min, const double *max,
                                   const void *data, void *udata),
                       void *udata)
{
    if (node->kind == LEAF) {
        for (int i = 0; i < node->count; i++) {
            if (rect_intersects(&node->rects[i], rect)) {
                if (!iter(node->rects[i].min, node->rects[i].max,
                          node->datas[i].data, udata)) {
                    return LW_FALSE;
                }
            }
        }
        return LW_TRUE;
    }
    for (int i = 0; i < node->count; i++) {
        if (rect_intersects(&node->rects[i], rect)) {
            if (!node_search(node->nodes[i], rect, iter, udata)) {
                return LW_FALSE;
            }
        }
    }
    return LW_TRUE;
}

// nv_rtree_search searches the rtree and iterates over each item that intersect
// the provided rectangle.
//
// Returning LW_FALSE from the iter will stop the search.
void nv_rtree_search(const struct nv_rtree *tr, const double min[],
                     const double max[],
                     int (*iter)(const double min[], const double max[],
                                 const void *data, void *udata),
                     void *udata)
{
    // copy input rect
    struct rect rect;
    memcpy(&rect.min[0], min, sizeof(double) * DIMS);
    memcpy(&rect.max[0], max ? max : min, sizeof(double) * DIMS);

    if (tr->root) {
        node_search(tr->root, &rect, iter, udata);
    }
}

static int node_scan(struct node *node,
                     int (*iter)(const double *min, const double *max,
                                 const void *data, void *udata),
                     void *udata)
{
    if (node->kind == LEAF) {
        for (int i = 0; i < node->count; i++) {
            if (!iter(node->rects[i].min, node->rects[i].max,
                      node->datas[i].data, udata)) {
                return LW_FALSE;
            }
        }
        return LW_TRUE;
    }
    for (int i = 0; i < node->count; i++) {
        if (!node_scan(node->nodes[i], iter, udata)) {
            return LW_FALSE;
        }
    }
    return LW_TRUE;
}

// nv_rtree_scan iterates over every item in the rtree.
//
// Returning LW_FALSE from the iter will stop the scan.
void nv_rtree_scan(const struct nv_rtree *tr,
                   int (*iter)(const double *min, const double *max,
                               const void *data, void *udata),
                   void *udata)
{
    if (tr->root) {
        node_scan(tr->root, iter, udata);
    }
}

// nv_rtree_count returns the number of items in the rtree.
size_t nv_rtree_count(const struct nv_rtree *tr)
{
    return tr->count;
}

static int node_delete(
    struct nv_rtree *tr, struct rect *nr, struct node *node, struct rect *ir,
    struct item item, int depth, int *removed, int *shrunk,
    int (*compare)(const void *a, const void *b, void *udata), void *udata)
{
    *removed = LW_FALSE;
    *shrunk = LW_FALSE;
    if (node->kind == LEAF) {
        for (int i = 0; i < node->count; i++) {
            if (!rect_equals_bin(ir, &node->rects[i])) {
                // Must be exactly the same, binary comparison.
                continue;
            }
            int cmp = compare ? compare(node->datas[i].data, item.data, udata)
                              : memcmp(&node->datas[i].data, &item.data,
                                       sizeof(void *));
            if (cmp != 0) {
                continue;
            }
            // Found the target item to delete.
            if (tr->item_free) {
                tr->item_free(node->datas[i].data, tr->udata);
            }
            node->rects[i] = node->rects[node->count - 1];
            node->datas[i] = node->datas[node->count - 1];
            node->count--;
            if (rect_onedge(ir, nr)) {
                // The item rect was on the edge of the node rect.
                // We need to recalculate the node rect.
                *nr = node_rect_calc(node);
                // Notify the caller that we shrunk the rect.
                *shrunk = LW_TRUE;
            }
            *removed = LW_TRUE;
            return LW_TRUE;
        }
        return LW_TRUE;
    }
    int h = 0;
#ifdef USE_PATHHINT
    h = tr->path_hint[depth];
    if (h < node->count) {
        if (rect_contains(&node->rects[h], ir)) {
            cow_node_or(node->nodes[h], return LW_FALSE);
            if (!node_delete(tr, &node->rects[h], node->nodes[h], ir, item,
                             depth + 1, removed, shrunk, compare, udata)) {
                return LW_FALSE;
            }
            if (*removed) {
                goto removed;
            }
        }
    }
    h = 0;
#endif
    for (; h < node->count; h++) {
        if (!rect_contains(&node->rects[h], ir)) {
            continue;
        }
        struct rect crect = node->rects[h];
        cow_node_or(node->nodes[h], return LW_FALSE);
        if (!node_delete(tr, &node->rects[h], node->nodes[h], ir, item,
                         depth + 1, removed, shrunk, compare, udata)) {
            return LW_FALSE;
        }
        if (!*removed) {
            continue;
        }
    removed:
        if (node->nodes[h]->count == 0) {
            // underflow
            node_free(tr, node->nodes[h]);
            node->rects[h] = node->rects[node->count - 1];
            node->nodes[h] = node->nodes[node->count - 1];
            node->count--;
            *nr = node_rect_calc(node);
            *shrunk = LW_TRUE;
            return LW_TRUE;
        }
#ifdef USE_PATHHINT
        tr->path_hint[depth] = h;
#endif
        if (*shrunk) {
            *shrunk = !rect_equals(&node->rects[h], &crect);
            if (*shrunk) {
                *nr = node_rect_calc(node);
            }
        }
        return LW_TRUE;
    }
    return LW_TRUE;
}

// returns LW_FALSE if out of memory
static int nv_rtree_delete0(
    struct nv_rtree *tr, const double *min, const double *max, const void *data,
    int (*compare)(const void *a, const void *b, void *udata), void *udata)
{
    // copy input rect
    struct rect rect;
    memcpy(&rect.min[0], min, sizeof(double) * DIMS);
    memcpy(&rect.max[0], max ? max : min, sizeof(double) * DIMS);

    // copy input data
    struct item item;
    memcpy(&item.data, &data, sizeof(void *));

    if (!tr->root) {
        return LW_TRUE;
    }
    int removed = LW_FALSE;
    int shrunk = LW_FALSE;
    cow_node_or(tr->root, return LW_FALSE);
    if (!node_delete(tr, &tr->rect, tr->root, &rect, item, 0, &removed, &shrunk,
                     compare, udata)) {
        return LW_FALSE;
    }
    if (!removed) {
        return LW_TRUE;
    }
    tr->count--;
    if (tr->count == 0) {
        node_free(tr, tr->root);
        tr->root = NULL;
        memset(&tr->rect, 0, sizeof(struct rect));
        tr->height = 0;
    }
    else {
        while (tr->root->kind == BRANCH && tr->root->count == 1) {
            struct node *prev = tr->root;
            tr->root = tr->root->nodes[0];
            prev->count = 0;
            node_free(tr, prev);
            tr->height--;
        }
        if (shrunk) {
            tr->rect = node_rect_calc(tr->root);
        }
    }
    return LW_TRUE;
}

// nv_rtree_delete deletes an item from the rtree.
//
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a binary comparison of its data to the provided
// data. The first item that is found is deleted.
//
// Returns LW_FALSE if the system is out of memory.
int nv_rtree_delete(struct nv_rtree *tr, const double *min, const double *max,
                    const void *data)
{
    return nv_rtree_delete0(tr, min, max, data, NULL, NULL);
}

// nv_rtree_delete_with_comparator deletes an item from the rtree.
// This searches the tree for an item that is contained within the provided
// rectangle, and perform a comparison of its data to the provided data using
// a compare function. The first item that is found is deleted.
//
// Returns LW_FALSE if the system is out of memory.
int nv_rtree_delete_with_comparator(
    struct nv_rtree *tr, const double *min, const double *max, const void *data,
    int (*compare)(const void *a, const void *b, void *udata), void *udata)
{
    return nv_rtree_delete0(tr, min, max, data, compare, udata);
}

// nv_rtree_clone makes an instant copy of the btree.
//
// This operation uses shadowing / copy-on-write.
struct nv_rtree *nv_rtree_clone(struct nv_rtree *tr)
{
    if (!tr)
        return NULL;
    struct nv_rtree *tr2 = (struct nv_rtree *)lwmalloc(sizeof(struct nv_rtree));
    if (!tr2)
        return NULL;
    memcpy(tr2, tr, sizeof(struct nv_rtree));
    if (tr2->root)
        rc_fetch_add(&tr2->root->rc, 1);
    return tr2;
}

// nv_rtree_opt_relaxed_atomics activates memory_order_relaxed for all atomic
// loads. This may increase performance for single-threaded programs.
// Optionally, define RTREE_NOATOMICS to disbale all atomics.
void nv_rtree_opt_relaxed_atomics(struct nv_rtree *tr)
{
    tr->relaxed = LW_TRUE;
}
