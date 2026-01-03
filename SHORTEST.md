# OneNote Debug Add-In

The OneNote Debug Add-In project is a native C++ development effort focused on
building a COM-based DLL from scratch to understand the low-level mechanics of
Windows extensibility. By implementing core entry points like `DllMain` for
lifecycle management and `DllRegisterServer` for COM registration, the project
establishes a functional foundation that can be loaded by OneNote and registered
via `regsvr32.exe`. Although currently a "no-op" implementation, it serves as a
clean educational template for mastering C++ linkage, calling conventions, and
the Windows export table within the context of Office add-in development.
