# The `DllMain.cpp` File

This file contains the entry point for a Windows DLL, the [`DllMain` C++
function][dllmain] in [the `DllMain.cpp` file](DllMain.cpp). It is called
automatically by Windows when the DLL is loaded or unloaded, or when threads are
created or destroyed in the process. This page goes through the essential C++
and Windows concepts needed to understand how DLLs are loaded, how their entry
points work, and how external tools and applications interact with your code at
a low level. There are also a few appendixes at the end of this page with more
details.

## Why C++

C++ is a foundational language for Windows development, providing direct access
to the Windows API and system resources. It enables developers to build high-
performance applications, libraries, and components, including DLLs and COM
objects. Windows uses C++ for many of its core technologies, and understanding
C++ is essential for working with low-level system features, interoperability,
and custom extensions. By using C++, we are exposed to the inner details of COM
and OneNote add-in loading and usage, allowing for deeper control and
customization than higher-level languages typically provide. In contrast, .NET
abstracts away many of these details, so it would not let us learn everything
that is happening under the hood. Specifically, using .NET would mean missing
out on direct access to DLL entry points like `DllMain`, the mechanics of COM
registration and export tables, and the ability to interact with the Windows API
at the lowest level. Most importantly, C++ reveals how external tools like
`regsvr32.exe` and OneNote actually access and invoke our code—showing the
underlying calls and mechanisms that make add-in loading and COM registration
work behind the scenes.

## Header File Inclusions in `DllMain.cpp`

A header file inclusion, or include directive, is a statement in C or C++ that
instructs the compiler to include the contents of another file. This is done
using the `#include` preprocessor directive. Header files typically contain
function declarations, type definitions, macros, and constants that are shared
across multiple source files.

```c++
// DllMain.cpp: Line #3:
#include <windows.h>
#include "Logging.h"
```

The statements above includes the two headers:

- `<windows.h>`: It uses angle brackets, which tells the compiler to search for
  the header in the system include directories (such as the Windows SDK or
  standard library). This is how you include standard or platform headers.
  Including `<windows.h>` is required for any program or DLL that interacts with
  the Windows operating system, such as handling DLL entry points, working with
  system resources, or using Windows-specific features.
- `"Logging.h"`: It uses quotes, which tells the compiler to first look for the
  header in the current project directory, and only then in the system
  directories. This is how to include our own project-specific headers.
  Including `"Logging.h"` here provides simple debug logging utilities, so we
  can track and debug what is going on through the log file `DebugAddIn.log`,
  which is located in the same directory as the DLL.

## C++ Functions

A C++ function is a reusable block of code that performs a specific task. The
function signature defines how the function is called, what it returns, and what
parameters it accepts. A typical function signature looks like:

```
[LinkageSpecifier] [StorageClass] <ReturnType> [CallingConvention] <FunctionName>(
    <ParameterList>
) { ... }
```

- [**Linkage Specifier**][linkage]: Controls how the function is linked and how
  its symbol name is generated. The most common example is `extern "C"`, which
  tells the compiler to use C linkage (no C++ name mangling), making the
  function accessible to other languages and system tools. This is important for
  interoperability, especially for DLL exports and system-level functions. We
  will see examples of `extern "C"` in later files.
- [**Storage Class**][storage]: Specifies the storage duration, linkage, and
  visibility of the function. Examples include [`static` (internal
  linkage)][static] and [`extern` (external linkage)][extern]. These are called
  storage class specifiers in C and C++.
- **Return Type**: Specifies the type of value the function returns, such as
  `int`, `void`, or `BOOL`.
- **Calling Convention**: (Optional) Defines how arguments are passed and how
  the call stack is managed. Common conventions are `__cdecl`, `__stdcall`, and
  `APIENTRY`. For Windows API and COM functions, specifying the calling
  convention ensures correct interoperability.
- **Function Name**: The identifier used to call the function (e.g., `DllMain`).
- **Parameter List**: The types and names of values the function accepts,
  enclosed in parentheses. Parameters are separated by commas.

