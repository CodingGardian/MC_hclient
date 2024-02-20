#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <wchar.h>
#include <cstring>
#include "DLLInjector.h"

#define MAX_NAME 100

// RULE 1: use wide chars (LPWSTR & s std::wstring)
// RULE 2: KEEP TRACK OF PROINTERS, ONLY USE WINDOWS LP DEFINES WHEN THE VARIABLE POINTS TO DATA, NOT POINTERS!!!!!!

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = (strlen(c) + 1)/2;
	wchar_t* wc = new wchar_t[cSize];

	size_t* ret = new size_t;
	mbstowcs_s(ret, wc, cSize, c, cSize*2);

	return wc;
}

int main() {
	LPSECURITY_ATTRIBUTES pProcessAttributes = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
	pProcessAttributes->nLength = sizeof(SECURITY_ATTRIBUTES);
	pProcessAttributes->lpSecurityDescriptor = NULL;
	pProcessAttributes->bInheritHandle = true;

	LPSECURITY_ATTRIBUTES pProcessThreadAttributes = (LPSECURITY_ATTRIBUTES)malloc(sizeof(SECURITY_ATTRIBUTES));
	pProcessThreadAttributes->nLength = sizeof(SECURITY_ATTRIBUTES);
	pProcessThreadAttributes->lpSecurityDescriptor = NULL;
	pProcessThreadAttributes->bInheritHandle = true;

	LPSTARTUPINFOW pStartUpInfo = (LPSTARTUPINFOW)malloc(sizeof(STARTUPINFOW));
	ZeroMemory(pStartUpInfo, sizeof(STARTUPINFOW));
	pStartUpInfo->cb = sizeof(STARTUPINFOW);
	/*wchar_t desktopname[MAX_NAME];
	DWORD dnSize=MAX_NAME;
	GetComputerNameW(desktopname, &dnSize);


	pStartUpInfo->lpDesktop = desktopname;
	wchar_t processwindowtitle[5];
	//LPCWSTR wintitle = L"Test";
	wcscpy_s(processwindowtitle, L"Test");
	pStartUpInfo->lpTitle = (LPWSTR)processwindowtitle;*/
	
	LPPROCESS_INFORMATION processinfo = (LPPROCESS_INFORMATION)malloc(sizeof(PROCESS_INFORMATION));
	ZeroMemory(processinfo, sizeof(PROCESS_INFORMATION));

	wchar_t pathname[256];
	wcscpy_s(pathname, L"C:\\Projects\\MC_hclient\\MC_hclient\\x64\\Release\\TestProgram.exe");

	BOOL bopenprocess = CreateProcessW(NULL, pathname, pProcessAttributes, pProcessThreadAttributes, TRUE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP, NULL, NULL, pStartUpInfo, processinfo);
	if (!bopenprocess) { std::cout << "CreateProcessW Failed!" << std::endl; return GetLastError(); }
	
	HANDLE processhandle = OpenProcess(PROCESS_ALL_ACCESS, false, processinfo->dwProcessId);
	if (!processhandle) { std::cout << "OpenProcess Failed! " << std::endl; return GetLastError(); }
	WaitForSingleObject(processhandle, INFINITY);
	DLLInjector testinjector(processhandle, L"C:\\Projects\\MC_hclient\\MC_hclient\\x64\\Release\\TestDLL.dll", DLLInjector::REMOTETHREAD, L"TestPrg");
	if (testinjector.IsBadInstance()) { return -1; }

	free(pProcessAttributes);

	CloseHandle(processhandle);
	return 0;
}