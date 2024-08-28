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

#include "buf.h"
#include <string.h>
#include <nv-common.h>

// buf_append appends data to buffer. To append a null-terminated c-string
// specify -1 for the len.
bool nv__buf_append(struct nv__buf *buf, const char *data, ssize_t len)
{
    if (len < 0) {
        len = strlen(data);
    }
    if (buf->len + len >= buf->cap) {
        size_t cap = buf->cap ? buf->cap * 2 : 1;
        while (buf->len + len > cap) {
            cap *= 2;
        }
        char *data = (char *)nv__malloc(cap + 1);
        if (!data) {
            return false;
        }
        memcpy(data, buf->data, buf->len);
        nv__free(buf->data);
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
bool nv__buf_append_byte(struct nv__buf *buf, char ch)
{
    if (buf->len == buf->cap) {
        return nv__buf_append(buf, &ch, 1);
    }
    buf->data[buf->len] = ch;
    buf->len++;
    buf->data[buf->len] = '\0';
    return true;
}

// buf_clear clears the buffer and frees all data
void nv__buf_clear(struct nv__buf *buf)
{
    if (buf->data) {
        nv__free(buf->data);
    }
    memset(buf, 0, sizeof(struct nv__buf));
}