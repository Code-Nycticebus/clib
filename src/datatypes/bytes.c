#include "bytes.h"
#include <stdio.h>
#include <string.h>

#define BITS 8
#define FIRST_BIT ((uint8_t)0x80)

Bytes bytes_from_parts(size_t size, const uint8_t *data) {
  return (Bytes){.size = size, .data = data};
}

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  uint8_t *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

Bytes bytes_slice(Bytes bytes, size_t idx1, size_t idx2) {
  if (idx2 <= idx1 || bytes.size <= idx1 || bytes.size < idx2) {
    return BYTES_STR("");
  }
  return bytes_from_parts(idx2 - idx1, &bytes.data[idx1]);
}

Str bytes_hex(Bytes bytes, Arena *arena) {
  char *buffer = arena_calloc(arena, bytes.size * 2 + 1);
  size_t b_idx = 0;
  for (size_t i = 0; i < bytes.size; i++) {
    if (i == 0) {
      b_idx += snprintf(&buffer[b_idx], 3, "%x", bytes.data[i]);
    } else {
      b_idx += snprintf(&buffer[b_idx], 3, "%02x", bytes.data[i]);
    }
  }
  return (Str){.len = b_idx, .data = buffer};
}

size_t bytes_leading_ones(uint8_t byte) {
  size_t count = 0;
  for (size_t i = 0; i < BITS; i++) {
    if (!(byte & (FIRST_BIT >> i))) {
      break;
    }
    count++;
  }
  return count;
}

size_t bytes_leading_zeros(uint8_t byte) {
  size_t count = 0;
  for (size_t i = 0; i < BITS; i++) {
    if (byte & (FIRST_BIT >> i)) {
      break;
    }
    count++;
  }
  return count;
}
