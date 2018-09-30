#pragma once

#include "Error.h"
#include "Entity.h"
#include "GUI.h"

class Message {
public:
	std::string msg = "xD";
	TCODColor col = TCODColor::red;
};

class Engine
{
public:

	std::vector<std::unique_ptr<GUI>> UI_list;
	int GUI_ID = -1;

	int x, y;
	int yPosition;

	std::shared_ptr<Entity> player = std::make_shared<Entity>(Entity());
	std::vector<std::shared_ptr<Entity>> npcs;

	std::shared_ptr<Entity> checkEntityAtPos(Map::Pos p);
	bool checkEntityCollisionAtPos(Map::Pos p);

	Map* map;

	TCOD_key_t lastKey;
	
	std::vector<Message> console;

	Engine();
	~Engine();

	void term();
	void init();
	void render();
	void update();


	void renderMap(int mOffX, int mOffY);
	void renderMapInverted();
	void renderMapZoomedOut();
	void renderMapStandard();

	void saveMap(const char * name = "default.map");
	void loadMap(const char * name = "default.map");

	int layerSize = 20;
	bool betterRenderer = false;
};

extern Engine engine;
