#ifndef STRINGUTIL_H
#define STRINGUTIL_H
#include <string>
#include <utility>

class CEWString : public std::wstring {
private:
public:
	CEWString(const wchar_t* str) : std::wstring(str) {}
	//CEWString(std::wstring str) : std::wstring(str) {}

	CEWString(const std::wstring& str) : std::wstring(str) { }

	CEWString(std::wstring&& str) : std::wstring(std::move(str)) { }
	
	std::wstring& TruncateFileFromPath();

	const std::wstring& GetWString() { return *static_cast<std::wstring*>(this); };

	CEWString& operator=(const std::wstring& str) { std::wstring::operator=(str); return *this; }
	//CEWString& operator=(const CEWString& str) { std::wstring::operator=(str.GetWString()); return *this; }

	std::string GetString();

	bool IsFilePath();

	inline float GetFloat() { 
		size_t i = 0; float returnVal = std::stof(this->GetWString(), &i); 
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetFloat function parsed unexpected number of characters for string: %ls as float: %f", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline double GetDouble() {
		size_t i = 0; double returnVal = std::stod(this->GetWString(), &i);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetDouble function parsed unexpected number of characters for string: %ls as double: %d", std::wstring::c_str(), returnVal); */ }
		return returnVal;
	}

	inline int GetInt() {
		size_t i = 0; int returnVal = std::stoi(this->GetWString(), &i);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetInt function parsed unexpected number of characters for string: %ls as int: %i", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline uint32_t GetUInt() {
		size_t i = 0; uint32_t returnVal = std::stoi(this->GetWString(), &i);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetUInt function parsed unexpected number of characters for string: %ls as uint: %u", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline int64_t GetInt64() {
		size_t i = 0; int64_t returnVal = std::stoll(this->GetWString(), &i);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetInt64 function parsed unexpected number of characters for string: %ls as int64: %ll", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline uint64_t GetUInt64() {
		size_t i = 0; uint64_t returnVal = std::stoull(this->GetWString(), &i);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetUInt64 function parsed unexpected number of characters for string: %ls as uint64: %ull", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}

	
	inline void* GetPointer() {//                                              base 16, hexadecimal
		size_t i = 0; uint64_t returnVal = std::stoull(this->GetWString(), &i, 16);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetDouble function parsed unexpected number of characters for string: %ls as pointer: %p", std::wstring::c_str(), returnVal);*/ }
		return reinterpret_cast<void*>(returnVal);
	}

	inline uint64_t GetHex64() {
		size_t i = 0; uint64_t returnVal = std::stoull(this->GetWString(), &i, 16);
		if (i != std::wstring::length()) { /*m_dbgTool.PrintConsole(L"GetHex64 function parsed unexpected number of characters for string: %ls as hex value: %llx", std::wstring::c_str(), returnVal);*/ }
		return returnVal;
	}
};

class CEString : public std::string {
private:
public:
	CEString(const char* str) : std::string(str) {}
	//CEWString(std::wstring str) : std::wstring(str) {}

	CEString(const std::string& str) : std::string(str) { }

	CEString(std::string&& str) : std::string(std::move(str)) { }

	std::string& TruncateFileFromPath();

	const std::string& GetString() { return *static_cast<std::string*>(this); };

	CEString& operator=(const std::string& str) { std::string::operator=(str); return *this; }

	std::wstring GetWString();

	bool IsFilePath();

	inline float GetFloat() {
		size_t i = 0; float returnVal = std::stof(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetFloat function parsed unexpected number of characters for string: %ls as float: %f", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline double GetDouble() {
		size_t i = 0; double returnVal = std::stod(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetDouble function parsed unexpected number of characters for string: %ls as double: %d", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline int GetInt() {
		size_t i = 0; int returnVal = std::stoi(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetInt function parsed unexpected number of characters for string: %s as int: %i", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline uint32_t GetUInt() {
		size_t i = 0; uint32_t returnVal = std::stoi(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetUInt function parsed unexpected number of characters for string: %s as uint: %u", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline int64_t GetInt64() {
		size_t i = 0; int64_t returnVal = std::stoll(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetInt64 function parsed unexpected number of characters for string: %s as int64: %ll", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}

	inline uint64_t GetUInt64() {
		size_t i = 0; uint64_t returnVal = std::stoull(this->GetString(), &i);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetUInt64 function parsed unexpected number of characters for string: %s as uint64: %ull", std::string::c_str(), returnVal);*/ }
		return returnVal;
	}


	inline void* GetPointer() {//                                              base 16, hexadecimal
		size_t i = 0; uint64_t returnVal = std::stoull(this->GetString(), &i, 16);
		if (i != std::string::length()) { /*m_dbgTool.PrintConsole(L"GetDouble function parsed unexpected number of characters for string: %s as pointer: %p", std::string::c_str(), returnVal);*/ }
		return reinterpret_cast<void*>(returnVal);
	}

};



extern std::string GetStringFromWString(const std::wstring& str);

extern std::wstring GetWStringFromString(const std::string& str);

extern bool IsFilePath(const std::wstring& str);
extern bool IsFilePath(const std::string& str);

#endif