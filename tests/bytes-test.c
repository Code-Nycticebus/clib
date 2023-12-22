#include "clib/asserts.h"
#include "clib/bytes.h"

void test_bytes(void) {
  Arena arena = {0};
  Bytes b = BYTES(0x02, 0xFF, 0xAA, 0xBB);
  clib_assert(b.data[0] == 0x02, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0xff, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0xaa, "Bytes are not initialized correctly");
  clib_assert(b.data[3] == 0xbb, "Bytes are not initialized correctly");

  Str s = bytes_hex(b, &arena);
  clib_assert(str_eq(s, STR("02ffaabb")), "String conversion was not correct!");
  arena_free(&arena);
}

void test_bytes_str(void) {
  Arena arena = {0};
  Bytes b = BYTES_STR("ABC");
  clib_assert(b.data[0] == 0x41, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0x42, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0x43, "Bytes are not initialized correctly");

  Str s = bytes_hex(b, &arena);
  clib_assert(str_eq(s, STR("414243")), "String conversion was not correct!");
  arena_free(&arena);
}

void test_bytes_cmp(void) {
  Bytes b1 = BYTES(0xFF);
  clib_assert(bytes_eq(b1, BYTES(0xFF)) == true,
              "Bytes not compared correctly");
  clib_assert(bytes_eq(b1, BYTES(0x0F)) == false,
              "Bytes not compared correctly");
}

void test_bytes_slice(void) {
  Bytes b = BYTES_STR("ABC");
  Bytes slice = bytes_slice(b, 1, 3);
  clib_assert(bytes_eq(slice, BYTES_STR("BC")),
              "Bytes were not sliced correctly");
  Bytes invalid = bytes_slice(b, b.size, 2);
  clib_assert(bytes_eq(invalid, BYTES_STR("")), "Bytes were not invalidated");

  Bytes invalid2 = bytes_slice(b, 0, b.size + 1);
  clib_assert(bytes_eq(invalid2, BYTES_STR("")), "Bytes were not invalidated");
}

int main(void) {
  test_bytes();
  test_bytes_str();
  test_bytes_cmp();
  test_bytes_slice();
}