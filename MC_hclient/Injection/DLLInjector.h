#ifndef DLLINJECTOR_H
#define DLLINJECTOR_H
#include <windows.h>
#include <string>
#include <vector>
#include "HackInterface.h"

// TODO: MAKE SURE THIS_INTSANCE IS INITIALIZED ON PROGRAM STARTUP
class DLLInjector {
public:
	enum InjectorType;
private:
	static std::vector<DLLInjector*> Injector_Table;
	
	std::wstring m_ID;
	std::wstring m_DLLPath;
	HANDLE m_process;

	InjectorType t;

	bool m_bLoaded;

	bool m_bBadInstance;

	static HANDLE THIS_INSTANCE;

	HackInterface* m_hackInterface;

public:
	enum InjectorType { REMOTETHREAD, WINDOWSHOOK };

	// load DLL
	DLLInjector(HANDLE process, std::wstring DLLPath, InjectorType t, std::wstring id);

	// unload DLL
	~DLLInjector();

	class DLLInjectorTable {
	private:

	public:
		DLLInjectorTable();
		~DLLInjectorTable();

		
		// static for now because I have no idea if this class needs data specific to each object
		static DLLInjector SearchByID(std::string ID);
		static DLLInjector searchByDLLPath(std::string DLLPath);
	};
private:
	static DLLInjectorTable m_InjectorObjectTable;
public:

	const DLLInjectorTable& GetInjectorTableInstance() const { return m_InjectorObjectTable; }

	const HackInterface& GetHackInterface() const { return *m_hackInterface; }

	//int LoadDLL();

	// DO LATER
	int UnloadDLL();

	bool IsBadInstance() { return m_bBadInstance; }
};

#endif