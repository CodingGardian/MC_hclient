#include "ConfigParser.h"
#include "ConfigFile.h"
#include <utility>
#include <memory>


std::map <std::wstring, ConfigParser::ParserContext*> ParserContextList::m_parserContextList;
DebugTools ParserContextList::m_dbgTool(L"ParserContextList");

#define NO_PARSER L"NULL"
std::wstring ConfigParser::ParserContext::GetParserFromSequence(wchar_t* sequence) {
	std::wstring returnVal;
	
	auto it = m_seqPairs.find(*sequence);
	if (it == m_seqPairs.end()) { return NO_PARSER; }
	else { return it->second; }

}


DEFINE_PARSER_CONTEXT(NameParser, L"NameParserInstance")
END_DEFINE_PARSER_CONTEXT(NameParser, L"NameParserInstance")

Variant<> NameParser::CallParser(wchar_t* startptr, ConfigFile& configFile) {
	std::wstring* returnVal = new std::wstring;
	std::wstring temp;
	startptr++;
	while(true) {
		if (*startptr == L'\"') {
			return returnVal;
		}
		returnVal->append(startptr);
		startptr++;
	}
}

DEFINE_PARSER_CONTEXT(HackHeaderParser, L"HackHeaderParserInstance")
{L'\"', L"NameParserInstance"},
{L'{', L""}
END_DEFINE_PARSER_CONTEXT(HackHeaderParser, L"HackHeaderParserInstance")

Variant<> HackHeaderParser::CallParser(wchar_t* startptr, ConfigFile& configFile) {
	std::wstring hackname;
	std::wstring gamename;
	std::wstring dllpath;
	// get hack name
	std::wstring tempgetparser;
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == L"NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			hackname = ParserContextList::GetParserContext(L"NameParserInstance")->CallParser(startptr, configFile).GetValue<std::wstring>();
			startptr++;
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %ls, while parsing file", *startptr);
			return nullptr;
		}
		startptr++;
	}

	// get game name
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == L"NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			gamename = ParserContextList::GetParserContext(L"NameParserInstance")->CallParser(startptr, configFile).GetValue<std::wstring>();
			startptr++;
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %ls, while parsing file", *startptr);
			return nullptr;
		}
		startptr++;
	}

	// get dll path
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == L"NameParserInstance") {
			// wow this looks really ugly, IDK if im rewrite because this took a while
			dllpath = ParserContextList::GetParserContext(L"NameParserInstance")->CallParser(startptr, configFile).GetValue<std::wstring>();
			startptr++;
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %ls, while parsing file", *startptr);
			return nullptr;
		}
		startptr++;
	}

	Variant<ConfigFile::GameHackData> gamehackdata( new ConfigFile::GameHackData(hackname, gamename, dllpath) );
	configFile.SetCurrentHackData(configFile.GetHackDataVecLen() - 1);

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else if (tempgetparser == L"HackBodyParserInstance") {
			// wow this is also really long... AAAHHHHH
			//gamehackdata.Get()->PushVariable(ParserContextList::GetParserContext(L"HackBodyParserInstance")->CallParser(startptr).GetValue<ConfigFile::GameHackData::VariableType>());
			ParserContextList::GetParserContext(L"HackBodyParserInstance")->CallParser(startptr, configFile);
			startptr++;
			break;
		}
		else {
			m_dbgTool.PrintConsole("Encountered unexpected sequence: %ls, while parsing file", *startptr);
			return nullptr;
		}
	}

	return gamehackdata.GetVariant<void>();
}


DEFINE_PARSER_CONTEXT(HackVariableParser, L"HackVariableParserInstance")
{L'\"', L"NameParserInstance"}
END_DEFINE_PARSER_CONTEXT(HackVariableParser, L"HackVariableParserInstance")

Variant<> HackVariableParser::CallParser(wchar_t* startptr, ConfigFile& configFile) {
	std::wstring tempgetparser;
	std::wstring variablename;
	ConfigFile::GameHackData::VariableType variabletype;
	Variant<> variabledata= nullptr;

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			variablename = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile).GetValue<std::wstring>();
			break;
		}
		startptr++;
	}

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			std::wstring variabletypestr = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile).GetValue<std::wstring>();
			variabletype = ConfigFile::GameHackData::GetVariableTypeFromStr(variabletypestr);
			startptr++;
			break;
		}
		startptr++;
	}

	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {

		}
		else {
			std::wstring variabledatastr = ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile).GetValue<std::wstring>();
			// I'll leave this here I guess...
		}
	}
}


DEFINE_PARSER_CONTEXT(HackBodyParser, L"HackBodyParserInstance")
{L'!', L"HackVariableParserInstance"}
END_DEFINE_PARSER_CONTEXT(HackBodyParser, L"HackBodyParserInstance")

Variant<> HackBodyParser::CallParser(wchar_t* startptr, ConfigFile& configFile) {
	std::wstring tempgetparser;
	Variant<int> returnVal = 1;
	while (true) {
		tempgetparser = GetParserFromSequence(startptr);
		if (tempgetparser == NO_PARSER) {
			if (*startptr = '}') { return returnVal; }
		}
		else {
			// wow this is also really long... AAAHHHHH
			configFile.GetCurrentHackData().PushVariable(ParserContextList::GetParserContext(tempgetparser)->CallParser(startptr, configFile).GetValue<ConfigFile::GameHackData::VariableMapType>());
		}
		startptr++;
	}
}