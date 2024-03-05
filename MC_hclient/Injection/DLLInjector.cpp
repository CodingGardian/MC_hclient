#include "DLLInjector.h"
#include <psapi.h>
#include <charconv>
#include <iostream>
// two different ways of injecting DLL code
// Creating remote threads
// Windows hooks

struct InjectorArgs {
	LPVOID LoadLibFuncPtr;
	LPVOID PathWChar;
};

#define LOADFUNC_SIZE 100
DWORD InjectorFunc(LPVOID* args) {
	typedef HMODULE(*LoadLibFunc)(LPCWSTR);
	LoadLibFunc LoadLibraryFunc = (LoadLibFunc)(*args);
	LPCWSTR* pdllpath = (LPCWSTR*)((char*)args + sizeof(LPVOID));

	
	//LoadLibrary is a part of kernel32.dll, so it's address is the same for EVERY address space
	HMODULE lib = LoadLibraryFunc(*pdllpath);

	return lib == NULL;
}

std::vector<DLLInjector*> DLLInjector::Injector_Table;

DLLInjector::DLLInjector(HANDLE process, std::wstring DLLPath, InjectorType t, std::wstring id) {
	if (t == REMOTETHREAD) {
		/*HMODULE locallib = LoadLibraryA(DLLPath_winstr);

		MODULEINFO locallib_info;
		GetModuleInformation(THIS_INSTANCE, locallib, &locallib_info, sizeof(MODULEINFO));*/
		
		// just allocate an arbitrary number of bytes because it's impossible to know the size of the function code
		LPVOID remoteptr = VirtualAllocEx(process, NULL, LOADFUNC_SIZE + sizeof(InjectorArgs) + MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!remoteptr) {
			m_bBadInstance = true;
			std::wcout << L"Fail at VirtualAllocEx, Error code: " << GetLastError() << std::endl;
			return;
		}
		
		SIZE_T numbyteswritten=0;
		// write function code to remote process
		//LPTHREAD_START_ROUTINE testf = InjectorFunc;

		if (!WriteProcessMemory(process, remoteptr, InjectorFunc, LOADFUNC_SIZE, &numbyteswritten) || numbyteswritten != LOADFUNC_SIZE) {
			m_bBadInstance = true;
			std::wcout << L"Fail at WriteProcessMemory for injector function, Error code: " << GetLastError() << std::endl;
			return;
		}

		// get path of dll from perspective of process
		std::wstring fullpath = DLLPath;

		std::wcout << fullpath.c_str() << std::endl;
		if (fullpath.length() > MAX_PATH) {
			m_bBadInstance = true;
			std::wcout << L"Path of injected DLL longer than MAX_PATH, EXITING" << std::endl;
			return;
		}
		
		InjectorArgs args;
		HMODULE hk32 = LoadLibraryW(L"kernel32.dll");
		if (!hk32) { std::wcout << L"Fail at WriteProcessMemory for loading kernel32.dll, Error code: " << GetLastError() << std::endl; return; }
		args.LoadLibFuncPtr = (LPTHREAD_START_ROUTINE)GetProcAddress(hk32, "LoadLibraryW");
		if (!args.LoadLibFuncPtr) { std::wcout << L"Fail when getting address of LoadLibraryW from kernel32.dll, Error code: " << GetLastError() << std::endl; return; }

		// write path to remote process
		LPVOID paramptr = (char*)remoteptr + LOADFUNC_SIZE + sizeof(InjectorArgs);
		if (!WriteProcessMemory(process, paramptr, fullpath.data(), (fullpath.length()*sizeof(wchar_t)), &numbyteswritten) || numbyteswritten != (fullpath.length()*sizeof(wchar_t)) ) {
			m_bBadInstance = true;
			std::wcout << L"Fail at WriteProcessMemory for thread paramaters DLL path, Error code: " << GetLastError() << std::endl;
			return;
		}

		args.PathWChar = paramptr;
		LPVOID iargsheader = (char*)remoteptr + LOADFUNC_SIZE;
		if (!WriteProcessMemory(process, iargsheader, &args, sizeof(InjectorArgs), &numbyteswritten) || numbyteswritten != sizeof(InjectorArgs)) {
			m_bBadInstance = true;
			std::wcout << L"Fail at WriteProcessMemory for thread parameters header, Error code: " << GetLastError() << std::endl;
			return;
		}
		
		
		LPDWORD remotethreadID=0;
		HANDLE hremotethread;
		LPSECURITY_ATTRIBUTES pthreadattributes = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		pthreadattributes->nLength = sizeof(SECURITY_ATTRIBUTES);
		pthreadattributes->lpSecurityDescriptor = NULL;
		pthreadattributes->bInheritHandle = false;
		if ( !( hremotethread = CreateRemoteThread(process, pthreadattributes, 0, (LPTHREAD_START_ROUTINE)remoteptr, (LPVOID)iargsheader, 0, remotethreadID))) {
			m_bBadInstance = true;
			std::wcout << L"Fail at CreateRemoteThread, Error code: " << GetLastError() << std::endl;
			return;
		}

		WaitForSingleObject(hremotethread, INFINITE);
		//CloseHandle(hremotethread);
		// todo: change this late

		DWORD exitcode;
		if (GetExitCodeThread(hremotethread, &exitcode)) {
			std::cout << exitcode << " " << (exitcode == EXIT_SUCCESS) << std::endl;
		}
		else {
			std::cout << GetLastError() << std::endl;
		}

		if (m_ID == L"") { m_ID = DLLPath; }
		else { m_ID = id; }

		m_DLLPath = DLLPath;

		m_process = process;

		m_bLoaded = true;

		//m_RemoteThreadID = remotethreadID;
		//m_hRemoteThread = hremotethread;
		

		free(pthreadattributes);
		Injector_Table.push_back(this);
		m_bBadInstance = false;
		return;
	}
	else if (t == WINDOWSHOOK) {
		m_bBadInstance = true;
	}
	m_bBadInstance = true;
}




DLLInjector::~DLLInjector() {
	//CloseHandle(m_hRemoteThread);
	
}