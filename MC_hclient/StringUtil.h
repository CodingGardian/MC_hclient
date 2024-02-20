#pragma once
#include <string>
#include <utility>


class CEWString : std::wstring {
private:

public:
	CEWString(const wchar_t* str) : std::wstring(str) {}
	CEWString(std::wstring str) : std::wstring(str) {}
	
	std::wstring TruncateFileFromPath();
	std::string ToString();


};

class CEString : std::string {

};
