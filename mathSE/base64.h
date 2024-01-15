#ifndef __BASE64_H__
#define __BASE64_H__

#include <stddef.h>

char *base64_encode(const unsigned char *data, size_t len);

unsigned char *base64_decode(const char *text, size_t *out_len);

#endif //__BASE64_H__