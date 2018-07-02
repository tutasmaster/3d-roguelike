#pragma once

#include "libtcod.hpp"
#include <vector>

class Tile {
public:
	enum Type {
		empty,
		wall,
		floor
	} type = empty;

	TCODColor bg = TCODColor::black;
	TCODColor color = TCODColor::white;

	int shadeLimit = 5;

	char c = '.';

	bool isBlocking = true;
};

typedef int TileID;

class TileManager {
public:
	TileManager();
	Tile* GetTileData(TileID tile);
	std::vector<Tile> tiles;

	enum tileIDs {
		empty,
		wall,
		floor,
		wallNB
	};
};

extern TileManager tileManager;
