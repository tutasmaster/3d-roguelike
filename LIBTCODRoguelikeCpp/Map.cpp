#include "Map.h"

#include "MapGenerators.h"

#include "engine.hpp"

Map::Map(int w = 255, int y = 255, int z = 255) : width(w) , height(y) , depth(z)
{
	arr = new TileID[w*y*z];
	groundArr = new GroundID[w*y*z];

	for (int i = 0; i < w*y*z; i++) {
		arr[i] = 0;
		groundArr[i] = 0;
	}
}


Map::~Map()
{
	delete[] arr;
}

Tile* Map::GetTileAt(Pos pos) {
	if ((pos.x > -1 && pos.y > -1 && pos.z > -1) &&
		(pos.x < width && pos.y < height && pos.z < depth))
		return tileManager.GetTileData(arr[pos.x + (pos.y * width) + (pos.z * width * height)]);
	return nullptr;
}

Tile* Map::GetTileAt(int w, int y, int z) {
	if ((w > -1 && y > -1 && z > -1) &&
		(w < width && y < height && z < depth))
		return tileManager.GetTileData(arr[w + (y * width) + (z * width * height)]);
	return nullptr;
}

bool Map::SetAt(const Pos p, TileID tile, GroundID ground) {
	if (isPosValid(p)) {
		groundArr[(p.x) + ((p.y)*width) + ((p.z)*height * width)] = ground;
		arr[(p.x) + ((p.y)*width) + ((p.z)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetAt(const int w, const int y, const int z, TileID tile, GroundID ground) {
	if (isPosValid(w, y, z)) {
		groundArr[(w)+((y)*width) + ((z)*height * width)] = ground;
		arr[(w)+((y)*width) + ((z)*height * width)] = tile;
		return true;
	}
	return false;
}


bool Map::SetTileAt(const Pos p, TileID tile) {
	if (isPosValid(p)) {
		arr[(p.x) + ((p.y)*width) + ((p.z)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetTileAt(const int w, const int y, const int z, TileID tile) {
	if(isPosValid(w,y,z)){
		arr[(w) + ((y)*width) + ((z)*height * width)] = tile;
		return true;
	}
	return false;
}



bool Map::isPosValid(Pos p) {
	return ((p.x > -1 && p.y > -1 && p.z > -1) && (p.x < width && p.y < height && p.z < depth));
}

bool Map::isPosValid(int w, int y, int z) {
	return ((w > -1 && y > -1 && z > -1) && (w < width && y < height && z < depth));
}

bool Map::isTilePosWalkable(Pos p) {
	return (isPosValid(p) &&
		GetTileAt(p)->type != TileManager::tile_wall &&
		!engine.checkEntityCollisionAtPos(p));
}

bool Map::isTilePosWalkable(int w, int y, int z) {
	Pos p(w, y, z);
	return isTilePosWalkable(p);
}


Ground* Map::GetGroundAt(Pos pos) {
	if ((pos.x > -1 && pos.y > -1 && pos.z > -1) &&
		(pos.x < width && pos.y < height && pos.z < depth))
		return tileManager.GetGroundData(groundArr[pos.x + (pos.y * width) + (pos.z * width * height)]);
	return nullptr;
}

Ground* Map::GetGroundAt(int w, int y, int z) {
	if ((w > -1 && y > -1 && z > -1) &&
		(w < width && y < height && z < depth))
		return tileManager.GetGroundData(groundArr[w + (y * width) + (z * width * height)]);
	return nullptr;
}


bool Map::SetGroundAt(const Pos p, GroundID tile) {
	if (isPosValid(p)) {
		groundArr[(p.x) + ((p.y)*width) + ((p.z)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetGroundAt(const int w, const int y, const int z, GroundID tile) {
	if (isPosValid(w, y, z)) {
		groundArr[(w)+((y)*width) + ((z)*height * width)] = tile;
		return true;
	}
	return false;
}




Map::Pos Map::Pos::operator+(const Pos& p) {
	Pos v(0,0,0);

	v.x = p.x + this->x;
	v.y = p.y + this->y;
	v.z = p.z + this->z;

	return v;
}

Map::Pos Map::Pos::operator+(int v) {
	Pos n(0, 0, 0);

	n.x = v + this->x;
	n.y = v + this->y;
	n.z = v + this->z;

	return n;
}

Map::Pos Map::Pos::operator*(int f) {
	Pos n(0, 0, 0);

	n.x = f * this->x;
	n.y = f * this->y;
	n.z = f * this->z;

	return n;
}

bool Map::Pos::operator==(const Pos&b) {
	return (x == b.x && y == b.y && z == b.z);
}

void SetMapLayer(Map& m, int l, TileID tile) {
	for (int j = 0; j < m.height; j++) {
		for (int i = 0; i < m.width; i++) {
			m.SetTileAt(i, j, l, tile);
		}
	}
}

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int z, TileID tile) {
	for (int j = y; j < h + y; j++) {
		for (int i = x; i < w + x; i++) {
			m.SetTileAt(i, j, z, tile);
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

float GetDistance(int x, int y, int z, int x1, int y1, int z1) {
	float tempX = x1 - x;
	float tempY = y1 - y;
	float tempZ = z1 - z;

	return sqrt(tempX * tempX + tempY * tempY + tempZ * tempZ);
}

float GetDistance(Map::Pos pos, Map::Pos pos1) {
	float tempX = pos1.x - pos.x;
	float tempY = pos1.y - pos.y;
	float tempZ = pos1.z - pos.z;

	return sqrt(tempX * tempX + tempY * tempY + tempZ * tempZ);
}