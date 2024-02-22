#pragma once
#include <string>
#include <windows.h>

#include "DebugTool.h"

class Process {
private:
	HANDLE m_hProcess;
	std::wstring m_processPath;
	LPSECURITY_ATTRIBUTES m_pProcessAttributes, m_pProcessThreadAttributes;
	LPSTARTUPINFOW m_pStartupInfo;
	LPPROCESS_INFORMATION m_processInfo;

	DWORD m_createFlags;

	BOOL m_bIsBadInstance;

	DebugTools m_dbgTool;

	DWORD m_dwProcessID;
public:
	struct CreateProcessArguments {
		std::wstring path;
		LPSECURITY_ATTRIBUTES processattr;
		LPSECURITY_ATTRIBUTES processthreadattr;
		LPSTARTUPINFO startupinfo;
		DWORD createflags;
		LPPROCESS_INFORMATION processinfo;
		DWORD timeout;
	};

	enum {DEFAULT_PARAM=NULL};

	// create a new process
	Process(std::wstring path, LPSECURITY_ATTRIBUTES processattr, LPSECURITY_ATTRIBUTES processthreadattr, LPSTARTUPINFO startupinfo, DWORD createflags, LPPROCESS_INFORMATION processinfo, DWORD timeout);
	Process(CreateProcessArguments p);


	// search for process ID and get handle
	Process(std::wstring processname);

	~Process();

	// move constructor
	Process(Process&& p) = delete;

	// copy constructor
	Process operator=(Process& p) = delete;

	//BOOL OpenProcess();
	BOOL CloseProcess();

	HANDLE GetProcessHandle();

	DWORD GetProcessID();

	BOOL IsBadInstance() { return m_bIsBadInstance; }
};