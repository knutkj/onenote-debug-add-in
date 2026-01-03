# OneNote Debug Add-In: Quick Summary

This project is a foundational OneNote add-in built in native C++, designed for
learning COM and DLL development.

## Core Components

### [DllMain.cpp](src/DllMain.cpp)

The DLL entry point. Windows calls `DllMain` during lifecycle events:

- **DLL_PROCESS_ATTACH**: Initialization when loaded.
- **DLL_PROCESS_DETACH**: Cleanup when unloaded.
- **DLL_THREAD_ATTACH/DETACH**: Thread-specific notifications. It uses the
  `APIENTRY` (`__stdcall`) calling convention as required by the Windows loader.

### [DllReg.cpp](src/DllReg.cpp)

Handles COM registration via `DllRegisterServer`.

- **COM Registration**: Allows OneNote to discover the add-in.
- **Export Table**: The `DllRegisterServer` function is exposed via
  [DebugAddIn.def](src/DebugAddIn.def) so tools like `regsvr32.exe` can find it.
- **Linkage**: Uses `extern "C"` to prevent name mangling, ensuring
  compatibility with system tools.

## Project Status

The DLL currently builds to `out/x64/Debug/DebugAddIn.dll`. It is a "no-op"
foundation: `regsvr32.exe` can call the registration function, but no registry
keys are written yet. This provides a clean starting point for implementing
actual COM logic.
