#pragma once
#include <string>

// Ideas: print in queue (errors from certain classes/functions are grouped together
//		  print as messages come

// used to assist with debugging (print functions)
class DebugTools {
private:
	std::wstring m_scope;
	std::wstring m_objID;
public:
	DebugTools(std::wstring scope) : m_scope(scope), m_objID(L"NULL") {}
	~DebugTools();

	void SetObjID(std::wstring objID);

	void PrintConsole(std::wstring message);

	void PrintWindow();
};