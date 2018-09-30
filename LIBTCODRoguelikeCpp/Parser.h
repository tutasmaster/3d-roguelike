#pragma once
#include "libtcod.hpp"
#include "engine.hpp"

class Parser
{
public:
	Parser();
	void Run();
	~Parser();
};

class ParserListener : public ITCODParserListener {
	bool parserNewStruct(TCODParser *parser, const TCODParserStruct *str, const char *name) override;
	bool parserFlag(TCODParser *parser, const char *name) override;
	bool parserProperty(TCODParser *parser, const char *name, TCOD_value_type_t type, TCOD_value_t value) override;
	bool parserEndStruct(TCODParser *parser, const TCODParserStruct *str, const char *name) override;
	void error(const char *msg) ;

	Item item;
};