#include "ProcessObject.h"

#include <winbase.h>
#include <iostream>
#include <wchar.h>
#include <cstring>



Process::Process(std::wstring path, LPSECURITY_ATTRIBUTES processattr, LPSECURITY_ATTRIBUTES processthreadattr, LPSTARTUPINFO startupinfo, DWORD createflags, LPPROCESS_INFORMATION processinfo, DWORD timeout) : m_dbgTool(L"CLASS--PROCESS") {
	m_dbgTool.SetObjID(path);

	m_processPath = path;

	if (processattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		processattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		processattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		processattr->lpSecurityDescriptor = NULL;
		processattr->bInheritHandle = true;
	}
	m_pProcessAttributes = processattr;

	if (processthreadattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		processthreadattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		processthreadattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		processthreadattr->lpSecurityDescriptor = NULL;
		processthreadattr->bInheritHandle = true;
	}
	m_pProcessThreadAttributes = processthreadattr;

	if (startupinfo == (LPSTARTUPINFOW)Process::DEFAULT_PARAM) {
		startupinfo = (LPSTARTUPINFOW)malloc(sizeof(STARTUPINFOW));
		ZeroMemory(startupinfo, sizeof(STARTUPINFOW));
		startupinfo->cb = sizeof(STARTUPINFOW);
	}
	m_pStartupInfo = startupinfo;

	if (processinfo == (LPPROCESS_INFORMATION)Process::DEFAULT_PARAM) {
		processinfo = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
		ZeroMemory(processinfo, sizeof(PROCESS_INFORMATION));
	}
	m_processInfo = processinfo;

	if (createflags == (DWORD)Process::DEFAULT_PARAM) {
		createflags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;

	}
	m_createFlags = createflags;

	wchar_t pathname[MAX_PATH];
	wcscpy_s(pathname, path.c_str());

	BOOL bopenprocess = CreateProcessW(NULL, pathname, processattr, processthreadattr, TRUE, createflags, NULL, NULL, startupinfo, processinfo);
	if (!bopenprocess) { m_dbgTool.PrintConsole("CreateProcess failed with error code: %i", GetLastError()); m_bIsBadInstance = true; return; }
	m_dwProcessID = processinfo->dwProcessId;

	HANDLE processhandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, processinfo->dwProcessId);
	if (!processhandle) { m_dbgTool.PrintConsole("OpenProcess failed with error code: %i", GetLastError()); m_bIsBadInstance = true; return; }
	WaitForSingleObject(processhandle, timeout);

	m_hProcess = processhandle;
}

Process::Process(CreateProcessArguments p) : m_dbgTool(L"CLASS--PROCESS") {
	m_dbgTool.SetObjID(p.path);
	m_processPath = p.path;
	if (p.processattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		p.processattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		p.processattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		p.processattr->lpSecurityDescriptor = NULL;
		p.processattr->bInheritHandle = true;
	}
	m_pProcessAttributes = p.processattr;

	if (p.processthreadattr == (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM) {
		p.processthreadattr = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
		p.processthreadattr->nLength = sizeof(SECURITY_ATTRIBUTES);
		p.processthreadattr->lpSecurityDescriptor = NULL;
		p.processthreadattr->bInheritHandle = true;
	}
	m_pProcessThreadAttributes = p.processattr;

	if (p.startupinfo == (LPSTARTUPINFOW)Process::DEFAULT_PARAM) {
		p.startupinfo = (LPSTARTUPINFOW)malloc(sizeof(STARTUPINFOW));
		ZeroMemory(p.startupinfo, sizeof(STARTUPINFOW));
		p.startupinfo->cb = sizeof(STARTUPINFOW);
	}
	m_pStartupInfo = p.startupinfo;

	if (p.processinfo == (LPPROCESS_INFORMATION)Process::DEFAULT_PARAM) {
		p.processinfo = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
		ZeroMemory(p.processinfo, sizeof(PROCESS_INFORMATION));
	}
	m_processInfo = p.processinfo;

	if (p.createflags == (DWORD)Process::DEFAULT_PARAM) {
		p.createflags = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP;
	}
	m_createFlags = p.createflags;

	wchar_t pathname[MAX_PATH];
	wcscpy_s(pathname, p.path.c_str());

	BOOL bopenprocess = CreateProcessW(NULL, pathname, p.processattr, p.processthreadattr, TRUE, p.createflags, NULL, NULL, p.startupinfo, p.processinfo);
	if (!bopenprocess) { m_dbgTool.PrintConsole("CreateProcess failed with error code: %i", GetLastError()); m_bIsBadInstance = true; return; }
	m_dwProcessID = p.processinfo->dwProcessId;

	HANDLE processhandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, p.processinfo->dwProcessId);
	if (!processhandle) { m_dbgTool.PrintConsole("OpenProcess failed with error code: %i", GetLastError()); m_bIsBadInstance = true; return; }
	WaitForSingleObject(processhandle, p.timeout);
	m_hProcess = processhandle;
}

Process::~Process() {
	CloseHandle(m_hProcess);
}

HANDLE Process::GetProcessHandle() {
	return m_hProcess;
}

BOOL Process::CloseProcess() {
	// wait how do I do this???
	return TRUE;
}