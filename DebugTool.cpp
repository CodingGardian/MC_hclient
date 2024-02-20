#include "DebugTool.h"
#include <iostream>

void DebugTools::PrintConsole(std::wstring message) {
	std::wcout << L"Error from scope: " << m_scope << L"Object ID: " << m_objID << L", " << message << std::endl;
}
