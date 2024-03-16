#ifndef __SBUF_H__
#define __SBUF_H__

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
sbuf sbufnewlen(const void *init, int len);
void sbuffree(sbuf s);


#ifdef __cplusplus
}
#endif

#endif //__SBUF_H__