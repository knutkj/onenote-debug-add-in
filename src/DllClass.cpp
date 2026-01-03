#include <windows.h>

// Global reference count for the DLL.
// Used by DllCanUnloadNow to determine if the DLL can be unloaded.
extern long g_nComObjsInUse;

// Forward declaration of the Class Factory
class CClassFactory : public IClassFactory {
public:
    // IUnknown methods
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IClassFactory methods
    STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject);
    STDMETHODIMP LockServer(BOOL fLock);

    CClassFactory();
protected:
    ~CClassFactory();
private:
    long m_nRefCount;
};

//
// DllGetClassObject
//
// Purpose:
//   Called by the COM runtime (CoGetClassObject) to get the class factory
//   object for a specific CLSID.
//
extern "C" HRESULT __stdcall DllGetClassObject(
    REFCLSID rclsid,
    REFIID riid,
    LPVOID* ppv
) {
    // 1. Validate arguments (ppv cannot be NULL).
    // 2. Check if rclsid matches our supported CLSID.
    // 3. Create a new instance of CClassFactory.
    // 4. QueryInterface on the factory for the requested interface (riid).
    // 5. Return result.
    
    // Skeleton return for now
    return CLASS_E_CLASSNOTAVAILABLE;
}

//
// DllCanUnloadNow
//
// Purpose:
//   Called by the COM runtime to determine if the DLL can be unloaded from memory.
//
extern "C" HRESULT __stdcall DllCanUnloadNow() {
    // If there are no objects in use and the server is not locked, return S_OK.
    // Otherwise, return S_FALSE.
    if (g_nComObjsInUse == 0) {
        return S_OK;
    }
    return S_FALSE;
}

//
// CClassFactory Implementation Skeleton
//

CClassFactory::CClassFactory() : m_nRefCount(1) {
    // Increment global DLL ref count
}

CClassFactory::~CClassFactory() {
    // Decrement global DLL ref count
}

STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void** ppvObject) {
    // Standard IUnknown implementation
    return E_NOTIMPL;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef() {
    return InterlockedIncrement(&m_nRefCount);
}

STDMETHODIMP_(ULONG) CClassFactory::Release() {
    long count = InterlockedDecrement(&m_nRefCount);
    if (count == 0) {
        delete this;
    }
    return count;
}

STDMETHODIMP CClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) {
    // Create the actual COM object here
    return E_NOTIMPL;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock) {
    // Lock/Unlock the server
    return E_NOTIMPL;
}
