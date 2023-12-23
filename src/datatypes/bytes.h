#ifndef __CLIB_BYTES_H__
#define __CLIB_BYTES_H__

#include "clib/arena.h"
#include "clib/defines.h"
#include "datatypes.h" // IWYU pragma: private: include "bytes.h"

#define BYTES(...)                                                             \
  (Bytes) {                                                                    \
    sizeof((uint8_t[]){__VA_ARGS__}), (uint8_t[]) { __VA_ARGS__ }              \
  }

#define BYTES_STR(s)                                                           \
  (Bytes) {                                                                    \
    sizeof(s) - 1, (uint8_t[]) { (s) }                                         \
  }

Bytes bytes_from_parts(size_t size, const uint8_t *data);
Bytes bytes_copy(Bytes bytes, Arena *arena);

Bytes bytes_slice(Bytes bytes, size_t idx1, size_t idx2);

bool bytes_eq(Bytes b1, Bytes b2);

Str bytes_hex(Bytes bytes, Arena *arena);

size_t bytes_leading_ones(uint8_t byte);
size_t bytes_leading_zeros(uint8_t byte);

#endif /* !__CLIB_BYTES_H__ */