## The `DllMain` Function Signature

`DllMain` is the entry point for a Windows DLL. The operating system calls this
function when processes and threads are initialized and terminated, or when the
DLL is loaded or unloaded using `LoadLibrary` and `FreeLibrary`. In
`DllMain.cpp` we can see the following signature:

```cpp
// DllMain.cpp: Line #9:
BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved) { /* ... */ }
```

### Why the Calling Convention Must Be `APIENTRY`

The calling convention for `DllMain` must be `APIENTRY` (which typically expands
to `__stdcall`) because Windows expects all DLL entry points to use this
specific convention. This ensures that the operating system can reliably call
your function, pass parameters correctly, and manage the call stack without
errors. If you use a different calling convention, Windows may not be able to
locate or execute your function properly, leading to crashes or undefined
behavior. Always use `APIENTRY` for DLL entry points to guarantee compatibility
with the Windows loader and system tools.

### The Parameters

- `HMODULE hModule`: A handle to the DLL module. This parameter is needed
  because it uniquely identifies the instance of the DLL loaded in the process's
  memory space. Windows uses this handle to manage resources associated with the
  DLL, such as loading and unloading, accessing embedded resources, or
  performing operations like retrieving the DLL's file path. If your DLL needs
  to interact with its own resources or perform actions that require knowledge
  of its own instance, you use `hModule` to reference itself within the process.
  This is especially important for DLLs that contain resources (like icons,
  dialogs, or strings) or need to register/unregister themselves with the
  system. We are storing the module handle in the `g_hModule` variabel so that
  other files can have access to it.
- `DWORD ul_reason_for_call`: This parameter tells the DLL why it is being
  called by the operating system. It is a 32-bit unsigned integer that can take
  one of several predefined values, each representing a specific event. The
  value of `ul_reason_for_call` allows your DLL to respond appropriately to
  different lifecycle events, ensuring proper initialization and cleanup as the
  process and its threads start and stop.
  - `DLL_PROCESS_ATTACH`: The DLL is being loaded into the address space of the
    process. This is a good place to perform one-time initialization.
  - `DLL_THREAD_ATTACH`: A new thread is being created in the process, and the
    DLL is being notified. This is rarely used, but can be useful for thread-
    specific setup.
  - `DLL_THREAD_DETACH`: A thread is exiting cleanly, and the DLL is being
    notified. Use this to clean up any thread-specific resources.
  - `DLL_PROCESS_DETACH`: The DLL is being unloaded from the process's address
    space. This is the place for cleanup and releasing resources.
- `LPVOID lpReserved`: Stands for "Long Pointer to VOID" and is a generic
  pointer type in Windows. It can point to any type of data, but does not
  specify what that data is. It is often used for passing arbitrary data or
  context between functions.

### The Body

The body of `DllMain` performs debug logging for key lifecycle events. When the
DLL is loaded, unloaded, or when threads are created or destroyed, a message is
written to the `DebugAddIn.log` file (located in the same directory as the DLL).
This allows us to track and analyze exactly what is happening as the DLL
interacts with the host process and external tools.

### Return Value

Return `TRUE` for successful initialization, or `FALSE` to indicate failure
(rare).

## Next: The `DllReg.cpp` File

The next file to study is [`DllReg.cpp`](DllReg.cpp), which introduces the
Component Object Model (COM) and explains how a COM DLL is registered with
Windows. In this file, you'll learn about the `DllRegisterServer` function, how
it is exported from the DLL, and how registration tools like `regsvr32.exe` use
it to add necessary registry entries. For a detailed walkthrough, see
[`DllReg.md`](DllReg.md).

---

## Appendix: More on Storage Class

Storage class specifiers in C and C++ determine the lifetime, visibility, and
linkage of functions and variables. The most common storage class specifiers for
functions are:

