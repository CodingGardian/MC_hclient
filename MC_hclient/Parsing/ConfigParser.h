#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H
#include <string>
#include <map>
#include <vector>

#include "Utilities/DebugTool.h"
#include "Utilities/variant.h"

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

#define END_DEFINE_PARSER_CONTEXT(classname, instancename) };					  \
		ParserContextList::AddParserContext(instancename, this);				  \
		}																		  \
		Variant<> CallParser(char*& startptr, ConfigFile& p, bool& isBadParse); \
		};																		  \
		classname classname##_NOUSEINSTANCE;
		

class ConfigFile;

class ConfigParser {
public:
	class ParserContext;
private:
	std::string m_fileName;
	//std::vector<std::string> m_vecContext;

	DebugTools m_dbgTool;

	char* m_startptr;
	ConfigFile& m_cfile;
	bool m_isBadParser;

public:
	// TODO: Fill this out and use for various ways that a parser can fail
	enum ParserExitCodes {};

	class ParserContext {
	protected:
		// const char*: sequence that triggers the coresponding ParserContext (specified as a string that is places in the list search function)
		std::map<char, std::string> m_seqPairs;
		DebugTools m_dbgTool;
	public:

		ParserContext() {}
		~ParserContext() {}

		// argument: pointer to the start of data to read from
		std::string GetParserFromSequence(char* sequence);

		virtual Variant<> CallParser(char*& startptr, ConfigFile& p, bool& isBadParse)=0;
		
		//typedef bool (*ParseLoopFunc)(char*, ConfigFile&);
		//void SafeParserLoop()

		//Variant<> ParserLoop(char* startptr)
	};
	

	// add desired parser contexts, ...
	ConfigParser(char* startptr, ConfigFile& cfile);
	~ConfigParser() {}

	//void AddParserContext(ParserContext);
	
	// find the correct context to start parsing
	bool ParseBuffer();

};

class ParserContextList {
private:
	static std::map <std::string, ConfigParser::ParserContext*> m_parserContextList;
	static DebugTools m_dbgTool;
public:
	ParserContextList() {}
	~ParserContextList() {}

	static inline ConfigParser::ParserContext* GetParserContext(std::string handle) { if (m_parserContextList.find(handle) == m_parserContextList.end()) { m_dbgTool.PrintConsole("Counld not find key: %ls for global context list!", handle); return nullptr; }  return m_parserContextList[handle]; }
	static inline void AddParserContext(std::string handle, ConfigParser::ParserContext* ptr) { m_parserContextList[handle] = ptr; }
};

#endif