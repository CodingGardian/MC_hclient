#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <string>
#include <windows.h>
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

	ConfigParser* m_parser;

	// std::wstring name,  Variant<>: value
	std::map<std::string, Variant<>> GlobalModifiers;

	std::vector<GameHackData*> m_hacks;

	DebugTools m_dbgTool;

	int m_CurrentHackData;

	HANDLE m_hFileHandle;

	// region in memory where the file was read!!!
	char* m_fileBuffer;
	// region in memory where the file was read!!!

	bool m_bisBadInstance;

	uint64_t m_fileSize;
public:
	// TODO: INTEGRATE ELEMNT TYPE INTO STD::MAP!!!!
	class GameHackData {
	public:
		enum VariableType {
			VERROR=-1,
			INT = 0,
			INT64,
			UINT,
			UINT64,
			FLOAT,
			DOUBLE,
			STRING,
			POINTER
		};

		static const std::map < std::string, VariableType > m_variablemap;

		struct VariableData {
			VariableType m_type;
			Variant<> data;
		};

		class Element;

	private:
		std::string m_hackName;
		std::string m_gameName;
		std::string m_dllPath;

		// wstring: name, void*: data
		std::vector<Element> m_variables;

		

		DebugTools m_dbgTool;
	public:
		typedef std::map<std::string, VariableData>::value_type VariableMapType;

		class Element {
		public:
			std::string m_name;
			VariableType m_type;
			Variant<> m_data;

			Element(std::string name, VariableType type, Variant<> data) : m_name(name), m_type(type), m_data(data) {}
		};

		GameHackData(std::string hackName, std::string gameName, std::string dllPath, ConfigFile& cfile) : m_hackName(hackName), m_gameName(gameName), m_dllPath(dllPath), m_dbgTool(L"GameHackData", GetWStringFromString(hackName)) { cfile.PushHackData(this); }
		~GameHackData() {}

		inline void PushVariable(Element var) { m_variables.push_back( var ); }

		inline static VariableType GetVariableTypeFromStr(std::string str) { if (m_variablemap.find(str) == m_variablemap.end()) { return VariableType::VERROR; } return m_variablemap.at(str); }

		inline std::string GetHackName() { return m_hackName; }
		inline std::string GetGameName() { return m_gameName; }
		inline std::string GetDLLPath() { return m_dllPath; }

		inline Element GetVariableAtIndex(int index) {
			if (index < 0 || index > m_variables.size()) {
				return Element("", VERROR, NULL_VARIANT);
			}
			return m_variables[index];
		}

		inline Element GetVariableData(std::string name) {
			for (auto elem : m_variables) { if (elem.m_name == name) { return elem; } }
			return Element("", VERROR, NULL_VARIANT);
		}

		inline int GetVariableMapSize() { return m_variables.size(); }
		
		void PrintAllVariables();

	};

	ConfigFile(std::wstring filePath);
	~ConfigFile() { for (GameHackData* ptr : m_hacks) { delete ptr; } delete m_parser; delete[] m_fileBuffer; }

	/*int PreLoadFile();
	int ReLoadFile();*/

	bool LoadFile() { if (m_bisBadInstance) { m_dbgTool.PrintConsole("LoadFile was called for bad instace of file: %ls", m_filePath); return false; } m_parser = new ConfigParser(m_fileBuffer, *this); m_parser->ParseBuffer(); return true; }

	GameHackData& GetHackData(int index) { return *m_hacks[index]; }

private:
	inline void PushHackData(GameHackData* ptr) { m_hacks.push_back(ptr); }
public:

	inline GameHackData& GetCurrentHackData() {
		if (m_CurrentHackData == -1) { m_dbgTool.PrintConsole(L"Tried to retrieve m_CurrentHackdata when it was invalid handle -1"); } return *m_hacks[m_CurrentHackData]; }

	inline void SetCurrentHackData(int index) { m_CurrentHackData = index; }

	inline size_t GetHackDataVecLen() { return m_hacks.size(); }

	inline char* GetEndPtr() { return m_fileBuffer + m_fileSize; }

	// PLEASE PLEASE PLEASE PLEASE PLEASE CHECK THIS BEFORE DOING ANYTHING SINCE FUNCTIONS WILL HAVE UNDEFINED BEHAVIOR IF THE INSTANCE IS BAD!!!!!
	bool IsBadInstance() { return m_bisBadInstance; }

	friend class GameHackData;
};

#endif