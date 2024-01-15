#include "base64.h"

static size_t base64_encode_step(const char *in, int len, int break_lines,
                                 char *out, int *state, int *save) {
  return 0;
}

static size_t base64_encode_close(int break_lines, char *out, int *state,
                                  int *save) {
  return 0;
}

static size_t base64_decode_step(const char *in, size_t len, unsigned char *out,
                                 int *state, unsigned int *save) {
  return 0;
}

static unsigned char *base64_decode_inplace(char *text, size_t *out_len) {
  return NULL;
}

char *base64_encode(const unsigned char *data, size_t len) { return NULL; }

unsigned char *base64_decode(const char *text, size_t *out_len) { return NULL; }
