#pragma once
#include <string>
#include <vector>

// Ideas: print in queue (errors from certain classes/functions are grouped together
//		  print as messages come

// used to assist with debugging (print functions)
class DebugTools {
private:
	std::wstring m_scope;
	std::wstring m_objID;
public:
	DebugTools(std::wstring scope, std::wstring objID) : m_scope(scope), m_objID(m_objID) {}
	DebugTools(std::wstring scope) : m_scope(scope), m_objID(L"NULL") {}
	DebugTools() : m_scope(L"NULL"), m_objID(L"NULL") {}
	~DebugTools() {}

	inline void SetScope(std::wstring scope) { m_scope = scope; }
	inline void SetObjID(std::wstring objID) { m_objID = objID; }

	void PrintConsole(const char* format, ...);
	void PrintConsole(const wchar_t* format, ...);


	void PrintWindow(std::wstring message);
};