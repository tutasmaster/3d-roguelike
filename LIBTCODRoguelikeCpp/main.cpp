// Main.cpp : Defines the entry point for the console application.
//

#include "engine.hpp"
#include "Parser.h"

TileManager tileManager = TileManager();
ItemManager itemManager = ItemManager();


Engine engine = Engine();

int main()
{
	TCODSystem::setFps(30);

	engine.init();

	while (!TCODConsole::isWindowClosed()) {
		engine.render();
		engine.update();
		TCODConsole::flush();
	}
    return 0;
}

