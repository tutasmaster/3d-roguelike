#pragma once

#include "Error.h"

#include "Entity.h"

class Message {
public:
	std::string msg = "xD";
	TCODColor col = TCODColor::red;
};

class Engine
{
public:

	int x, y;
	int yPosition;

	std::shared_ptr<Entity> player = std::make_shared<Entity>(Entity());
	std::vector<std::shared_ptr<Entity>> npcs;

	std::shared_ptr<Entity> checkEntityAtPos(Map::Pos p);
	bool checkEntityCollisionAtPos(Map::Pos p);

	Map map;

	TCOD_key_t lastKey;
	
	std::vector<Message> console;

	Engine();
	~Engine();

	void term();
	void init();
	void render();
	void update();


	void renderMap();
	void renderMapStandard();

	int layerSize = 20;
	bool betterRenderer = false;
};

extern Engine engine;
