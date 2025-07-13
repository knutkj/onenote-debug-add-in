# The `DllReg.cpp` File

This document builds on concepts from [`DllMain.md`](DllMain.md), and introduces
[the Component Object Model (COM)][com] in the context of DLL registration. COM
is a Microsoft technology for enabling interoperability between software
components. In this file, we focus on how a COM DLL is registered with the
system using the [`DllRegisterServer` function][thefunc]. The
`DllRegisterServer` function is exported from the DLL and can be called by
external tools (such as `regsvr32.exe`) or other programs using Windows APIs.
These tools locate and invoke the function by name through the DLL's export
table, allowing the COM DLL to register itself with the system so its COM
objects can be discovered and used by other applications.

## Component Object Model (COM)

The Component Object Model (COM) is a Microsoft technology for enabling
interoperability between software components. COM allows objects created in
different languages and processes to communicate through well-defined
interfaces. It is widely used in Windows for building reusable libraries,
automation, and system-level features such as ActiveX controls and shell
extensions. Understanding COM is useful for developing advanced Windows
applications and integrating with the operating system.

## The Export Table

When you build a DLL, a special program called the _linker_ creates a data
structure inside the DLL file called the _export table_. The linker is a tool
that runs after the compiler and combines all the pieces of your program (object
files) into a single DLL file. It also decides which functions and variables
will be made available to other programs.

The export table is a list of all functions and variables that are made
available for use by other programs. Only symbols explicitly exported—either by
marking them with `__declspec(dllexport)` in your code or by listing them in a
module definition file (`.def`)—are included in this table.

The module definition file (`.def`) gives you fine-grained control over which
symbols are exported. It contains sections like `LIBRARY` (naming the DLL) and
`EXPORTS` (listing the functions to export). During the build process, the
linker reads the `.def` file and adds the specified functions to the export
table. This is especially important for COM DLLs, where Windows tools (such as
`regsvr32.exe`) and other programs need to locate and call specific entry points
like `DllRegisterServer` by name.

If a function is not exported, it will not appear in the export table and cannot
be called by other programs. Exporting is what makes a function discoverable:
when another program loads your DLL, it can look up the export table to find the
address of exported functions and call them directly.

In summary:

- The export table is the mechanism that exposes selected functions and
  variables from your DLL to the outside world.
- The `.def` file (or `__declspec(dllexport)`) controls which symbols are
  exported.
- The linker processes these instructions and builds the export table.
- Only exported symbols are visible and callable by other programs or system
  tools.

This process ensures that your DLL presents a clean, well-defined interface,
with only the necessary entry points exposed for use by others.

### The `DebugAddIn.def` File

The `DebugAddIn.def` file is the module definition file that controls which
functions are exported from the DebugAddIn DLL. We are introducing this file
together with the `DllReg.cpp` file since the `DllRegisterServer` function must
be. It is marked as `PRIVATE`. The `PRIVATE` keyword restricts visibility so
that only system tools (like `regsvr32.exe`) can access it, rather than making
it available to all programs. This is important for COM DLLs, where you want to
expose only the required entry points and keep internal functions hidden.

If you do not list a function in the `EXPORTS` section, it will not be added to
the export table and cannot be called by other programs. For COM registration,
it is essential that `DllRegisterServer` is exported, otherwise registration
will fail because Windows cannot locate the entry point.

#### The special case of `DllMain`

Unlike other functions, `DllMain` is a special entry point that Windows looks
for by its well-known name and signature when loading or unloading a DLL. The
Windows loader scans the DLL for a function named `DllMain` with the correct
signature, even if it is not listed in the export table. This is handled
internally by the operating system and does not require the function to be
exported. Other programs cannot call `DllMain` directly; only Windows itself
invokes it as part of the DLL lifecycle.

This is why you should not list `DllMain` in your `.def` file or mark it for
export—it is not meant to be called from outside the DLL, and Windows will
always find and call it as needed.

## Signature

```cpp
extern "C" HRESULT __stdcall DllRegisterServer()
```

- **Linkage Specifier (`extern "C"`)**: Ensures the function uses C linkage,
  disabling C++ name mangling. This is required so Windows tools (like
  `regsvr32.exe`) can locate and call the function reliably.
- **Return Type (`HRESULT`)**: Windows status code type, used to indicate
  success or error. See [`DllMain.md`](DllMain.md) for more on return types.
- **Calling Convention (`__stdcall`)**: Standard calling convention for
  Windows/COM functions. Ensures parameters and stack are handled as expected by
  the OS. Unlike the DLL entry point `DllMain`, which is using the `APIENTRY`
  macro, COM functions such as `DllRegisterServer` do not use `APIENTRY`.
  Instead, they use the `__stdcall` calling convention explicitly, as shown
  above. This is the standard and required convention for COM registration
  functions.
- **Function Name (`DllRegisterServer`)**: Standard function name for COM DLL
  registration. Windows expects this exact name when registering COM servers.

Returns an `HRESULT` status code. Common values include `S_OK` (success) and
various error codes. Use standard COM error codes for consistency.

## Usage

`DllRegisterServer` is called by registration tools (such as `regsvr32.exe`) to
register the DLL as a COM server. The function should add necessary registry
entries so the system can locate and use the COM objects in this DLL. For simple
DLLs, this may be a stub; for production code, it typically writes registry keys
describing the COM classes, interfaces, and type libraries.

### The COM Server Terminology

A COM server is any executable (DLL or EXE) that implements one or more COM
objects—reusable software components that expose well-defined interfaces for
other programs to use. In the context of a DLL, a COM server provides these
objects to client applications via the Windows COM infrastructure. The server is
responsible for registering itself and its objects in the Windows registry so
clients can discover and instantiate them. COM servers are a foundational part
of Windows extensibility, enabling automation, inter-process communication, and
integration between applications and system components.

[thefunc]:
  https://learn.microsoft.com/en-us/windows/win32/api/olectl/nf-olectl-dllregisterserver
  "DllRegisterServer function (olectl.h)"
[com]:
  https://learn.microsoft.com/en-us/windows/win32/com/component-object-model--com--portal
  "Component Object Model (COM)"
