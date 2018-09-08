#pragma once

#include <libtcod.hpp>

#include <vector>

#include "TileManager.h"

class Map
{
public:
	class Pos {
	public:
		Pos(int w = 0, int h = 0, int d = 0) : w(w), h(h), d(d) {}

		Pos operator+(const Pos& p);
		Pos operator+(int v);
		Pos operator*(int f);
		bool operator==(const Pos&b);

		int w;
		int h;
		int d;
	};

	Map(int w, int h, int d);
	~Map();

	Tile* GetTileAt(Pos p);
	Tile* GetTileAt(int w, int h, int d);

	bool SetAt(const Pos p, TileID tile, GroundID ground);
	bool SetAt(const int w, const int h, const int d, TileID tile, GroundID ground);

	bool SetTileAt(const Pos p, TileID tile);
	bool SetTileAt(const int w, const int h, const int d, TileID tile);

	bool isPosValid(const Pos p);
	bool isPosValid(const int w, const int h, const int d);

	bool isTilePosWalkable(const Pos p);
	bool isTilePosWalkable(const int w, const int h, const int d);

	
	Ground* GetGroundAt(Pos p);
	Ground* GetGroundAt(int w, int h, int d);

	bool SetGroundAt(const Pos p, GroundID tile);
	bool SetGroundAt(const int w, const int h, const int d, GroundID tile);



	void GenerateTerrain(int type);

	TileID * arr;
	GroundID * groundArr;

	int width, height, depth;

};

void SetMapLayer(Map& m, int l, TileID tile);

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int d, TileID tile);