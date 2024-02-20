#pragma once
#include <windows.h>
#include <string>
#include <vector>

class DLLInjector {
public:
	enum InjectorType;
private:
	static std::vector<DLLInjector*> Injector_Table;
	
	// DLLPath is local to the running process
	std::wstring m_ID;
	std::wstring m_DLLPath;
	HANDLE m_process;

	InjectorType t;

	bool m_bLoaded;

	bool m_bBadInstance;

	static HANDLE THIS_INSTANCE;

	HANDLE m_hRemoteThread;
	LPDWORD m_RemoteThreadID;

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

	DLLInjectorTable& GetInjectorTableInstance();

	//int LoadDLL();

	// DO LATER
	int UnloadDLL();

	bool IsBadInstance() { return m_bBadInstance; }
};