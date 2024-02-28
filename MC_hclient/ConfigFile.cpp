#include "ConfigFile.h"


const std::map < std::wstring, ConfigFile::GameHackData::VariableType > ConfigFile::GameHackData::m_variablemap =
{
	{L"INT", INT},
	{L"FLOAT", FLOAT},
	{L"DOUBLE", DOUBLE},
	{L"STRING", STRING},
	{L"POINTER", POINTER}
};