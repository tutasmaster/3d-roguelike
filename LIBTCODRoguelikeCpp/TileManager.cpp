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
	dirt.isBlocking = false;
	tiles.push_back(dirt);

	Tile grass;
	grass.c = ' ';
	grass.bg = TCODColor::green;
	grass.color = TCODColor::lightGreen;
	grass.type = wall.wall;
	grass.isBlocking = false;
	tiles.push_back(grass);

	Tile stone;
	stone.c = ' ';
	stone.bg = TCODColor::grey;
	stone.color = TCODColor::grey;
	stone.type = wall.wall;
	stone.isBlocking = false;
	tiles.push_back(stone);
}

Tile* TileManager::GetTileData(TileID tile) {
	if (tile < tiles.size() && tile > -1)
		return &tiles.at(tile);
	return nullptr;
}