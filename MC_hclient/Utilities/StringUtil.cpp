#include "StringUtil.h"

std::wstring& CEWString::TruncateFileFromPath() {
	std::wstring::erase(std::wstring::find_last_of(L"\\", std::wstring::npos), std::wstring::npos);
	return *this;
}

std::string& CEString::TruncateFileFromPath() {
	std::string::erase(std::string::find_last_of("\\", std::string::npos), std::string::npos);
	return *this;
}

bool CEWString::IsFilePath() {
    if (std::wstring::find(L'\\') == std::wstring::npos) {
        return false;
    }
    
    return true;
}

bool CEString::IsFilePath() {
    if (std::string::find('\\') == std::string::npos) {
        return false;
    }

    return true;
}


std::string CEWString::GetString() {
    std::size_t size = sizeof(GetWString().c_str());
    char* str = new char[size];
    std::string temp;

    std::wcstombs(str, GetWString().c_str(), size);

    temp = str;
    delete[] str;

    return temp;
}

std::wstring CEString::GetWString() {
    size_t requiredSize = 0;
    std::wstring answer;
    wchar_t* pWTempString = NULL;

    /*
    * Call the conversion function without the output buffer to get the required size
    *  - Add one to leave room for the NULL terminator
    */
    requiredSize = mbstowcs(NULL, GetString().c_str(), 0) + 1;

    /* Allocate the output string (Add one to leave room for the NULL terminator) */
    pWTempString = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
    if (pWTempString == NULL)
    {
        printf("Memory allocation failure.\n");
    }
    else
    {
        // Call the conversion function with the output buffer
        size_t size = mbstowcs(pWTempString, GetString().c_str(), requiredSize);
        if (size == (size_t)(-1))
        {
            printf("Couldn't convert string\n");
        }
        else
        {
            answer = pWTempString;
        }
    }


    if (pWTempString != NULL)
    {
        delete[] pWTempString;
    }

    return answer;
}

std::wstring GetWStringFromString(const std::string& str) {
    size_t requiredSize = 0;
    std::wstring answer;
    wchar_t* pWTempString = NULL;

    /*
    * Call the conversion function without the output buffer to get the required size
    *  - Add one to leave room for the NULL terminator
    */
    requiredSize = mbstowcs(NULL, str.c_str(), 0) + 1;

    /* Allocate the output string (Add one to leave room for the NULL terminator) */
    pWTempString = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
    if (pWTempString == NULL)
    {
        printf("Memory allocation failure.\n");
    }
    else
    {
        // Call the conversion function with the output buffer
        size_t size = mbstowcs(pWTempString, str.c_str(), requiredSize);
        if (size == (size_t)(-1))
        {
            printf("Couldn't convert string\n");
        }
        else
        {
            answer = pWTempString;
        }
    }


    if (pWTempString != NULL)
    {
        delete[] pWTempString;
    }

    return answer;
}

std::string GetStringFromWString(const std::wstring& wstr) {
    std::size_t size = sizeof(wstr.c_str());
    char* str = new char[size];
    std::string temp;

    std::wcstombs(str, wstr.c_str(), size);

    temp = str;
    delete[] str;

    return temp;
}

bool IsFilePath(const std::wstring& str) {
    if (str.find(L'\\') == std::wstring::npos) {
        return false;
    }

    return true;
}

bool IsFilePath(const std::string& str) {
    if (str.find('\\') == std::string::npos) {
        return false;
    }

    return true;
}