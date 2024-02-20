#pragma once
#include <string>
#include <windows.h>

#include "DebugTool.h"

class Process {
private:
	HANDLE m_hprocess;
	std::wstring m_ProcessPath;
	LPSECURITY_ATTRIBUTES m_pProcessAttributes, m_pProcessThreadAttributes;
	LPSTARTUPINFOW m_pStartupInfo;
	LPPROCESS_INFORMATION m_Processinfo;

	DWORD m_CreateFlags;

	BOOL m_IsBadInstance;

	DebugTools m_dbgTool;
public:
	enum {DEFAULT_PARAM=NULL};

	Process(std::wstring path, LPSECURITY_ATTRIBUTES processattr, LPSECURITY_ATTRIBUTES processthreadattr, LPSTARTUPINFO startupinfo, DWORD createflags, LPPROCESS_INFORMATION processinfo);
	Process(std::wstring processname);
	~Process();

	// move constructor
	Process(Process&& p) = delete;

	// copy constructor
	Process operator=(Process& p);

	BOOL OpenProcess();
	BOOL CloseProcess();

	HANDLE GetProcessHandle();

	DWORD GetProcessID();

	BOOL IsBadInstance() { return m_IsBadInstance; }
};