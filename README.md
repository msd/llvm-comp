# Compiler using LLVM-IR

Program to parse and compile source files of a C-like language (called "the language"
henceforth). The language supports `int`, `float`, `double` and
`bool` which map exactly to the corresponding C types.

## On ubuntu 22.04

### Installing requirements

`sudo apt install build-essential clang llvm`

### Compiling the compiler

This project uses the common out-of-source build layout.
Execute the following to produce the language compiler

```
mkdir build
cd build
cmake ../src
cmake --build .
```

This generates the executable `my_compiler` in the folder `build`.

You can add this compiler to the PATH environment variable for your convenience. (The next section assumes you have done so.)

 which can be used to compile a single source file of the language, producing `output.ll`.

Using the `output.ll` and some driver code (see `tests` folder) one can produce an executable

### Compiling a program to LLVM IR

Suppose you are trying to compile the language source file `program.c`

`my_compiler program.c`

should produce the LLVM intermediate representation (IR)
`program.ll` with all functions having external linkage.

### Driver

At the moment separate driver file is needed to print the results.
Simply add the program's functions' signatures
in `driver.cpp` and then you can use them like regular functions.

For example

```c
// program.c
int hello(float x, bool y){
    // ...
}
```

```cpp
// driver.cpp
#include <iostream>

int hello(float x, bool y);

int main()
{
    std::cout << "ans=" << hello(10.0f, false) << std::endl;
}
```

Note: you can find more examples in the `tests` folder.

### Generate executable

`clang++ program.ll driver.cpp -o my_executable`
