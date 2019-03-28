#pragma once

#include <libtcod.hpp>

#include <vector>

#include "TileManager.h"

class Map
{
public:
	class Pos {
	public:
		Pos(int w = 0, int y = 0, int z = 0) : x(w), y(y), z(z) {}

		Pos operator+(const Pos& p);
		Pos operator+(int v);
		Pos operator*(int f);
		bool operator==(const Pos&b);

		int x;
		int y;
		int z;
	};

	Map(int w, int y, int z);
	~Map();

	Tile* GetTileAt(Pos p);
	Tile* GetTileAt(int w, int y, int z);

	bool SetAt(const Pos p, TileID tile, GroundID ground);
	bool SetAt(const int w, const int y, const int z, TileID tile, GroundID ground);

	bool SetTileAt(const Pos p, TileID tile);
	bool SetTileAt(const int w, const int y, const int z, TileID tile);

	bool isPosValid(const Pos p);
	bool isPosValid(const int w, const int y, const int z);

	bool isTilePosWalkable(const Pos p);
	bool isTilePosWalkable(const int w, const int y, const int z);

	
	Ground* GetGroundAt(Pos p);
	Ground* GetGroundAt(int w, int y, int z);

	bool SetGroundAt(const Pos p, GroundID tile);
	bool SetGroundAt(const int w, const int y, const int z, GroundID tile);



	void GenerateTerrain(int type);

	TileID * arr;
	GroundID * groundArr;

	int width, height, depth;

};

void SetMapLayer(Map& m, int l, TileID tile);

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int z, TileID tile);

float GetDistance(int x, int y, int z, int x1, int y1, int z1);
float GetDistance(Map::Pos pos, Map::Pos pos1);