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

#include "sda.h"

#define SDA_DEFAULT_CAPACITY (10)

void sda_default_deletor(void *p)
{
    free(p);
}

bool sda_new(struct sda *a, size_t elem_size, deletor del)
{
    a->elem_size = elem_size;
    a->del = del ? del : sda_default_deletor;
    a->capacity = 10;
    a->size = 0;
    unsigned char *d =
        (unsigned char *)malloc(elem_size * SDA_DEFAULT_CAPACITY);
    if (d == NULL)
        return false;
    a->data = d;
    memset(a->data, 0, elem_size * SDA_DEFAULT_CAPACITY);
    return true;
}

bool sad_reserve(struct sda *a, size_t elem_size, size_t size, deletor del)
{
    return NULL;
}

void sda_free(struct sda *a)
{
}

void sda_free2(struct sda *a)
{
}

bool sda_append(struct sda *a, void *d)
{
    return false;
}

bool sda_insert(struct sda *a, void *d, size_t i)
{
    return false;
}

bool sda_remove(struct sda *a, void *d)
{
    return false;
}

bool sda_remove2(struct sda *a, int i)
{
    return false;
}

size_t sda_size(const struct sda *a)
{
    assert(a);
    return a->size;
}

void *sda_at(const struct sda *a, size_t i)
{
    return NULL;
}

bool sda_isempty(const struct sda *a)
{
    return false;
}

void **sda_rawdata(const struct sda *a)
{
}