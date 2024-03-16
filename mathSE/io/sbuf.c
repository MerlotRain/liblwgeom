#include "sbuf.h"

#include <stdlib.h>
#include <string.h>

#define SBUF_MASK (sizeof(int) * 2)

sbuf sbufnew(const char *init) {
    if (init == NULL)
        return NULL;
    return sbufnewlen(init, strlen(init));
}

sbuf sbufnewlen(const void *init, int len) {
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