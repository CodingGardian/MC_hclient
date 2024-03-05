//#include "ConfigParser.h"
#include <utility>
#include <memory>

#include "ConfigFile.h"
#include "Utilities/StringUtil.h"

std::map <std::string, ConfigParser::ParserContext*> ParserContextList::m_parserContextList;
DebugTools ParserContextList::m_dbgTool("ParserContextList");

#define NO_PARSER "NULL"
std::string ConfigParser::ParserContext::GetParserFromSequence(char* sequence) {
	
	auto it = m_seqPairs.find(*sequence);
	if (it == m_seqPairs.end()) { return NO_PARSER; }
	else { return it->second; }

}


DEFINE_PARSER_CONTEXT(NameParser, L"NameParserInstance")
END_DEFINE_PARSER_CONTEXT(NameParser, "NameParserInstance")

Variant<> NameParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParse) {
	std::string* returnVal = new std::string("");
	std::string temp;
	startptr++;
	while(true) {
		if (*startptr == L'\"') {
			startptr++;
			return Variant<std::string>(returnVal).GetVariant<>();
		}
		(*returnVal) += *startptr;
		startptr++;
		if (configFile.GetEndPtr() == startptr) {
			m_dbgTool.PrintConsole("Enexpected end of file while parsing string! String so far: %s", returnVal->c_str());
			isBadParse = true;
			return returnVal;
		}
	}
}

DEFINE_PARSER_CONTEXT(HackHeaderParser, L"HackHeaderParserInstance")
{'\"', "NameParserInstance"},
{'{', "HackBodyParserInstance"}
END_DEFINE_PARSER_CONTEXT(HackHeaderParser, "HackHeaderParserInstance")

// should be called only when '#' is parsed
Variant<> HackHeaderParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParse) {
	std::string hackname;
	std::string gamename;
	std::string dllpath;
	// get hack name
	std::string tempgetparser;
	
	// each while loop parses untill it reaches a ", then it gives control to NameParser
	// whose job is to parse the value within the "" and return the string and set startptr to the character after the string

	// get hack name
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == "NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			hackname = ParserContextList::GetParserContext("NameParserInstance")->CallParser(startptr, configFile, isBadParse).GetValue<std::string>();
			if (hackname == "") { m_dbgTool.PrintConsole("hack name was null string!"); isBadParse = true; return NULL_VARIANT; }
			else if (isBadParse == true) { m_dbgTool.PrintConsole("Parsing hack name failed!"); return NULL_VARIANT; }
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %s, while parsing file", *startptr);
			isBadParse = true;
			return NULL_VARIANT;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) { m_dbgTool.PrintConsole("Encountered unexpected end of file while parsing hack name!"); isBadParse = true; return NULL_VARIANT; }
	}
	
	m_dbgTool.SetObjID(hackname);
	m_dbgTool.PrintConsole("Parsing hack name: %s", hackname.c_str());
	// get game name
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == "NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			gamename = ParserContextList::GetParserContext("NameParserInstance")->CallParser(startptr, configFile, isBadParse).GetValue<std::string>();
			if (gamename == "") { m_dbgTool.PrintConsole("Game name was null string!"); isBadParse = true; }
			else if (isBadParse == true) { m_dbgTool.PrintConsole("Parsing game name for hack: %s failed!", hackname.c_str()); return NULL_VARIANT; }
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %s, while parsing file", *startptr);
			isBadParse = true;
			return NULL_VARIANT;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) { m_dbgTool.PrintConsole("Encountered unexpected end of file while parsing game name for hack: %s!", hackname.c_str()); isBadParse = true; return NULL_VARIANT; }
	}

	// get dll path
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == "NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			dllpath = ParserContextList::GetParserContext("NameParserInstance")->CallParser(startptr, configFile, isBadParse).GetValue<std::string>();
			if (gamename == "") { m_dbgTool.PrintConsole("DLL name was null string!"); isBadParse = true; }
			else if (isBadParse == true) { m_dbgTool.PrintConsole("Parsing DLL path for hack: %s failed!", hackname.c_str()); return NULL_VARIANT; }
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %s, while parsing file", *startptr);
			isBadParse = true;
			return nullptr;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) { m_dbgTool.PrintConsole("Encountered unexpected end of file while parsing game name for hack: %s!", hackname.c_str()); isBadParse = true; return NULL_VARIANT; }
	}


	m_dbgTool.PrintConsole("Parsed header. Hack name: %s, Game name: %s, DLL path: %s", hackname.c_str(), gamename.c_str(), dllpath.c_str());

	Variant<ConfigFile::GameHackData> gamehackdata( new ConfigFile::GameHackData(hackname, gamename, dllpath, configFile) );

	configFile.SetCurrentHackData(configFile.GetHackDataVecLen() - (size_t)1);

	// begin parsing the body of the hack config file
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == "HackBodyParserInstance") {
			// wow this is also really long... AAAHHHHH
			ParserContextList::GetParserContext("HackBodyParserInstance")->CallParser(startptr, configFile, isBadParse);
			if (isBadParse) { m_dbgTool.PrintConsole("Parsing body for hack: %s failed!", hackname.c_str()); return NULL_VARIANT; }
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %s, while parsing file", *startptr);
			isBadParse = true;
			return NULL_VARIANT;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) { m_dbgTool.PrintConsole("Encountered unexpected end of file while parsing for game hack body for hack: %s!", hackname.c_str()); isBadParse = true; return NULL_VARIANT; }
	}


	return gamehackdata.GetVariant<>();
}


