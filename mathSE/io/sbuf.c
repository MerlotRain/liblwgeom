#include "sbuf.h"

#include <stdlib.h>
#include <string.h>

#define SBUF_MASK (sizeof(int) * 2)

sbuf sbufnew(const char *init) {
    if (init == NULL)
        return NULL;
    return sbufnewlen(init, strlen(init));
}

sbuf sbufnewlen(const void *init, size_t len) {
    if (init == NULL)
        return NULL;

    sbuf s;

    sbufhdr *sh = (sbufhdr *)malloc(len + SBUF_MASK + 1);
    if (sh == NULL)
        return NULL;
    memset(sh, 0, len + SBUF_MASK + 1);
    sh->capacity = len;
    sh->length   = len;
    s            = (char *)sh + SBUF_MASK;
    memcpy(s, init, len);
    s[len] = '\0';
    return s;
}

void sbuffree(sbuf s) {
    if (s == NULL)
        return;
    sbufhdr *sh = (sbufhdr *)s - SBUF_MASK;
    free(sh);
}

sbuf sbufcat(sbuf s, const char *t) { return NULL; }

sbuf sbufcatlen(sbuf s, const void *t, size_t len) { return NULL; }

sbuf sbufcpy(sbuf s, const char *t) { return NULL; }

sbuf sbufcpylen(sbuf s, const void *t, size_t len) { return NULL; }

sbuf sbufcatvprintf(sbuf s, const char *fmt, va_list ap) { return NULL; }

sbuf sbufcatprintf(sbuf s, const char *fmt, ...) { return NULL; }

sbuf sbufcatfmt(sbuf s, const char *fmt, ...) { return NULL; }

sbuf sbuftrim(sbuf s, const char *cset) { return NULL; }

sbuf sbufsbustr(sbuf s, size_t start, size_t len) { return NULL; }
