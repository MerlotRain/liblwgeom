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

#include "uuid.h"
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#include <wincrypt.h>
#endif

static uint64_t seed[2];

static uint64_t xorshift128plus(uint64_t *s)
{
    /* http://xorshift.di.unimi.it/xorshift128plus.c */
    uint64_t s1 = s[0];
    const uint64_t s0 = s[1];
    s[0] = s0;
    s1 ^= s1 << 23;
    s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return s[1] + s0;
}

int uuid_init()
{
#if defined(__APPLE__) || defined(__linux__)
    int res;
    FILE *fp = fopen("/dev/urandom", "rb");
    if (!fp) {
        return -1;
    }
    res = fread(seed, 1, sizeof(seed), fp);
    fclose(fp);
    if (res != sizeof(seed)) {
        return -1;
    }
#elif defined(_WIN32)
    int res;
    HCRYPTPROV hCryptProv;
    res = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL,
                              CRYPT_VERIFYCONTEXT);
    if (!res) {
        return -1;
    }
    res = CryptGenRandom(hCryptProv, (DWORD)sizeof(seed), (PBYTE)seed);
    CryptReleaseContext(hCryptProv, 0);
    if (!res) {
        return -1;
    }
#else
#error "uuid_init: unsupported platform"
#endif
    return 0;
}

void uuid_generate(char *dst)
{
    static const char *template = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    static const char *chars = "0123456789abcdef";
    union {
        unsigned char b[16];
        uint64_t word[2];
    } s;
    const char *p;
    int i, n;
    /* get random */
    s.word[0] = xorshift128plus(seed);
    s.word[1] = xorshift128plus(seed);
    /* build string */
    p = template;
    i = 0;
    while (*p) {
        n = s.b[i >> 1];
        n = (i & 1) ? (n >> 4) : (n & 0xf);
        switch (*p) {
        case 'x':
            *dst = chars[n];
            i++;
            break;
        case 'y':
            *dst = chars[(n & 0x3) + 8];
            i++;
            break;
        default:
            *dst = *p;
        }
        dst++, p++;
    }
    *dst = '\0';
}
