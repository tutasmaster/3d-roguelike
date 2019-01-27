#include "TileManager.h"
#include "Map.h"
#include <iostream>

TileManager::TileManager() : tiles(), grounds() {
	/*
	I   |    TILE
		    
	0   |   Empty
	1   |    Wall
	2   |  WallNB
	3   |    Dirt
	4   |   Grass
	5   |   Stone
	6   |   Water
	7   |    Wood
	8   |  Leaves
	9   |    Sand
	10  |   Cacti
	11  |   Cloud
	12  |   Glass
	*/

	tiles.push_back({ Tile::empty });
	tiles.push_back({ Tile::wall   , TCODColor::white         , TCODColor::black         , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::wall   , TCODColor::white         , TCODColor::black         , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::wall   , TCODColor::darkOrange    , TCODColor::darkOrange    , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::wall   , TCODColor::darkerGreen   , TCODColor::darkerGreen   , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::wall   , TCODColor::grey          , TCODColor::grey          , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::liquid , TCODColor::blue          , TCODColor::lightCyan     , 20 , 2    , true  , '~' });
	tiles.push_back({ Tile::wall   , TCODColor::darkestOrange , TCODColor::darkestYellow , 20 , 1    , false , '%' });
	tiles.push_back({ Tile::wall   , TCODColor::darkestGreen  , TCODColor::darkGrey      , 20 , 1    , false , '/' });
	tiles.push_back({ Tile::wall   , TCODColor::darkerYellow  , TCODColor::darkerYellow  , 20 , 1    , false , ' ' });
	tiles.push_back({ Tile::wall   , TCODColor::green         , TCODColor::darkYellow    , 20 , 1    , false , 'O' });
	tiles.push_back({ Tile::liquid , TCODColor::white         , TCODColor::white         , 20 , 0.8f , true  , '~' });
	tiles.push_back({ Tile::wall   , TCODColor::white         , TCODColor::black         , 20 , 0.2f , true  , ' ' });

	//GROUND TYPES

	/*Empty Ground*/{ 
	Ground empty;
	empty.c = ' ';
	empty.type = empty.empty;
	grounds.push_back(empty);}
	/*Wall  Ground*/ {
	Ground wall;
	wall.c = ' ';
	wall.bg = TCODColor::white;
	grounds.push_back(wall);}
	/*Dirt  Ground*/ {
	Ground dirt;
	dirt.c = ' ';
	dirt.bg = TCODColor::darkOrange;
	grounds.push_back(dirt); }
	/*Grass Ground*/{  
	Ground grass;
	grass.c = '.';
	grass.color = TCODColor::white;
	grass.bg = TCODColor::green;
	grass.type = grass.wall;
	grounds.push_back(grass); }
	/*Stone Ground*/ {
	Ground stone;
	stone.c = ' ';
	stone.bg = TCODColor::white;
	grounds.push_back(stone); }
	/*Water Ground*/ {
		Ground stone;
		stone.c = ' ';
		stone.bg = TCODColor::white;
		grounds.push_back(stone); }
	/*Wood Ground*/ {
		Ground stone;
		stone.c = ' ';
		stone.bg = TCODColor::white;
		grounds.push_back(stone); }
	/*Leaves Ground*/ {
		Ground stone;
		stone.c = ' ';
		stone.bg = TCODColor::white;
		grounds.push_back(stone); }
	/*Sand Ground*/ {
		Ground stone;
		stone.c = '.';
		stone.bg = TCODColor::darkYellow;
		grounds.push_back(stone); }
	/*Cacti Ground*/ {
		Ground stone;
		stone.c = ' ';
		stone.bg = TCODColor::white;
		grounds.push_back(stone); }

}

Tile* TileManager::GetTileData(TileID tile) {
	if (tile < tiles.size() && tile > -1)
		return &tiles.at(tile);
	return nullptr;
}

Ground* TileManager::GetGroundData(GroundID ground) {
	if (ground < grounds.size() && ground > -1)
		return &grounds.at(ground);
	std::cout << "out of bounds\n" << grounds.size() << ":" << (int)ground << "\n";
	return nullptr;
}