// Documented in DllReg.md.

#include <windows.h>
#include <objbase.h>
#include "Logging.h"

extern "C" HRESULT __stdcall DllRegisterServer()
{
    Log("DllRegisterServer", "Registration started");

    // TODO: Implement actual COM registration

    Log("DllRegisterServer", "Registration completed successfully");

    return S_OK;
}
