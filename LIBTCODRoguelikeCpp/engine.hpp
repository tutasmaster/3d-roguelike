#pragma once

#include "Error.h"

#include "Entity.h"

class Engine
{
public:

	int x, y;
	int yPosition;

	std::shared_ptr<Entity> player = std::make_shared<Entity>(Entity());

	Map map;

	TCOD_key_t lastKey;

	Engine();
	~Engine();

	void term();
	void init();
	void render();
	void update();
};

extern Engine engine;
