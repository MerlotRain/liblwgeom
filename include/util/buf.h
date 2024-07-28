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

#ifndef BUF_H
#define BUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mathse.h>
#include <sys/types.h>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

struct buf {
    char *data;
    size_t len, cap;
};

EXTERN bool buf_append(struct buf *buf, const char *data, ssize_t len);
EXTERN bool buf_append_byte(struct buf *buf, char ch);
EXTERN void buf_clear(struct buf *buf);

#ifdef __cplusplus
}
#endif

#endif