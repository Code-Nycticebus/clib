#ifndef __CLIB_FLOATS_H__
#define __CLIB_FLOATS_H__

#include "core/defines.h"

#define FLOAT_DECL(T)                                                          \
  bool T##_eq(T a, T b);                                                       \
  bool T##_isnan(T value);                                                     \
  T T##_abs(T value);                                                          \
  T T##_min(T a, T b);                                                         \
  T T##_max(T a, T b);                                                         \
  T T##_clamp(T min, T max, T value);                                          \
  T T##_lerp(T min, T max, T value);                                           \
  T T##_rad(T deg);

FLOAT_DECL(f32)
FLOAT_DECL(f64)

#endif /* ifndef __CLIB_FLOATS_H__ */
