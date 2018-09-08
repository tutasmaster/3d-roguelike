#pragma once

#include "libtcod.hpp"
#include <vector>

class Tile {
public:
	enum Type {
		empty,
		wall,
		liquid
	} type = empty;

	TCODColor bg = TCODColor::black;
	TCODColor color = TCODColor::white;

	int shadeLimit = 20;

	char c = '.';

	bool isBlocking = true;
};

class Ground {
public:
	enum Type {
		empty,
		wall,
		liquid
	} type = wall;

	TCODColor bg = TCODColor::black;
	TCODColor color = TCODColor::white;

	int shadeLimit = 20;

	char c = '.';
};

typedef unsigned char TileID;
typedef unsigned char GroundID;

class TileManager {
public:
	TileManager();
	Tile* GetTileData    (TileID tile);
	Ground* GetGroundData (GroundID ground);
	std::vector <Tile>           tiles;
	std::vector <Ground>         grounds;

	enum tileIDs : unsigned char{
		tile_empty,
		tile_wall,
		tile_wallNB,
		tile_dirt,
		tile_grass,
		tile_stone,
		tile_water,
		tile_wood,
		tile_leaves,
		tile_sand,
		tile_cacti,
	};

	enum groundIDs : unsigned char {
		ground_empty,
		ground_wall,
		ground_dirt,
		ground_grass,
		ground_stone,
		ground_water,
		ground_wood,
		ground_leaves,
		ground_sand,
		ground_cacti,
	};
};

extern TileManager tileManager;
