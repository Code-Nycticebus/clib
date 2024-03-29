#ifndef __CLIB_ASSERTS_H__
#define __CLIB_ASSERTS_H__

/* DOCUMENTATION
## Usage

Use the assertion macros to validate conditions and log failures:

```c
clib_assert(1 == 1, "It needs a message");
clib_assert(2 == 2, "It can even handle arguments: %d", 420);
```

## Available Macros

- `clib_assert(EXPR, FMT, ...)`: Asserts a condition and logs an error if the condition is false.
- `clib_assert_warn(EXPR, FMT, ...)`: Asserts a condition and logs a warning if the condition is false.
- `clib_assert_debug(EXPR, FMT, ...)`: Debug-only assert that logs a debug message if the condition is false.
- `clib_assert_return(EXPR, RETURN_VALUE)`: Asserts a condition and returns a value if the condition is false.

## Debug Break

A `debugbreak` is triggered on assertion failure to facilitate debugging, with behavior varying by platform:

- **Linux**: Uses `raise(SIGTRAP)`.
- **Windows**: Uses `__debugbreak()`.
- **Other Platforms**: Uses `abort()`.


*/

#include "clib/core/platform.h"
#include "logging.h" // IWYU pragma: export

////////////////////////////////////////////////////////////////////////////

#if defined(LINUX)
#include <signal.h> // IWYU pragma: export
#define debugbreak() raise(SIGTRAP)
#elif defined(WINDOWS)
#include <intrin.h> // IWYU pragma: export
#define debugbreak() __debugbreak()
#else
#include <stdlib.h> // IWYU pragma: export
#define debugbreak(...) abort()
#endif

////////////////////////////////////////////////////////////////////////////

#define _clib_assert_print(level, expr, ...)                                   \
  clib_log(level, "%s:%d: assert '%s' failed", __FILE__, __LINE__, expr);      \
  clib_log(level, "Description: "__VA_ARGS__)

#define clib_assert(expression, ...)                                           \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_ERROR, #expression, __VA_ARGS__);            \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)

#define clib_assert_return(expression, ret)                                    \
  do {                                                                         \
    if (!(expression)) {                                                       \
      return ret;                                                              \
    }                                                                          \
  } while (0)

#define clib_assert_warn(expression, ...)                                      \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_WARNING, #expression, __VA_ARGS__);          \
    }                                                                          \
  } while (0)

#ifndef NDEBUG
#define clib_assert_debug(expression, ...)                                     \
  do {                                                                         \
    if (!(expression)) {                                                       \
      _clib_assert_print(CLIB_LOG_DEBUG, #expression, __VA_ARGS__);            \
      debugbreak();                                                            \
    }                                                                          \
  } while (0)
#else
#define clib_assert_debug(...)
#endif

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ASSERTS_H__ */
