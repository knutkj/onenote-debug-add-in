// Documented in DllMain.md.

#include <windows.h>
#include "Globals.h"
#include "Logging.h"

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        LogProcess();
        Log(__func__, "DLL_PROCESS_ATTACH - DLL loaded into process");
        break;
    case DLL_THREAD_ATTACH:
        Log(__func__, "DLL_THREAD_ATTACH - New thread created in process");
        break;
    case DLL_THREAD_DETACH:
        Log(__func__, "DLL_THREAD_DETACH - Thread exiting cleanly");
        break;
    case DLL_PROCESS_DETACH:
        Log(__func__, "DLL_PROCESS_DETACH - DLL being unloaded from process");
        break;
    }

    return TRUE; // Successful DLL initialization
}
