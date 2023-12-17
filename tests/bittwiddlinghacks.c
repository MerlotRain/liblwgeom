#include <limits.h>
#include <stdint.h>
#include <stdio.h>

/* http://graphics.stanford.edu/~seander/bithacks.html#OperationCounting */

int compute_the_sign_of_an_integer(int v) {
  int sign = (v) >> (sizeof(int) * CHAR_BIT - 1);
  return sign;
}

int main(int argc, char **argv) {}