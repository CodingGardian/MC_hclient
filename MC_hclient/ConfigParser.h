#pragma once

#include <string>
#include <map>
#include <vector>

#include "Utilities/DebugTool.h"
#include "variant.h"
#include "ConfigFile.h"

/* PLAN:
* In order to parse the file, it steps through the data in the file and enters certain contexts triggered by certain character values
* When the context exits, it returns an Element, which is a readable format of the data in the file


* EX: "" enters context that reads the value as a string (does not interpret # and !)
* 

*/

// TODO: investigate methods of key delegation


#define DEFINE_PARSER_CONTEXT(classname, instancename)			  \
		class classname : protected ConfigParser::ParserContext { \
		public:													  \
		classname() {											  \
		m_dbgTool.SetScope(instancename);						  \
		ConfigParser::ParserContext::m_seqPairs = {

#define END_DEFINE_PARSER_CONTEXT(classname, instancename) };      \
		ParserContextList::AddParserContext(instancename, this);   \
		}														   \
		Variant<> CallParser(wchar_t* startptr, ConfigFile& p); \
		};														   \
		classname classname##_NOUSEINSTANCE;
		



class ConfigParser {
public:
	class ParserContext;
private:
	std::wstring m_fileName;
	std::vector<std::string> m_vecContext;

	DebugTools m_dbgTool;
public:
	class ParserContext {
	protected:
		// const char*: sequence that triggers the coresponding ParserContext (specified as a wstring that is places in the list search function)
		std::map<wchar_t, std::wstring> m_seqPairs;
		DebugTools m_dbgTool;
	public:

		ParserContext();
		~ParserContext();

		// argument: pointer to the start of data to read from
		std::wstring GetParserFromSequence(wchar_t* sequence);

		virtual Variant<> CallParser(wchar_t* startptr, ConfigFile& p)=0;
		
		//Variant<> ParserLoop(char* startptr)
	};
	

	// add desired parser contexts, ...
	ConfigParser();
	~ConfigParser();

	void AddParserContext(ParserContext);
	
	// find the correct context to start parsing
	void BeginParsing(char* startptr);

};

class ParserContextList {
private:
	static std::map <std::wstring, ConfigParser::ParserContext*> m_parserContextList;
	static DebugTools m_dbgTool;
public:
	ParserContextList() {}
	~ParserContextList() {}

	static inline ConfigParser::ParserContext* GetParserContext(std::wstring handle) { if (m_parserContextList.find(handle) == m_parserContextList.end()) { m_dbgTool.PrintConsole("Counld not find key: %ls for global context list!"); return nullptr; }  return m_parserContextList[handle]; }
	static inline void AddParserContext(std::wstring handle, ConfigParser::ParserContext* ptr) { m_parserContextList[handle]; }
};
