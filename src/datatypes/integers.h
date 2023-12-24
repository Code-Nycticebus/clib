#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

#include "clib/arena.h"
#include "datatypes.h"

u8 u8_reverse_bits(u8 value);
usize u8_leading_ones(u8 value);
usize u8_leading_zeros(u8 value);
u8 u8_to_be(u8 value);
u8 u8_from_be(u8 value);
u8 u8_to_le(u8 value);
u8 u8_from_le(u8 value);
Str u8_to_str(u8 value, Arena *arena);

#endif /* !__CLIB_INTEGERS_H__ */