# The `Globals.cpp` File

This file serves as an introduction to basic C++ concepts in the context of our
OneNote Debug Add-In project. It contains simple global variables and
demonstrates fundamental C++ programming principles that will be built upon in
later files.

## Why C++

C++ provides direct access to Windows APIs and system resources, essential for
building DLLs and COM objects that integrate with OneNote. Unlike higher-level
languages like .NET that abstract away low-level details, C++ exposes the inner
workings of DLL entry points (`DllMain`), COM registration mechanics, export
tables, and how external tools like `regsvr32.exe` and OneNote actually locate
and invoke our code—revealing the underlying mechanisms that make add-in loading
and COM registration work behind the scenes.

## Header File Inclusions

A header file inclusion, or include directive, is a statement in C or C++ that
instructs the compiler to include the contents of another file. This is done
using the `#include` preprocessor directive. Header files typically contain
function declarations, type definitions, macros, and constants that are shared
across multiple source files.

```cpp
#include <windows.h>
```

The statement above includes [the Windows API header][winnt]. Notice how it uses
angle brackets (we will learn about quotes later), which tells the compiler to
search for the header in the system include directories (such as the Windows SDK
or standard library). This is how you include standard or platform headers.

The Windows API header is included in `Globals.cpp` because it provides the
definition for the `HMODULE` type, which is required for declaring the global
variable `g_hModule` used throughout the project to represent the DLL module
handle.

## C++ Variables and Storage Classes

### Global Variables

Global variables in C++ are declared outside of any function and are accessible
from any part of the program after they are declared. In our `Globals.cpp` file:

```cpp
HMODULE g_hModule = NULL;
```

This declares a global variable `g_hModule` of type `HMODULE` and initializes it
to `NULL`. The `g_` prefix is a common naming convention indicating a global
variable.

### Storage Class Specifiers

Storage class specifiers in C and C++ determine the lifetime, visibility, and
linkage of functions and variables. The storage classes we will encounter are:

- `static`: Limits the variable's visibility to the current source file. This
  means the variable cannot be accessed from other files, helping encapsulate
  internal state and prevent name conflicts. For variables, `static` also means
  the variable retains its value between function calls.
- `extern`: Declares that the variable has external linkage, meaning it can be
  referenced from other files. This is used to share variables across multiple
  translation units (source files). For example, you might declare a variable as
  `extern` in a header file and define it in a source file.
- `auto` and `register`: These control automatic storage duration and suggest
  register storage, respectively. Modern C++ rarely uses these explicitly.

Storage class specifiers are fundamental for managing how code is organized and
how symbols are resolved during linking. They help you control the scope and
lifetime of your variables, making your code more modular and preventing
accidental symbol collisions.

## Windows Types

### HMODULE

`HMODULE` is a Windows-specific type that represents a handle to a loaded module
(DLL or EXE). In Windows programming, handles are opaque identifiers that the
operating system uses to track resources. An `HMODULE` specifically identifies
an instance of a loaded library in the process's memory space.

The module handle is essential because:

- It uniquely identifies the DLL instance in memory
- Windows uses it to manage resources associated with the DLL
- It enables operations like retrieving the DLL's file path
- It's required for accessing embedded resources (icons, dialogs, strings)

## C++ Macros

A macro in C or C++ is a preprocessor directive that defines a symbolic name or
pattern, which is replaced by its value or code before compilation. Macros are
created using the `#define` directive and can be used for constants, code
snippets, or conditional compilation. For example:

```cpp
#define PI 3.14159
```

This replaces every occurrence of `PI` in the code with `3.14159` before
compiling. Macros can also take parameters, allowing for simple code generation:

```cpp
#define SQUARE(x) ((x) * (x))
```

Macros are widely used in Windows programming to abstract platform-specific
details. For instance, `APIENTRY` is a macro that expands to the correct calling
convention for Windows API functions (usually `__stdcall`). Macros help make
code portable, readable, and easier to maintain, but they should be used
carefully, as they do not respect C++ scope or type safety.

## For Developers Coming from Other Languages

### For .NET Developers

In .NET, fields are members of classes and are not free-standing like C++ global
variables. The concept of a global variable does not exist in managed code;
everything is encapsulated in types. C++ global variables can be thought of as
similar to static fields in .NET classes, but they are not tied to a class or
object unless explicitly made so.

### For JavaScript Developers

C++ global variables are similar to variables declared with `var` at the top
level of a JavaScript file - they become part of the global scope and are
accessible from anywhere in the program. However, unlike JavaScript's dynamic
typing, C++ variables must be declared with specific types and that type cannot
change during the variable's lifetime.

## Next: The `DllMain.cpp` File

The next file to study is [`DllMain.cpp`](DllMain.cpp), which builds on these
foundational concepts to implement the DLL entry point. You'll learn about
Windows DLL lifecycle management, function signatures, and how the global
variables defined here are used in practice. For a detailed walkthrough, see
[`DllMain.md`](DllMain.md).

[winnt]:
  https://learn.microsoft.com/en-us/windows/win32/api/winnt/
  "winnt.h header"
