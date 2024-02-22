#include "Utilities/DebugTool.h"
#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#include <string>

void DebugTools::PrintConsole(const char* format, ...) {
	wprintf(L"Scope:%s Object ID:%s Message: ", m_scope.c_str(), m_objID.c_str());

	std::string temp(format);
	temp.append("\n");

	va_list vl;
	va_start(vl, temp.c_str());
	vfprintf(stdout, temp.c_str(), vl);
}

void DebugTools::PrintConsole(const wchar_t* format, ...) {
	wprintf(L"Scope:%s Object ID:%s Message: ", m_scope.c_str(), m_objID.c_str());
	
	std::wstring temp(format);
	temp.append(L"\n");

	va_list vl;
	va_start(vl, temp.c_str());
	vfwprintf(stdout, temp.c_str(), vl);
}

void DebugTools::SetObjID(std::wstring objID) {
	m_objID = objID;
}

void DebugTools::PrintWindow(std::wstring message) {

}