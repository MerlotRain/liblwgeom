#ifndef __SBUF_H__
#define __SBUF_H__

#include <sys/types.h>

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef char *sbuf;

typedef struct sbufhdr {
    int  capacity;
    int  length;
    char buf[];
} sbufhdr;

sbuf sbufnew(const char *init);
sbuf sbufnewlen(const void *init, size_t len);
void sbuffree(sbuf s);

sbuf sbufcat(sbuf s, const char *t);
sbuf sbufcatlen(sbuf s, const void *t, size_t len);
sbuf sbufcpy(sbuf s, const char *t);
sbuf sbufcpylen(sbuf s, const void *t, size_t len);

sbuf sbufcatvprintf(sbuf s, const char *fmt, va_list ap);
sbuf sbufcatprintf(sbuf s, const char *fmt, ...);

sbuf sbufcatfmt(sbuf s, const char *fmt, ...);
sbuf sbuftrim(sbuf s, const char *cset);
sbuf sbufsbustr(sbuf s, size_t start, size_t len);

#ifdef __cplusplus
}
#endif

#endif //__SBUF_H__