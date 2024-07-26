/*****************************************************************************/
/*  MathSE - Open source 2D geometry algorithm library                       */
/*                                                                           */
/*  Copyright (C) 2013-2024 Merlot.Rain                                      */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/


#ifndef HASH_H
#define HASH_H


#include <stdint.h>
#include <inttypes.h>

#define HASH_LOAD 75 // in percent. recommended is 50
#define HASH_NOT_FOUND (uint64_t) - 1

struct _hashbucket
{
  uint64_t key;
  uint64_t value;
};
typedef struct _inthash
{
  struct _hashbucket *array; /* of key, value pairs */
  uint64_t size;
  uint64_t elems; // to get the fill rate
} se_inthash;

se_inthash *hash_new (uint64_t size);
uint64_t hash_get (se_inthash *hash, uint64_t key);
void hash_set (se_inthash *hash, uint64_t key, uint64_t value);
void hash_free (se_inthash *hash);

#endif
