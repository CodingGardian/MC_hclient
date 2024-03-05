#include "ConfigFile.h"

const std::map < std::string, ConfigFile::GameHackData::VariableType > ConfigFile::GameHackData::m_variablemap =
{
	{"INT", INT},
	{"INT64", INT64},
	{"UINT", UINT},
	{"UINT64", UINT64},
	{"FLOAT", FLOAT},
	{"DOUBLE", DOUBLE},
	{"STRING", STRING},
	{"POINTER", POINTER}
};


ConfigFile::ConfigFile(std::wstring filepath) : m_dbgTool(L"ConfigFile"), m_bisBadInstance(false), m_CurrentHackData(-1), m_filePath(filepath) {
	DWORD dwbytesread = 0;
	OVERLAPPED ol = { 0 };

	m_hFileHandle = CreateFile(
		filepath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hFileHandle == INVALID_HANDLE_VALUE) {
		m_dbgTool.PrintConsole(L"Create file failed for file name: %ls, Error code: %i", filepath.c_str(), GetLastError());

		m_bisBadInstance = true;
		return;
	}
	
	m_fileSize = GetFileSize(m_hFileHandle, NULL);

	if (m_fileSize == INVALID_FILE_SIZE) {
		m_dbgTool.PrintConsole(L"GetFileSizeEx for file: %ls failed! Error code: %i", filepath.c_str(), GetLastError());

		m_bisBadInstance = true;
		return;
	}

	DWORD numBytesRead = 0;

	// Fill the file buffer (will maybe convert to wchar_t* x amount of bytes at a time instead of all in one sitting to save memory
	char* tempBuffer = new char[m_fileSize];
	
	if (FALSE == ReadFile(m_hFileHandle, tempBuffer, m_fileSize, &numBytesRead, NULL)) {
		m_dbgTool.PrintConsole(L"Read file header failed for file name: %s, failed!\nError code %i", filepath.c_str(), GetLastError());
		m_bisBadInstance = true;
		return;
	}
	
	if (numBytesRead != m_fileSize) {
		m_dbgTool.PrintConsole(L"ReadFile did not read full size of file data for file: %ls", filepath.c_str());

		m_bisBadInstance = true;
		return;
	}


	m_fileBuffer = tempBuffer;
}



void ConfigFile::GameHackData::PrintAllVariables() {
	for (auto elem : m_variables) {
		switch (elem.m_type) {
		case ConfigFile::GameHackData::VariableType::INT:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: INT, Default value: %i", elem.m_name.c_str(), elem.m_data.GetValue<int>());
			break;
		case ConfigFile::GameHackData::VariableType::INT64:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: UINT64, Default value: %ll", elem.m_name.c_str(), elem.m_data.GetValue<int64_t>());
			break;
		case ConfigFile::GameHackData::VariableType::UINT:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: UINT, Default value: %i", elem.m_name.c_str(), elem.m_data.GetValue<unsigned int>());
			break;
		case ConfigFile::GameHackData::VariableType::UINT64:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: UINT64, Default value: %ull", elem.m_name.c_str(), elem.m_data.GetValue<uint64_t>());
			break;
		case ConfigFile::GameHackData::VariableType::FLOAT:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: FLOAT, Default value: %f", elem.m_name.c_str(), elem.m_data.GetValue<float>());
			break;
		case ConfigFile::GameHackData::VariableType::DOUBLE:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: DOUBLE, Default value: %f", elem.m_name.c_str(), elem.m_data.GetValue<double>());
			break;
		case ConfigFile::GameHackData::VariableType::POINTER:
			m_dbgTool.PrintConsole("Variable name: %s, Variable type: POINTER, Default value: 0x%p", elem.m_name.c_str(), elem.m_data.GetValue<void*>());
			break;
		case ConfigFile::GameHackData::VariableType::VERROR:
			m_dbgTool.PrintConsole("Unexpected variable type ERROR for variable: %s!", elem.m_name.c_str());
			break;
		default:
			m_dbgTool.PrintConsole("Unexpected variable type %i for variable: %s!", elem.m_type, elem.m_name.c_str());
			break;
		}
	}
}