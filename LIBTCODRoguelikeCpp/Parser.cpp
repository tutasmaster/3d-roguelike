#include "Parser.h"
#include <filesystem>

Parser::Parser()
{

}

void Parser::Run() {

	static const char *aiList[] = { "friendly", "wander", NULL };

	TCODParser parser;
	TCODParserStruct *itemTypeStruct = parser.newStructure("item_type");
	itemTypeStruct
		->addProperty("name", TCOD_TYPE_STRING, false)
		->addProperty("short_name", TCOD_TYPE_STRING, false)
		->addProperty("character", TCOD_TYPE_CHAR, false)
		->addProperty("character_color", TCOD_TYPE_COLOR, false)
		->addProperty("background_color", TCOD_TYPE_COLOR, false);

	TCODParserStruct *entityTypeStruct = parser.newStructure("entity_type");
	
	entityTypeStruct
		->addProperty("name", TCOD_TYPE_STRING, false)
		->addProperty("character", TCOD_TYPE_CHAR, false)
		->addProperty("character_color", TCOD_TYPE_COLOR, false)
		->addProperty("ai", TCOD_TYPE_STRING, false)
		->addFlag("noCollision");
	

	std::string path = "./DataFiles";

	for (const auto & p : std::experimental::filesystem::directory_iterator(path)) {
		std::string s = "./DataFiles/" + p.path().filename().string();
		if (!s.find(".dat"))
			return;
		parser.run(s.c_str(), new ParserListener());
	}
}

Parser::~Parser()
{
}

bool ParserListener::parserNewStruct(TCODParser * parser, const TCODParserStruct * str, const char * name)
{
	std::string n = str->getName();
	if (n == "item_type") {
		item.name = name;
		parserType = type_item;
		return true;
	}if (n == "entity_type") {
		entity.name = name;
		parserType = type_entity;
		return true;
	}
	else {
		parser->error(str->getName());
		return false;
	}
}

bool ParserListener::parserFlag(TCODParser * parser, const char * name)
{
	std::string n = name;
	if (parserType == type_entity) {
		if (n == "noCollision") {
			entity.isColliding = false;
		}
		else {
			std::string s = entity.name + "(ENTITY) seems to have an invalid flag: " + n;
			parser->error(s.c_str());
			return false;
		}
	}
	return true;
}

bool ParserListener::parserProperty(TCODParser * parser, const char * name, TCOD_value_type_t type, TCOD_value_t value)
{
	std::string n = name;
	if(parserType == type_item){
		if      (n == "name")             {
			item.name = value.s;
		}
		else if (n == "short_name")       {
			item.shrtnm = value.s;
		}
		else if (n == "character")        {
			item.ch = value.c;
		}
		else if (n == "character_color")  {
			item.chCol = value.col;
		}
		else if (n == "background_color") {
			item.bgCol = value.col;
		}
		else {
			std::string s = item.name + "(ITEM) seems to have an invalid field: " + n;
			parser->error(s.c_str());
			return false;
		}
	}
	else if (parserType == type_entity) {
		if (n == "name") {
			entity.name = value.s;
		}
		else if (n == "character") {
			entity.c = value.c;
		}
		else if (n == "character_color") {
			entity.col = value.col;
		}
		else if (n == "ai") {
			std::string val = value.s;
			if (val == "wander") {
				entity.ai = std::make_shared<WanderingAi>();
			}
			else if (val == "friendly") {
				entity.ai = std::make_shared<FriendlyAi>();
			}
			else {
				std::string s = entity.name + "(ENTITY) seems to have an invalid type of AI: " + val;
				parser->error(s.c_str());
				return false;
			}
		}
		else {
			std::string s = entity.name + "(ENTITY) seems to have an invalid field: " + n;
			parser->error(s.c_str());
			return false;
		}
	}
	return true;
}

bool ParserListener::parserEndStruct(TCODParser * parser, const TCODParserStruct * str, const char * name)
{
	if (parserType == type_entity) {
		std::cout << "Loaded Entity: " << entity.name << "!\n";
		engine.npcs.push_back(std::make_shared<Entity>(entity));
	}
	else if (parserType == type_item) {
		std::cout << "Loaded Item: " << item.name << "!\n";
		itemManager.items.push_back(item);
	}
	return true;
}

void ParserListener::error(const char * msg)
{
	std::cout << msg << "\n";
}
