#include "Entity.h"

#include "engine.hpp"

Entity::Entity()
{

}

void Ai::Move(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	entity->pos.w = x;
	entity->pos.h = y;
	entity->pos.d = z;
}

void Ai::MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	entity->pos.w += x;
	entity->pos.h += y;
	entity->pos.d += z;
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
		break;
	case TCODK_DOWN:
		MoveRelative(entity,  0,  1,  0);
		break;
	case TCODK_LEFT:
		MoveRelative(entity, -1,  0,  0);
		break;
	case TCODK_RIGHT:
		MoveRelative(entity,  1,  0,  0);
		break;
	case TCODK_PAGEUP:
		MoveRelative(entity,  0,  0,  1);
		break;
	case TCODK_PAGEDOWN:
		MoveRelative(entity,  0,  0, -1);
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
	}
}
