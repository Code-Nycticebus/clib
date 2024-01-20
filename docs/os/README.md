# [io.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/io.h)
## Usage
Use the functions:
```c
Error e = ErrCreate;
io_write(stdout, BYTES_STR("Hello, World"), &e);
```

The input function is just like the one in python:
```c
Str ret = input(STR(":> "));
printf("input: '"STR_FMT"'\n", STR_ARG(ret));
```
Outputs:
```console
:> name
input: 'name'
```
# [os.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/os.h)
# [fs.h](https://github.com/Code-Nycticebus/clib/blob/main/src/os/fs.h)