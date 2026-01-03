# The `DllClass.cpp` File

This file implements the core COM requirements for the DLL. While `DllMain`
handles the DLL lifecycle and `DllReg` handles registration, `DllClass.cpp`
provides the machinery that allows Windows to actually create and use the
objects contained in the DLL.

## The "Engine" of the COM Server

For a DLL to function as a COM server, it must export two specific functions
that the COM runtime calls:

1.  **`DllGetClassObject`**: The entry point for creating objects.
2.  **`DllCanUnloadNow`**: The mechanism for memory management.

It must also implement the **Class Factory**, a standard COM object responsible
for manufacturing other objects.

## `DllGetClassObject`

```cpp
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
```

When a client application (like OneNote) wants to use your add-in, it calls
`CoCreateInstance`. Windows looks up your DLL in the registry, loads it, and
calls `DllGetClassObject`.

- **`rclsid`**: The Class ID (CLSID) of the object the client wants to create.
  If your DLL supports multiple types of objects, you check this ID to decide
  which factory to return.
- **`riid`**: The Interface ID (IID) the client wants to use to talk to the
  factory (usually `IID_IClassFactory`).
- **`ppv`**: The output parameter where you return the pointer to your Class
  Factory.

## `DllCanUnloadNow`

```cpp
extern "C" HRESULT __stdcall DllCanUnloadNow()
```

COM is designed to be efficient. Periodically, Windows asks the DLL if it is
safe to unload it from memory to free up resources.

- **Returns `S_OK`**: "Yes, I am not doing anything. You can unload me." (i.e.,
  no objects are active, and the server is not locked).
- **Returns `S_FALSE`**: "No, I am still in use. Keep me loaded."

This is typically tracked using a global counter (`g_nComObjsInUse`) that
increments whenever an object is created and decrements when one is destroyed.

## The Class Factory (`IClassFactory`)

The Class Factory is a standard COM object that implements the `IClassFactory`
interface. Its sole purpose is to create instances of your actual COM object
(the OneNote add-in).

### `CreateInstance`

```cpp
STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
```

This is the manufacturing method. When called, it:

1.  Allocates memory for a new instance of your actual COM object.
2.  Queries the new object for the requested interface (`riid`).
3.  Returns the interface pointer to the client.

### `LockServer`

```cpp
STDMETHODIMP LockServer(BOOL fLock)
```

Allows a client to keep the DLL in memory even if no objects are currently
active. This increments or decrements the global lock count.

## `IUnknown`

You will notice that `CClassFactory` implements `IUnknown`. Every COM object
must implement this interface, which provides three methods:

1.  **`QueryInterface`**: "Do you speak this language?" (Casts the object to a
    different interface).
2.  **`AddRef`**: "I am using you." (Increments reference count).
3.  **`Release`**: "I am done with you." (Decrements reference count; deletes
    object when count hits zero).
