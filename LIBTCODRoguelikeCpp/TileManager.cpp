#include "TileManager.h"
#include "Map.h"
#include <iostream>

TileManager::TileManager() : tiles(), grounds() {
	/**Empty Tile**/ 
	Tile tile_empty;
	tile_empty.c = ' ';
	tiles.push_back(tile_empty); 

	/**Wall  Tile**/ 
	Tile tile_wall;
	tile_wall.c = ' ';
	tile_wall.bg = TCODColor::white;
	tile_wall.color = TCODColor::black;
	tile_wall.type = tile_wall.wall;
	tiles.push_back(tile_wall); 
	/*WallNB  Tile*/ 
	Tile wallNB;
	wallNB.c = ' ';
	wallNB.bg = TCODColor::white;
	wallNB.color = TCODColor::black;
	wallNB.type = wallNB.wall;
	wallNB.isBlocking = false;
	tiles.push_back(wallNB); 
	/**Dirt  Tile**/ 
	Tile dirt;
	dirt.c = ' ';
	dirt.bg = TCODColor::darkOrange;
	dirt.color = TCODColor::yellow;
	dirt.type = dirt.wall;
	dirt.isBlocking = true;
	tiles.push_back(dirt); 
	/**Grass Tile**/ 
	Tile grass;
	grass.c = ' ';
	grass.bg = TCODColor::darkerGreen;
	grass.color = TCODColor::white;
	grass.type = grass.wall;
	grass.isBlocking = true;
	tiles.push_back(grass);
	
	/**Stone Tile**/ 	
	Tile stone; 
	stone.c = ' ';
	stone.bg = TCODColor::grey;
	stone.color = TCODColor::grey;
	stone.type = stone.wall;
	stone.isBlocking = true;
	tiles.push_back(stone); 
	/**Water Tile**/ 
	Tile water;
	water.c = ' ';
	water.bg = TCODColor::blue;
	water.color = TCODColor::white;
	water.type = water.liquid;
	water.isBlocking = false;
	tiles.push_back(water); 
	/**Wood  Tile**/ 
	Tile wood; 
	wood.c = '%';
	wood.bg = TCODColor::darkestOrange;
	wood.color = TCODColor::darkestYellow;
	wood.type = wood.wall;
	wood.isBlocking = false;
	tiles.push_back(wood); 
	/*Leaves  Tile*/ 
	Tile leaves;
	leaves.c = '/';
	leaves.bg = TCODColor::darkestGreen;
	leaves.color = TCODColor::darkGrey;
	leaves.type = leaves.wall;
	leaves.isBlocking = false;
	tiles.push_back(leaves); 
	/**Sand  Tile**/ 
	Tile sand;
	sand.c = ' ';
	sand.bg = TCODColor::darkerYellow;
	sand.color = TCODColor::white;
	sand.type = sand.wall;
	sand.isBlocking = false;
	tiles.push_back(sand); 
	/**Cacti Tile**/ 
	Tile cacti;
	cacti.c = 'O';
	cacti.bg = TCODColor::green;
	cacti.color = TCODColor::darkYellow;
	cacti.type = cacti.wall;
	cacti.isBlocking = false;
	tiles.push_back(cacti); 

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
	/*Sand Ground*/ uff{
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