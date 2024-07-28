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

#include <buf.h>

// buf_append appends data to buffer. To append a null-terminated c-string
// specify -1 for the len.
bool buf_append(struct buf *buf, const char *data, ssize_t len)
{
    if (len < 0) {
        len = strlen(data);
    }
    if (buf->len + len >= buf->cap) {
        size_t cap = buf->cap ? buf->cap * 2 : 1;
        while (buf->len + len > cap) {
            cap *= 2;
        }
        char *data = (char *)malloc(cap + 1);
        if (!data) {
            return false;
        }
        memcpy(data, buf->data, buf->len);
        free(buf->data);
        buf->data = data;
        buf->cap = cap;
    }
    memcpy(buf->data + buf->len, data, len);
    buf->len += len;
    buf->data[buf->len] = '\0';
    return true;
}

// buf_append_byte appends a single byte to buffer.
// Returns false if the
bool buf_append_byte(struct buf *buf, char ch)
{
    if (buf->len == buf->cap) {
        return buf_append(buf, &ch, 1);
    }
    buf->data[buf->len] = ch;
    buf->len++;
    buf->data[buf->len] = '\0';
    return true;
}

// buf_clear clears the buffer and frees all data
void buf_clear(struct buf *buf)
{
    if (buf->data) {
        free(buf->data);
    }
    memset(buf, 0, sizeof(struct buf));
}