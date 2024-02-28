#pragma once
#include <string>
#include "ConfigParser.h"

// TODO!!!!!!: Change Variant to Variant (what was I thinking...)

/*FORMAT:
* # deduce start hack structure
* "" mean interpret data as a string
* !denotes var name
*	- variables can be changed from client editor
*   - variables are handled on the hack dll, the hack dll will read and change variable values from the client
* @ denotes parsing command?
*   - @END -> end parsing

#"hackname1" "game name" "dll path" {
	!"var1" "var type" "default_value",
	...
}

#"hackname2" ...


@END

*/


class ConfigFile {
public:
	class GameHackData;

private:
	std::wstring m_filePath;
	ConfigParser m_parser;

	// std::wstring name,  Variant<>: value
	std::map<std::wstring, Variant<>> GlobalModifiers;

	std::vector<GameHackData> m_hacks;

	DebugTools m_dbgTool;

	GameHackData& m_CurrentHackData;

public:
	// TODO: Add class that is an element (code it myself no library because I am a big brain programmer (in great pain)
	class GameHackData {
	public:
		enum VariableType {
			ERROR=-1,
			INT = 0,
			FLOAT,
			DOUBLE,
			STRING,
			POINTER
		};

		static const std::map < std::wstring, VariableType > m_variablemap;

		struct VariableData {
			VariableType m_type;
			Variant<> data;
		};

	private:
		std::wstring m_hackName;
		std::wstring m_gameName;
		std::wstring m_dllPath;

		// wstring: name, void*: data
		std::map<std::wstring, VariableData> m_variables;

		DebugTools m_dbgTool;
	public:
		typedef std::map<std::wstring, VariableData>::value_type VariableMapType;

		GameHackData(std::wstring hackName, std::wstring gameName,std::wstring dllPath) : m_hackName(hackName), m_gameName(gameName), m_dllPath(dllPath), m_dbgTool(L"GameHackData", hackName) {}
		GameHackData();
		~GameHackData();

		void PushVariable(std::map<std::wstring, VariableData>::value_type var) { m_variables.insert(var); }

		inline static VariableType GetVariableTypeFromStr(std::wstring str) { if (m_variablemap.find(str) == m_variablemap.end()) { return VariableType::ERROR; } return m_variablemap.at(str); }
		
	};

	ConfigFile(std::wstring filePath);
	~ConfigFile();

	int PreLoadFile();
	int ReLoadFile();

	GameHackData& GetHackData(int index);

	GameHackData& GetCurrentHackData() { return m_CurrentHackData; }
	void SetCurrentHackData(int index) { m_CurrentHackData = m_hacks[index]; }

	int GetHackDataVecLen() { return m_hacks.size(); }


};