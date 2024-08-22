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

#ifndef SDA_H
#define SDA_H

#include <mathse.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*deletor)(void *);

struct sda {
    unsigned char *data;
    deletor del;
    size_t elem_size;
    size_t capacity;
    size_t size;
};

EXTERN bool sda_new(struct sda *a, size_t elem_size, deletor del);
EXTERN bool sad_reserve(struct sda *a, size_t elem_size, size_t size,
                        deletor del);
EXTERN void sda_free(struct sda *a);

EXTERN bool sda_append(struct sda *a, void *d);
EXTERN bool sda_insert(struct sda *a, void *d, size_t i);
EXTERN bool sda_remove(struct sda *a, void *d);
EXTERN bool sda_remove2(struct sda *a, int i);
EXTERN size_t sda_size(const struct sda *a);
EXTERN void *sda_at(const struct sda *a, size_t i);
EXTERN bool sda_isempty(const struct sda *a);

EXTERN void **sda_rawdata(const struct sda *a);

#ifdef __cplusplus
}
#endif

#endif