DEFINE_PARSER_CONTEXT(HackVariableParser, "HackVariableParserInstance")
{'\"', "NameParserInstance"}
END_DEFINE_PARSER_CONTEXT(HackVariableParser, "HackVariableParserInstance")

// parses a variable name, type, and default value and converts it to the VariableType which can be used immediately in program
Variant<> HackVariableParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParser) {
	std::string tempgetparser;
	std::string variablename;
	ConfigFile::GameHackData::VariableType variabletype;
	Variant<> variabledata= nullptr;

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			variablename = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile, isBadParser).GetValue<std::string>();
			break;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) {
			if (variabledata.CheckVariantNull()) { m_dbgTool.PrintConsole("Unexpected end file before name of variable could be parsed!"); isBadParser = true; return NULL_VARIANT; }
			break;
		}
	}

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			std::string variabletypestr = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile, isBadParser).GetValue<std::string>();
			variabletype = ConfigFile::GameHackData::GetVariableTypeFromStr(variabletypestr);
			if (variabletype == ConfigFile::GameHackData::VERROR) { m_dbgTool.PrintConsole("Unable to parse type: %s for variablename: %s in hack: %s!", variabletypestr.c_str(), variablename.c_str(), configFile.GetCurrentHackData().GetHackName().c_str()); isBadParser = true; }
			break;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) {
			if (variabledata.CheckVariantNull()) { m_dbgTool.PrintConsole("Unexpected end file before type of variable: %s could be parsed!", variablename.c_str()); isBadParser = true; return NULL_VARIANT; }
			break;
		}
	}

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			CEString variabledatastr = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile, isBadParser).GetValue<std::string>();
			if (variabledatastr.GetString() == "") { m_dbgTool.PrintConsole("Variable data string for variable: %s is null!", variablename.c_str()); isBadParser = true; return NULL_VARIANT; }
			else if (isBadParser == true) { m_dbgTool.PrintConsole("Parsing variable data for variable: %s failed!", variablename.c_str());  return NULL_VARIANT; }

			switch (variabletype) {
			case ConfigFile::GameHackData::VariableType::INT:
				// for some dumb reason the compiler doesnt work if I use the regular = operator...
				variabledata.operator=<int>( variabledatastr.GetInt() );
				break;
			case ConfigFile::GameHackData::VariableType::INT64:
				variabledata = variabledatastr.GetInt64();
				break;
			case ConfigFile::GameHackData::VariableType::UINT:
				variabledata = variabledatastr.GetUInt();
				break;
			case ConfigFile::GameHackData::VariableType::UINT64:
				variabledata = variabledatastr.GetUInt64();
				break;
			case ConfigFile::GameHackData::VariableType::FLOAT:
				variabledata = variabledatastr.GetFloat();
				break;
			case ConfigFile::GameHackData::VariableType::DOUBLE:
				variabledata = variabledatastr.GetDouble();
				break;
			case ConfigFile::GameHackData::VariableType::POINTER:
				variabledata = variabledatastr.GetPointer();
				break;
			case ConfigFile::GameHackData::VariableType::VERROR:
				m_dbgTool.PrintConsole("Unexpected variable type ERROR for variable: %s!", variablename.c_str());
				isBadParser = true;
				return NULL_VARIANT;
				break;
			default:
				m_dbgTool.PrintConsole("Unexpected variable type %i for variable: %s!", variabletype, variablename.c_str());
				return NULL_VARIANT;
				break;
			}
			if (variabledata.CheckVariantNull()) { m_dbgTool.PrintConsole("Variabledata for variable: %s in hack: %s is NULL!", variablename.c_str(), configFile.GetCurrentHackData().GetHackName().c_str()); }
			break;
		}
		startptr++;
		if (configFile.GetEndPtr() == startptr) {
			if (variabledata.CheckVariantNull()) { m_dbgTool.PrintConsole("Unexpected end file before variabledata of variable: %s in hack: %s could be parsed!", variablename.c_str(), configFile.GetCurrentHackData().GetHackName().c_str()); isBadParser = true; return NULL_VARIANT; }
			break;
		}
	}

	configFile.GetCurrentHackData().PushVariable( ConfigFile::GameHackData::Element(variablename, variabletype, variabledata) );
	return NULL_VARIANT;
}


