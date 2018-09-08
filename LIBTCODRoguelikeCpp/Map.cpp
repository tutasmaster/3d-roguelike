#include "Map.h"

#include "MapGenerators.h"

#include "engine.hpp"

Map::Map(int w = 255, int h = 255, int d = 255) : width(w) , height(h) , depth(d)
{
	arr = new TileID[w*h*d];
	groundArr = new GroundID[w*h*d];

	for (int i = 0; i < w*h*d; i++) {
		arr[i] = 0;
		groundArr[i] = 0;
	}
}


Map::~Map()
{
	delete[] arr;
}

Tile* Map::GetTileAt(Pos pos) {
	if ((pos.w > -1 && pos.h > -1 && pos.d > -1) &&
		(pos.w < width && pos.h < height && pos.d < depth))
		return tileManager.GetTileData(arr[pos.w + (pos.h * width) + (pos.d * width * height)]);
	return nullptr;
}

Tile* Map::GetTileAt(int w, int h, int d) {
	if ((w > -1 && h > -1 && d > -1) &&
		(w < width && h < height && d < depth))
		return tileManager.GetTileData(arr[w + (h * width) + (d * width * height)]);
	return nullptr;
}

bool Map::SetAt(const Pos p, TileID tile, GroundID ground) {
	if (isPosValid(p)) {
		groundArr[(p.w) + ((p.h)*width) + ((p.d)*height * width)] = ground;
		arr[(p.w) + ((p.h)*width) + ((p.d)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetAt(const int w, const int h, const int d, TileID tile, GroundID ground) {
	if (isPosValid(w, h, d)) {
		groundArr[(w)+((h)*width) + ((d)*height * width)] = ground;
		arr[(w)+((h)*width) + ((d)*height * width)] = tile;
		return true;
	}
	return false;
}


bool Map::SetTileAt(const Pos p, TileID tile) {
	if (isPosValid(p)) {
		arr[(p.w) + ((p.h)*width) + ((p.d)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetTileAt(const int w, const int h, const int d, TileID tile) {
	if(isPosValid(w,h,d)){
		arr[(w) + ((h)*width) + ((d)*height * width)] = tile;
		return true;
	}
	return false;
}



bool Map::isPosValid(Pos p) {
	return ((p.w > -1 && p.h > -1 && p.d > -1) && (p.w < width && p.h < height && p.d < depth));
}

bool Map::isPosValid(int w, int h, int d) {
	return ((w > -1 && h > -1 && d > -1) && (w < width && h < height && d < depth));
}

bool Map::isTilePosWalkable(Pos p) {
	return (isPosValid(p) &&
		GetTileAt(p)->type != TileManager::tile_wall &&
		!engine.checkEntityCollisionAtPos(p));
}

bool Map::isTilePosWalkable(int w, int h, int d) {
	Pos p(w, h, d);
	return isTilePosWalkable(p);
}


Ground* Map::GetGroundAt(Pos pos) {
	if ((pos.w > -1 && pos.h > -1 && pos.d > -1) &&
		(pos.w < width && pos.h < height && pos.d < depth))
		return tileManager.GetGroundData(groundArr[pos.w + (pos.h * width) + (pos.d * width * height)]);
	return nullptr;
}

Ground* Map::GetGroundAt(int w, int h, int d) {
	if ((w > -1 && h > -1 && d > -1) &&
		(w < width && h < height && d < depth))
		return tileManager.GetGroundData(groundArr[w + (h * width) + (d * width * height)]);
	return nullptr;
}


bool Map::SetGroundAt(const Pos p, GroundID tile) {
	if (isPosValid(p)) {
		groundArr[(p.w) + ((p.h)*width) + ((p.d)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetGroundAt(const int w, const int h, const int d, GroundID tile) {
	if (isPosValid(w, h, d)) {
		groundArr[(w)+((h)*width) + ((d)*height * width)] = tile;
		return true;
	}
	return false;
}




Map::Pos Map::Pos::operator+(const Pos& p) {
	Pos v(0,0,0);

	v.w = p.w + this->w;
	v.h = p.h + this->h;
	v.d = p.d + this->d;

	return v;
}

Map::Pos Map::Pos::operator+(int v) {
	Pos n(0, 0, 0);

	n.w = v + this->w;
	n.h = v + this->h;
	n.d = v + this->d;

	return n;
}

Map::Pos Map::Pos::operator*(int f) {
	Pos n(0, 0, 0);

	n.w = f * this->w;
	n.h = f * this->h;
	n.d = f * this->d;

	return n;
}

bool Map::Pos::operator==(const Pos&b) {
	return (w == b.w && h == b.h && d == b.d);
}

void SetMapLayer(Map& m, int l, TileID tile) {
	for (int j = 0; j < m.height; j++) {
		for (int i = 0; i < m.width; i++) {
			m.SetTileAt(i, j, l, tile);
		}
	}
}

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int d, TileID tile) {
	for (int j = y; j < h + y; j++) {
		for (int i = x; i < w + x; i++) {
			m.SetTileAt(i, j, d, tile);
		}
	}
}

void Map::GenerateTerrain(int type) {
	switch (type) {
	case 0:
		GeneratePerlinNoiseMap(this);
		break;
	case 1:
		GeneratePerlinFBMNoiseMap(this);
		break;
	case 2:
		GeneratePerlinTurbulenceNoiseMap(this);
		break;
	case 3:
		GeneratePerlinRigidNoiseMap(this);
		break;
	case 4:
		GenerateSimplexNoiseMap(this);
		break;
	case 5:
		GenerateSimplexFBMNoiseMap(this);
		break;
	case 6:
		GenerateSimplexTurbulenceNoiseMap(this);
		break;
	case 7:
		GenerateWaveletNoiseMap(this);
		break;
	case 8:
		GenerateWaveletFBMNoiseMap(this);
		break;
	case 9:
		GenerateWaveletTurbulenceNoiseMap(this);
		break;
	}
}

