#ifndef HACKINTERFACE_H
#define HACKINTERFACE_H

#include <string>
#include <map>
#include "Parsing/ConfigFile.h"

class DLLInjector;

class HackInterface {
public:
	class InterfaceVariable;
private:
	std::string hackname;

	HANDLE m_processHandle;
	bool m_badHandle;

	DLLInjector& m_DLLInjector;
	std::map<std::string, InterfaceVariable> m_variablemap;
public:
	HackInterface(std::string hackname, HANDLE processhandle, DLLInjector& injector);
	~HackInterface();

	/*enum CONNECTION_TYPE {
		FILE_MAP,
		WINDOWS_PIPE,
		WINDOWS_RPC,
		NETWORK_PROTOCOL
	};*/

	class InterfaceVariable {
	public:
		std::string m_name;
		ConfigFile::GameHackData::VariableType m_type;
		void* m_location; // not planning on passing this pointer around, so no smart pointers or variant (thank goodness)

		InterfaceVariable() : m_name(""), m_type(ConfigFile::GameHackData::VERROR), m_location(nullptr) {}
		InterfaceVariable(std::string name, ConfigFile::GameHackData::VariableType type, void* location) : m_name(name), m_type(type), m_location(location) {}

		~InterfaceVariable() {}
	};
	
	virtual bool UpdateVariable(std::string name, void* val, ConfigFile::GameHackData::VariableType)=0;
	virtual bool RequestVariable(std::string name, ConfigFile::GameHackData::VariableType)=0;


	std::string GetHackName();
	HANDLE GetProcessHandle();

	void MakeProccessHandleBad() { m_badHandle = true; }
};


class HackInterfaceFileMap : public HackInterface {
private:

public:
	HackInterfaceFileMap(std::string hackname, HANDLE processhandle, DLLInjector& injector);
	~HackInterfaceFileMap();

	bool UpdateVariable(std::string name, void* val, ConfigFile::GameHackData::VariableType var);
	bool RequestVariable(std::string name, ConfigFile::GameHackData::VariableType);

};


#endif