#pragma once
#include "libtcod.h"
#include "Map.h"

#include <string>
#include <memory>

class Entity;

class Ai {
public:
	bool hasUpdated = false;
	virtual void OnTick(std::shared_ptr<Entity> entity) = 0;
	void Move(std::shared_ptr<Entity> entity, int x, int y, int z);
	void MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z);
	void Follow(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> follower, int step);
};

class FriendlyAi : public Ai {
	void OnTick(std::shared_ptr<Entity> entity) override;
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

	bool isColliding = true;
};