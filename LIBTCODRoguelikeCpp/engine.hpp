#pragma once

#include "Error.h"
#include "Entity.h"
#include "GUI.h"
#include "Effect.h"

#include <iomanip>
#include <ctime>
#include <sstream>

class Message {
public:
	std::string msg = "xD";
	TCODColor col = TCODColor::red;
};

static class EngineRenderer
{
public:
	void renderMap(int mOffX, int mOffY, int angle, int width = 62, int height = 62);
	void renderMapOLD(int mOffX, int mOffY, int angle);
	void renderMapOLD(int mOffX, int mOffY);
	void renderMapInverted();
	void renderMapZoomedOut();
	void renderMapStandard();
	void renderCompass(char angle);
	void drawFullCharacter(int x, int y, char c, const TCODColor col, const TCODColor bg);


	int layerSize = 20;
} engineRenderer;

class Engine
{
public:

	std::vector<std::shared_ptr<Effect>> effects;

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

	std::vector<int> GetAngles(char angle);


	void saveMap(const char * name = "default.map");
	void loadMap(const char * name = "default.map");

	bool betterRenderer = false;
	char angle = 0;

	TCOD_key_t keyboardInput;
	TCOD_mouse_t mouseInput;
};

extern Engine engine;
