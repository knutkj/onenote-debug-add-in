# The `DllMain.cpp` File

This file contains the entry point for a Windows DLL, the [`DllMain` C++
function][dllmain] in [the `DllMain.cpp` file](DllMain.cpp). It is called
automatically by Windows when the DLL is loaded or unloaded, or when threads are
created or destroyed in the process. This page focuses on DLL-specific concepts,
building on the foundational C++ knowledge covered in [`Globals.md`](Globals.md).

## Header File Inclusions in `DllMain.cpp`

```c++
// DllMain.cpp: Lines #3-5:
#include <windows.h>
#include "Globals.h"
#include "Logging.h"
```

The statements above include three headers:

- `<windows.h>`: The Windows API header (angle brackets indicate system headers)
- `"Globals.h"`: Our project header containing global variable declarations like `g_hModule`
- `"Logging.h"`: Our project header providing debug logging utilities for tracking DLL behavior

## C++ Function Signatures for DLL Entry Points

DLL entry points require specific function signatures to work correctly with Windows. A typical function signature looks like:

```
[LinkageSpecifier] [StorageClass] <ReturnType> [CallingConvention] <FunctionName>(
    <ParameterList>
) { ... }
```

Key components for DLL functions:
- **Linkage Specifier**: `extern "C"` for COM functions (prevents C++ name mangling)
- **Return Type**: `BOOL` for DllMain, `HRESULT` for COM functions
- **Calling Convention**: `APIENTRY` for DllMain, `__stdcall` for COM functions
- **Parameters**: Specific to the function's purpose

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

## More on Calling Conventions

**Calling conventions** define how functions receive parameters from the caller
and how the call stack is managed. This is critical for DLL entry points:

- `APIENTRY`: A macro that expands to `__stdcall` for Windows API functions
- `__stdcall`: Used by Windows API functions. The callee cleans up the stack
- `extern "C"`: Prevents C++ name mangling for COM functions

Why does this matter for DLLs? If you use the wrong calling convention, Windows may not be able to locate or execute your function properly, leading to crashes or undefined behavior. Always use `APIENTRY` for DLL entry points like `DllMain` to guarantee compatibility with the Windows loader and system tools.

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
