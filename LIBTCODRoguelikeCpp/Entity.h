#pragma once
#include "libtcod.h"
#include "Map.h"
#include "ItemManager.h"

#include <string>
#include <memory>

class Entity;

class Ai {
public:
	bool hasUpdated = false;
	virtual void OnTick(std::shared_ptr<Entity> entity) = 0;
	bool Move(std::shared_ptr<Entity> entity, int x, int y, int z);
	bool MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z);
	bool OnMoveSideways(std::shared_ptr<Entity> entity, int x, int y);
	bool OnMoveSidewaysAtAngle(std::shared_ptr<Entity> entity, int x, int y, int angle);
	void Follow(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> follower, int step);
};

class FriendlyAi : public Ai {
	void OnTick(std::shared_ptr<Entity> entity) override;
};

class WanderingAi : public Ai {
	void OnTick(std::shared_ptr<Entity> entity) override;
	


	bool isFollowing = false;
	int nextPosX, nextPosY;
};

class PlayerAi : public Ai {
public:
	void OnTick(std::shared_ptr<Entity> entity) override;
	bool isDigging = false;
	bool isBuilding = false;
	bool isBuildingNB = false;
	bool isAttacking = false;
};

class WorldBuilderAi : public Ai {
public:
	WorldBuilderAi(std::shared_ptr<Entity> entity);
	void OnTick(std::shared_ptr<Entity> entity) override;

	enum BuildState {
		idle,
		parallelogram,
		sphere,
		cylinder
	} buildState;

	int args = 0;

	void OrderPositions();

	void Build();
	void BuildBlock();
	void BuildSphere();
	void BuildCylinder();

	//Arguments
	Map::Pos pos1;
	Map::Pos pos2;

	TileID block = TileManager::tile_wall;
};

class CastAi : public Ai {
public:
	void OnTick(std::shared_ptr<Entity> entity) override;

	enum Direction : unsigned char {
		up,down,left,right
	} dir;
};

class Renderer {
public:
	virtual void OnRender(std::shared_ptr<Entity> entity) = 0;
};

class WorldBuilderRenderer : public Renderer {
public:
	void OnRender(std::shared_ptr<Entity> entity) override;
};

class Physics {
public:
	float relPosX, relPosY, relPosZ;
	float speedX,speedY,speedZ,gravity;
	float drag, airDrag;
	virtual void ApplyPhysics(std::shared_ptr<Entity> entity) = 0;
};

class BoulderPhysics : public Physics{
public:
	void ApplyPhysics(std::shared_ptr<Entity> entity) override;
};

class Metabolism {
public:
	std::shared_ptr<Entity> entity;

	Metabolism(std::shared_ptr<Entity> entity);

	int HP = 100;
	int DEF = 2;

	int DoDamage(int ATK);
	void Die();

	bool isDead = false;
};

class Weapon {
public:
	int ATK = 50;
	std::string name = "Sword";
};

class Inventory {
public:
	Inventory();
	std::vector<std::pair<ItemID, unsigned char>> item_vector;
	void DeleteRemainingItems();
	virtual void PickupItem(Map::Pos p);
	void AddItem(int itemID);
};

class PlayerInventory : public Inventory{
	void PickupItem(Map::Pos p) override;
};

class Entity
{
public:
	Entity();
	~Entity();

	Map::Pos pos;

	std::shared_ptr<Ai> ai;
	std::shared_ptr<Metabolism> meta;
	std::shared_ptr<Inventory> inv;
	std::shared_ptr<Renderer> ren;
	std::shared_ptr<Physics> phys;
	std::shared_ptr<Weapon> wep;

	std::string name = "Entity";
	char c = 'E';
	TCODColor col = TCODColor::yellow;

	bool isItem = false;
	int itemID = 0;
	bool isColliding = true;
};
