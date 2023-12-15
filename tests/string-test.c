#include "clib/str.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>

bool sep(char c) { return isspace(c); }

void test_compare(void) {
  Str s = STR("Hello, World");

  assert(str_eq(s, STR("Hello, World")) == true);
  assert(str_startswith(s, STR("Hello, ")) == true);
  assert(str_endswith(s, STR(", World")) == true);

  assert(str_contains(s, STR("Hell")) == true);
  assert(str_contains(s, STR("World")) == true);
  assert(str_contains(s, STR("TEST")) == false);
}

void test_transform(void) {
  Arena *arena = arena_make();
  Str s = STR("Hello, World");
  Str lower = str_lower(s, arena);
  Str upper = str_upper(s, arena);

  assert(str_eq(lower, STR("hello, world")));
  assert(str_eq(upper, STR("HELLO, WORLD")));

  arena_free(arena);
}

void test_copy(void) {
  Arena *arena = arena_make();

  Str str = STR("Hello");
  Str str2 = str_copy(str, arena);
  Str str3 = str_from_cstr(", World");
  Str full = str_cat(str2, str3, arena);

  assert(str_eq(full, STR("Hello, World")));

  arena_free(arena);
}

void test_trim(void) {
  Str dirty = STR("\t  Hello World  \n");
  Str trim_l = str_trim_left(dirty);
  assert(str_eq(trim_l, STR("Hello World  \n")));

  Str trim_r = str_trim_right(dirty);
  assert(str_eq(trim_r, STR("\t  Hello World")));

  Str trim = str_trim(dirty);
  assert(str_eq(trim, STR("Hello World")));
}

void test_chop(void) {
  Str text = STR("Hello\nThis is text");
  Str h = str_chop_by_delim(&text, '\n');
  Str rest = str_chop_by_predicate(&text, sep);
  assert(str_eq(h, STR("Hello")));
  assert(str_eq(rest, STR("This")));
  assert(str_eq(text, STR("is text")));
}

void test_u64(void) {
  Arena *arena = arena_make();
  const size_t N = 64;
  Str number = str_u64_to_str(arena, N);
  assert(str_eq(number, STR("64")));

  Str n = str_cat(number, STR(" bytes"), arena);
  assert(str_eq(n, STR("64 bytes")));

  assert(str_to_u64(n) == 64);
  assert(str_chop_u64(&n) == 64);
  assert(str_eq(n, STR(" bytes")));

  arena_free(arena);
}

int main(void) {
  test_compare();
  test_copy();
  test_trim();
  test_chop();
  test_u64();
}
