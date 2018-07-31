#include "TileManager.h"
#include "Map.h"

TileManager::TileManager() : tiles() {
	Tile empty;
	empty.c = ' ';
	tiles.push_back(empty);

	Tile wall;
	wall.c = ' ';
	wall.bg = TCODColor::white;
	wall.color = TCODColor::black;
	wall.type = wall.wall;
	tiles.push_back(wall);

	Tile floor;
	floor.c = '.';
	floor.bg = TCODColor::black;
	floor.color = TCODColor::white;
	floor.type = floor.floor;
	tiles.push_back(floor);

	Tile wallNB;
	wallNB.c = ' ';
	wallNB.bg = TCODColor::white;
	wallNB.color = TCODColor::black;
	wallNB.type = wall.wall;
	wallNB.isBlocking = false;
	tiles.push_back(wallNB);

	Tile dirt;
	dirt.c = ' ';
	dirt.bg = TCODColor::darkOrange;
	dirt.color = TCODColor::yellow;
	dirt.type = wall.wall;
	dirt.isBlocking = true;
	tiles.push_back(dirt);

	Tile grass;
	grass.c = '.';
	grass.bg = TCODColor::darkGreen;
	grass.color = TCODColor::white;
	grass.type = wall.wall;
	grass.isBlocking = true;
	tiles.push_back(grass);

	Tile stone;
	stone.c = ' ';
	stone.bg = TCODColor::grey;
	stone.color = TCODColor::grey;
	stone.type = wall.wall;
	stone.isBlocking = true;
	tiles.push_back(stone);

	Tile water;
	water.c = ' ';
	water.bg = TCODColor::blue;
	water.color = TCODColor::white;
	water.type = wall.liquid;
	water.isBlocking = false;
	tiles.push_back(water);

	Tile wood;
	wood.c = '%';
	wood.bg = TCODColor::darkestOrange;
	wood.color = TCODColor::darkestYellow;
	wood.type = wall.wall;
	wood.isBlocking = false;
	tiles.push_back(wood);

	Tile leaves;
	leaves.c = '/';
	leaves.bg = TCODColor::darkestGreen;
	leaves.color = TCODColor::darkGrey;
	leaves.type = wall.wall;
	leaves.isBlocking = false;
	tiles.push_back(leaves);

	Tile sand;
	leaves.c = '.';
	leaves.bg = TCODColor::yellow;
	leaves.color = TCODColor::white;
	leaves.type = wall.wall;
	leaves.isBlocking = false;
	tiles.push_back(leaves);

	Tile cacti;
	cacti.c = 'O';
	cacti.bg = TCODColor::green;
	cacti.color = TCODColor::darkYellow;
	cacti.type = wall.wall;
	tiles.push_back(cacti);
}

Tile* TileManager::GetTileData(TileID tile) {
	if (tile < tiles.size() && tile > -1)
		return &tiles.at(tile);
	return nullptr;
}