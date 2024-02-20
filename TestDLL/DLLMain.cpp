#include <windows.h>
#include <iostream>

// yeah yeah I know globals bad >:(
bool bexitthread;

#define DLLEXPORT __declspec(dllexport)

VOID Threadfunc(LPVOID);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    LPSECURITY_ATTRIBUTES threadattributes = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
    threadattributes->nLength = sizeof(SECURITY_ATTRIBUTES);
    threadattributes->lpSecurityDescriptor = NULL;
    threadattributes->bInheritHandle = false;
    
    LPDWORD threadid = 0;

    HANDLE hthread = 0;

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        bexitthread = true;
        hthread = CreateThread(threadattributes, 0, (LPTHREAD_START_ROUTINE)Threadfunc, 0, 0, threadid);
        if (!hthread) { MessageBoxW(NULL, L"Create Thread failed", L"Error", MB_ICONINFORMATION); }
        else { MessageBoxW(NULL, L"Created Thread!", L"Notice", MB_ICONINFORMATION); }
        return TRUE;
        break;
    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }
        
        // Perform any necessary cleanup.
        if (hthread != NULL) { MessageBoxW(NULL, L"Deleting thread", L"Notice", MB_ICONINFORMATION); bexitthread = true; WaitForSingleObject(hthread, INFINITE); }
        return TRUE;
        break;
    }
    
}

VOID Threadfunc(LPVOID params) {
    MessageBoxW(NULL, L"Injected code is running", L"Hello World!", MB_ICONINFORMATION);
    while (true) {
        if (bexitthread) { ExitThread(1); }
    }
}