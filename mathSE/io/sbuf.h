#ifndef __SBUF_H__
#define __SBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef char *sbuf;

struct sbufhdr {
    int  capacity;
    int  length;
    char buf[];
};

#ifdef __cplusplus
}
#endif

#endif //__SBUF_H__