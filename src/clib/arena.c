#include "arena.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
#include "datatypes/integers.h"

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

struct Chunk {
  Chunk *next;
  usize cap;
  usize allocated;
  u8 data[];
};

static Chunk *chunk_allocate(usize size) {
  Chunk *chunk = malloc(sizeof(Chunk) + size);
  clib_assert(chunk != NULL, "Memory allocation failed: %s", strerror(errno));
  chunk->cap = size;
  chunk->allocated = 0;
  return chunk;
}

static void chunk_free(Chunk *chunk) { free(chunk); }

void arena_free(Arena *arena) {
  Chunk *next = arena->begin;
  while (next != NULL) {
    Chunk *temp = next;
    next = next->next;
    chunk_free(temp);
  }
  arena->begin = NULL;
}

void arena_reset(Arena *arena) {
  for (Chunk *next = arena->begin; next != NULL; next = next->next) {
    next->allocated = 0;
  }
}

void *arena_alloc(Arena *arena, usize size) {
  Chunk *chunk = arena->begin;
  for (; chunk != NULL; chunk = chunk->next) {
    clib_assert_debug(size <= SIZE_MAX - chunk->allocated, "integer overflow");
    if (chunk->allocated + size < chunk->cap) {
      break;
    }
  }
  if (chunk == NULL) {
    const usize chunk_size = usize_max(size, CHUNK_DEFAULT_SIZE);
    chunk = chunk_allocate(chunk_size);
    chunk->next = arena->begin;
    arena->begin = chunk;
  }
  void *ptr = &chunk->data[chunk->allocated];
  chunk->allocated += size;
  return ptr;
}

void *arena_calloc(Arena *arena, usize size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}

void *arena_temp_alloc(Arena *arena, usize size) {
  Chunk *next = arena->begin;
  for (; next != NULL; next = next->next) {
    if (next->allocated == 0 && size < next->cap) {
      next->allocated = next->cap;
      return next->data;
    }
  }
  const usize chunk_size = usize_max(size, CHUNK_DEFAULT_SIZE);
  Chunk *chunk = chunk_allocate(chunk_size);
  chunk->allocated = chunk_size;
  chunk->next = arena->begin;
  arena->begin = chunk;
  return &chunk->data[0];
}

void *arena_temp_calloc(Arena *arena, usize size) {
  void *data = arena_temp_alloc(arena, size);
  memset(data, 0, size);
  return data;
}

void *arena_temp_realloc(Arena *arena, void *ptr, usize size) {
  if (ptr == NULL) {
    return arena_temp_alloc(arena, size);
  }
  Chunk *chunk = (Chunk *)((uintptr_t)ptr - sizeof(Chunk));
  if (size < chunk->cap) {
    return &chunk->data[0];
  }
  void *data = arena_temp_alloc(arena, size);
  memcpy(data, chunk->data, size);
  arena_temp_free(ptr);
  return data;
}

void arena_temp_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  Chunk *chunk = (Chunk *)((uintptr_t)ptr - sizeof(Chunk));
  chunk->allocated = 0;
}