- `static`: Limits the function's visibility to the current source file. This
  means the function cannot be called or referenced from other files, helping
  encapsulate internal logic and prevent name conflicts. For variables, `static`
  also means the variable retains its value between function calls.
- `extern`: Declares that the function or variable has external linkage, meaning
  it can be referenced from other files. This is used to share functions and
  variables across multiple translation units (source files). For example, you
  might declare a function as `extern` in a header file and define it in a
  source file.
- `auto` and `register`: These are rarely used for functions, but for variables,
  they control automatic storage duration and suggest register storage,
  respectively. Modern C++ rarely uses these.

Storage class specifiers are fundamental for managing how code is organized and
how symbols are resolved during linking. They help you control the scope and
lifetime of your functions and variables, making your code more modular and
preventing accidental symbol collisions. For more details, see the [Storage
Classes documentation][storage].

## Appendix: More on Calling Conventions

**Calling conventions** define how functions receive parameters from the caller
and how the call stack is managed. This is important for interoperability,
especially when working with system APIs or libraries compiled with different
settings. Common calling conventions include:

- `__cdecl`: The default for C/C++ programs. The caller cleans up the stack.
- `__stdcall`: Used by Windows API functions. The callee cleans up the stack.
- `APIENTRY`: A macro that expands to the appropriate calling convention for
  Windows API functions (usually `__stdcall`).
- `__fastcall`: Passes some arguments in registers for speed.

Why does this matter? If you mix calling conventions between function
implementations and callers, you can get crashes or unpredictable behavior. For
most C++ code, you don't need to specify a calling convention, but for Windows
API, COM, or DLL entry points, you must use the correct one to ensure your code
works with the operating system and other components.

In summary, storage classes control how and where a function can be used, while
calling conventions control how functions interact at the binary level. Both are
important for writing robust, interoperable C++ code, especially on Windows.

### C++ Macros

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

## Appendix: C++ Functions for .NET and JS Developers

### For .NET Developers

In .NET, methods are members of classes and are not free-standing like C++
functions. The concept of a global function does not exist; everything is
encapsulated in types. C++ functions can be thought of as similar to static
methods in .NET, but they are not tied to a class or object unless explicitly
made so.

### For JavaScript Developers

In context of C++ functions, think of a DLL as similar to a large, concatenated
JavaScript file containing multiple global functions. When you load a DLL in
Windows, all its exported functions become available for use, much like how
globally declared functions in JavaScript are accessible after the script is
loaded. Just as you can call any global function from a loaded JS file, you can
call exported functions from a loaded DLL.

When building a DLL, the C++ compiler (`cl.exe`) processes each `.cpp` file
individually, compiling them into object files. The linker then "stitches" these
object files together into a single DLL, resolving references between functions
and variables across files. This is conceptually similar to concatenating
multiple JS files into one bundle, where all global functions become available
after loading the bundle. However, in C++, this process is static and happens at
compile/link time, while in JavaScript, concatenation is dynamic and happens at
runtime or build time.

So, while the analogy is not perfect, loading a DLL and accessing its exported
functions is much like loading a bundled JS file and calling its global
functions. The main difference is that C++/DLLs are compiled and linked, while
JavaScript is interpreted or just-in-time compiled.

[dllmain]:
  https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
  "DllMain entry point"
[winnt]:
  https://learn.microsoft.com/en-us/windows/win32/api/winnt/
  "winnt.h header"
[extern]:
  https://learn.microsoft.com/en-us/cpp/cpp/storage-classes-cpp?view=msvc-170#extern
  "extern Storage Class"
[storage]:
  https://learn.microsoft.com/en-us/cpp/cpp/storage-classes-cpp?view=msvc-170
  "Storage Classes"
[static]:
  https://learn.microsoft.com/en-us/cpp/cpp/storage-classes-cpp?view=msvc-170#static
  "static Storage Class"
[linkage]:
  https://learn.microsoft.com/en-us/cpp/cpp/extern-cpp?view=msvc-170
  "Linkage Specifier "
