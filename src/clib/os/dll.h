/* DOCUMENTATION
## Functions

- **`dll_load(path, error)`**: Loads a dynamic link library.
- **`dll_close(handle)`**: Closes an opened dynamic link library.
- **`dll_symbol(handle, symbol, error)`**: Retrieves a symbol from the dynamic link library.


## Usage Example

```c
Error error = ErrNew;
Dll *myLib = dll_load(STR("myLibrary.dll"), &error);
error_context(&error, {
  error_raise();
});
Function *myFunction = dll_symbol(myLib, "myFunctionName", &error);
// Use the function pointer as needed
dll_close(myLib);
```
*/

#ifndef __CLIB_DLL_H__
#define __CLIB_DLL_H__

#include "clib/core/error.h"

typedef void Dll;
typedef void (*Function)(void);

Dll *dll_load(Str path, Error *error);
void dll_close(Dll *handle);

Function dll_symbol(Dll *handle, const char *symbol, Error *error);

#endif /*  !__CLIB_DLL_H__ */
