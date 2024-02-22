#include <windows.h>
#include <winbase.h>
#include <iostream>
#include <wchar.h>
#include <cstring>

#include "Injection/DLLInjector.h"
#include "Utilities/ProcessObject.h"

#define MAX_NAME 100

// RULE 1: use wide chars (LPWSTR & s std::wstring)
// RULE 2: KEEP TRACK OF PROINTERS, ONLY USE WINDOWS LP DEFINES WHEN THE VARIABLE POINTS TO DATA, NOT POINTERS!!!!!!

// TODO:
//    Finish DLLInjector functions & destructor (unload injected libraries?)
//    Make User Interface
//    Create some usable injection code
//    Maybe some default applications to inject into?

const wchar_t* GetWC(const char* c)
{
	const size_t cSize = (strlen(c) + 1)/2;
	wchar_t* wc = new wchar_t[cSize];

	size_t* ret = new size_t;
	mbstowcs_s(ret, wc, cSize, c, cSize*2);

	return wc;
}

int main() {


	Process TestProgram(L"C:\\Projects\\MC_hclient\\MC_hclient\\x64\\Release\\TestProgram.exe", (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM, (LPSECURITY_ATTRIBUTES)Process::DEFAULT_PARAM, (LPSTARTUPINFO)Process::DEFAULT_PARAM, (DWORD)Process::DEFAULT_PARAM, (LPPROCESS_INFORMATION)Process::DEFAULT_PARAM, INFINITY);

	DLLInjector testinjector(TestProgram.GetProcessHandle(), L"C:\\Projects\\MC_hclient\\MC_hclient\\x64\\Release\\TestDLL.dll", DLLInjector::REMOTETHREAD, L"TestPrg");
	if (testinjector.IsBadInstance()) { return -1; }


	return 0;
}