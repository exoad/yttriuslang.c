#include "../core/token.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h> 

static_assert(sizeof(struct token) >= 24, "`struct token` should be 24 bytes or of another size");
static_assert(__builtin_offsetof(struct token, vp) == 0, "vp=0");
static_assert(__builtin_offsetof(struct token, p) == 4, "p=4");
static_assert(__builtin_offsetof(struct token, len) == 8, "len=8");
static_assert(__builtin_offsetof(struct token, line_no) == 12, "line_no=12");
static_assert(__builtin_offsetof(struct token, type) == 16, "type=16");
static_assert(__builtin_offsetof(struct token, special) == 20, "special=20");
static_assert(__builtin_offsetof(struct token, fpv) == 34, "fpv=e4");


int isdigit(int c) {
  return (c >= '0' && c <= '9');
}

int isalnum(int c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

int isspace(int c) {
  return c == ' ' || (c >= '\t' && c <= '\r'); 
}
