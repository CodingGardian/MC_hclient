#include "ProcessObject.h"

#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <wchar.h>
#include <cstring>

Process::Process(std::wstring path, LPSECURITY_ATTRIBUTES processattr, LPSECURITY_ATTRIBUTES processthreadattr, LPSTARTUPINFO startupinfo, DWORD createflags, LPPROCESS_INFORMATION processinfo) {
	m_dbgTool.SetObjID(path);

	if (processattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		processattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		processattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		processattr->lpSecurityDescriptor = NULL;
		processattr->bInheritHandle = true;
	}

	if (processthreadattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		processthreadattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		processthreadattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		processthreadattr->lpSecurityDescriptor = NULL;
		processthreadattr->bInheritHandle = true;
	}

	if (startupinfo == (LPSTARTUPINFOW)Process::DEFAULT_PARAM) {
		startupinfo = (LPSTARTUPINFOW)malloc(sizeof(STARTUPINFOW));
		ZeroMemory(startupinfo, sizeof(STARTUPINFOW));
		startupinfo->cb = sizeof(STARTUPINFOW);
	}

	if (processinfo == (LPPROCESS_INFORMATION)Process::DEFAULT_PARAM) {
		processinfo = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
		ZeroMemory(processinfo, sizeof(PROCESS_INFORMATION));
	}


}

Process::~Process() {
	
}