# OneNote Debug Add-In

## Assumptions

In this repository we assume that you are new to C++, C++-development in Visual
Studio, COM-development, and OneNote add-in development. If you are sufficient
in all of these, this repository might not be useful to you.

## What you can Learn from a high level Perspective

- Build a OneNote add-in from scratch in native code.
- How the COM supportedf DLL is registered in the Windows registry.
- How OneNote:
  - Loads the assembly.
  - Uses the assembly.

## Learning Guidelines

The recommended way of learning is to follow the natural flow of the files which
introduces the different concepts in a natural order:

### The `DllMain.cpp` File

This file contains the entry point for the DLL, called `DllMain`. It is called
automatically by Windows when the DLL is loaded or unloaded. The corresponding
[`DllMain.md`](src/DllMain.md) document explains the function signature,
parameters, calling conventions, and how Windows interacts with this entry
point.

### The `DllReg.cpp` File

This file implements the `DllRegisterServer` function, which is responsible for
registering the COM DLL in the Windows registry so its COM objects can be
discovered and used by other applications. The corresponding
[`DllReg.md`](src/DllReg.md) document covers how exported functions are made
visible to other programs, the role of the export table, the module definition
file (`.def`), and how registration tools like `regsvr32.exe` interact with this
function.

## Status

After building the project, the resulting DLL (assembly) will be placed in the
output folder specified by the default build configuration which is:

```
out/x64/Debug/DebugAddIn.dll
```

Currently, the DLL is set up to satisfy the requirements of the `regsvr32.exe`
tool. This means that `regsvr32.exe` can successfully locate and call the
`DllRegisterServer` function exported by the DLL. However, the registration
function is currently a no-op (it does not perform any real registration or
write to the registry yet).

This provides a working foundation for further development. Future updates will
implement the actual COM registration logic, allowing OneNote and other
applications to discover and use the add-in's COM objects. For details on
planned features and improvements, see the
[GitHub issues](https://github.com/knutkj/onenote-debug-add-in/issues) for this
repository
