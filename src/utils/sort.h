#ifndef __SORT_H__
#define __SORT_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Use quick sort algorithm to implement array sorting function。
 *
 * @param p the array begin pointer
 * @param ele_count number of elements
 * @param ele_size  size of one element
 * @param compare compare function to compare elements
 * @param user_data user data
 */
void quick_sort(const void *p, size_t ele_count, size_t ele_size,
                int (*compare)(const void *, const void *), void *user_data);

#ifdef __cplusplus
}
#endif

#endif // __SORT_H__