DEFINE_PARSER_CONTEXT(HackBodyParser, "HackBodyParserInstance")
{'!', "HackVariableParserInstance"}
END_DEFINE_PARSER_CONTEXT(HackBodyParser, "HackBodyParserInstance")

Variant<> HackBodyParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParser) {
	std::string tempgetparser = "";
	Variant<int> returnVal = new int(1); // why is it 1
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {
			if (*startptr == '}') { return returnVal.GetVariant<>(); }
		}
		else {
			// wow this is also really long... AAAHHHHH
			ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile, isBadParser);
		}
		startptr++;
		if (startptr == configFile.GetEndPtr()) { m_dbgTool.PrintConsole("Unexpected end of file while parsing hack: %s", configFile.GetCurrentHackData().GetHackName().c_str()); return NULL_VARIANT; }
	}

}


DEFINE_PARSER_CONTEXT(GlobalContextParser, "GlobalContextParserInstance")
{ '#', "HackHeaderParserInstance" },
{ '@', "GlobalModifierParserInstance" }
END_DEFINE_PARSER_CONTEXT(GlobalContextParser, "GlobalContextParserInstance")

Variant<> GlobalContextParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParser) {
	isBadParser = false;
	std::string tempgetparser;

	m_dbgTool.PrintConsole(L"In GlobalContextParser!");
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		
		if (tempgetparser == NO_PARSER) {

		}
		else {
			m_dbgTool.PrintConsole(L"Calling parser %s from context %c", tempgetparser.c_str(), *startptr);
			ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile, isBadParser);
			if (isBadParser == true) { m_dbgTool.PrintConsole("Parsing function %s failed!", tempgetparser.c_str()); return NULL_VARIANT; }
		}
		startptr++;
		if (startptr == configFile.GetEndPtr()) { return NULL_VARIANT; }
	}

	return NULL_VARIANT;
}

DEFINE_PARSER_CONTEXT(GlobalModifierParser, "GlobalModifierParserInstance")
END_DEFINE_PARSER_CONTEXT(GlobalModifierParser, "GlobalModifierParserInstance")

Variant<> GlobalModifierParser::CallParser(char*& startptr, ConfigFile& configFile, bool& isBadParser) {
	return NULL_VARIANT;
}



ConfigParser::ConfigParser(char* startptr, ConfigFile& cfile) : m_startptr(startptr), m_cfile(cfile) {}

bool ConfigParser::ParseBuffer() {
	ParserContextList::GetParserContext("GlobalContextParserInstance")->CallParser(m_startptr, m_cfile, m_isBadParser);
	return m_isBadParser;
}