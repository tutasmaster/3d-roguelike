#include "Entity.h"

#include "engine.hpp"

Entity::Entity()
{

}

void Ai::Move(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	Map::Pos p(x, y, z);
	if  (engine.map.isTilePosValid(p) && (!entity->isColliding || engine.map.GetTileAt(p)->type != TileManager::wall) && !engine.checkEntityCollisionAtPos(p)){
		entity->pos = p;
	}
}

void Ai::MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	Move(entity, x + entity->pos.w, y + entity->pos.h, z + entity->pos.d);
}

void Ai::Follow(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> follower, int step = 1) {
	Map::Pos movement;
	
	if (entity->pos.w < follower->pos.w) {
		movement.w = step;
	}
	else if(entity->pos.w > follower->pos.w) {
		movement.w = -step;
	}

	if (entity->pos.h < follower->pos.h) {
		movement.h = step;
	}
	else if (entity->pos.h > follower->pos.h) {
		movement.h = -step;
	}

	if (entity->pos.d < follower->pos.d) {
		movement.d = step;
	}
	else if (entity->pos.d > follower->pos.d) {
		movement.d = -step;
	}

	MoveRelative(entity, movement.w, movement.h, movement.d);
}

void FriendlyAi::OnTick(std::shared_ptr<Entity> entity) {
	Follow(entity, engine.player);
}

void PlayerAi::OnTick(std::shared_ptr<Entity> entity)
{
	auto lastKey = TCODConsole::root->checkForKeypress(TCOD_KEY_PRESSED);

	if (isDigging)
		engine.map.SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::empty);
	else if (isBuilding)
		engine.map.SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::wall);
	else if (isBuildingNB)
		engine.map.SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::wallNB);

	switch (lastKey.vk) {
	case TCODK_UP:
		MoveRelative(entity,  0, -1,  0);
		hasUpdated = true;
		break;
	case TCODK_DOWN:
		MoveRelative(entity,  0,  1,  0);
		hasUpdated = true;
		break;
	case TCODK_LEFT:
		MoveRelative(entity, -1,  0,  0);
		hasUpdated = true;
		break;
	case TCODK_RIGHT:
		MoveRelative(entity,  1,  0,  0);
		hasUpdated = true;
		break;
	case TCODK_PAGEUP:
		MoveRelative(entity,  0,  0,  1);
		hasUpdated = true;
		break;
	case TCODK_PAGEDOWN:
		MoveRelative(entity,  0,  0, -1);
		hasUpdated = true;
		break;
	case TCODK_DELETE:
		isDigging = !isDigging;
		break;
	case TCODK_INSERT:
		isBuilding = !isBuilding;
		break;
	case TCODK_END:
		isBuildingNB = !isBuildingNB;
		break;
	case TCODK_HOME:
		entity->isColliding = !entity->isColliding;
		break;
	default:
		hasUpdated = false;
		break;
	}

}
