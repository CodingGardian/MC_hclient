#pragma once
#include <string>
#include <utility>

class CEWString : std::wstring {
private:

public:
	CEWString(const wchar_t* str) : std::wstring(str) {}
	CEWString(std::wstring str) : std::wstring(str) {}

	CEWString(const std::wstring& str) : std::wstring(str) { }

	CEWString(std::wstring&& str) : std::wstring(str) { }
	
	std::wstring& TruncateFileFromPath();
};

class CEString : std::string {
private:

public:
	CEString(const char* str) : std::string(str) {}
	CEString(std::string str) : std::string(str) {}

	// calls the move constructor for std::string I think? (I have no idea what I'm doing)
	CEString(const std::string& str) : std::string(str) { }
	CEString(std::string&& str) : std::string(str) { }

	std::string& TruncateFileFromPath();

};
