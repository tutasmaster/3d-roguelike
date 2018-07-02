#pragma once
#include "libtcod.h"
#include "Map.h"

#include <string>
#include <memory>

class Entity;

class Ai {
public:
	virtual void OnTick(std::shared_ptr<Entity> entity) = 0;
	void Move(std::shared_ptr<Entity> entity, int x, int y, int z);
	void MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z);
};

class PlayerAi : public Ai {
public:
	void OnTick(std::shared_ptr<Entity> entity) override;
	bool isDigging = false;
	bool isBuilding = false;
	bool isBuildingNB = false;
};

class Entity
{
public:
	Entity();

	Map::Pos pos;

	std::shared_ptr<Ai> ai;

	std::string name = "Entity";
	char c = 'E';
	TCODColor col = TCODColor::yellow;
};