#include "StringUtil.h"

std::wstring& CEWString::TruncateFileFromPath() {
	std::wstring::erase(std::wstring::find_last_of(L"\\", std::wstring::npos), std::wstring::npos);
	return *this;
}

std::string& CEString::TruncateFileFromPath() {
	std::string::erase(std::string::find_last_of("\\", std::string::npos), std::string::npos);
	return *this;
}