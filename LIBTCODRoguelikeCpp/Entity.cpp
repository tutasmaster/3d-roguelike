#include "Entity.h"
#include <algorithm>
#include "engine.hpp"



Entity::Entity()
{

}

Entity::~Entity()
{

}

Metabolism::Metabolism(std::shared_ptr<Entity> entity) : entity(entity) {

}

void Metabolism::Die() {
	entity->c = '.';
	entity->ai = nullptr;
}

int Metabolism::DoDamage(int ATK) {
	if (ATK > DEF && HP != 0) {
		HP = HP - (ATK - DEF) > 0 ? HP - (ATK - DEF) : 0;
		std::string s = entity->name + " now has " + std::to_string(HP) + "HP.";

		Message msg{ s, TCODColor::red };
		engine.console.push_back(msg);

		if (HP == 0) {
			entity->c = '.';
			isDead = true;
		}
	}
	else if (HP == 0) {
		std::string s = "" + entity->name + " is dead. Stop hitting it.";
		Message msg{ s, TCODColor::red };
		engine.console.push_back(msg);
	}
}


void WorldBuilderRenderer::OnRender(std::shared_ptr<Entity> entity) {
	TCODConsole::root->print(0, 0, std::to_string((std::static_pointer_cast<WorldBuilderAi>(entity->ai)->block)).c_str());
}

void BoulderPhysics::ApplyPhysics(std::shared_ptr<Entity> entity) {
	if (engine.map->GetTileAt(Map::Pos(entity->pos.w, entity->pos.h, entity->pos.d - 1)) != nullptr && engine.map->GetTileAt(Map::Pos(entity->pos.w, entity->pos.h, entity->pos.d - 1))->type != TileManager::tile_wall) {
		speedZ -= gravity;
	}

	speedX *= 1 - drag;
	speedY *= 1 - drag;
	speedZ *= 1 - drag;

	relPosX += speedX;
	relPosY += speedY;
	relPosZ += speedZ;

	if(engine.map->GetTileAt(entity->pos.w + relPosX, entity->pos.h + relPosY, entity->pos.d + relPosZ)->type == TileManager::tile_empty){
		entity->pos.w += (int)relPosX;
		entity->pos.h += (int)relPosY;
		entity->pos.d += (int)relPosZ;
	}

	relPosX = (int)relPosX % 1;
	relPosY = (int)relPosY % 1;
	relPosZ = (int)relPosZ % 1;
}

Inventory::Inventory() {

}

void Inventory::DeleteRemainingItems() {
	int i = 0;
	for (auto &it : item_vector) {
		if (it.second <= 0) {
			item_vector.erase(item_vector.begin() + i);
		}
		i++;
	}
}

void Inventory::PickupItem(Map::Pos p) {
	int i = 0;
	for (auto &e : engine.npcs) {
		if (e->isItem) {
			AddItem(e->itemID);
			engine.npcs.erase(engine.npcs.begin() + i);
			break;
		}
		i++;
	}
}

void Inventory::AddItem(int itemID) {
	bool hasItem = false;
	for (auto &i : item_vector) {
		if (i.first == itemID && i.second < 255){
			i.second++;
			hasItem = true;
		}
	}

	if (!hasItem) {
		item_vector.push_back(std::make_pair(itemID, 1));
	}
}

void PlayerInventory::PickupItem(Map::Pos p) {
	int i = 0;
	for (auto &e : engine.npcs) {
		if (e->isItem) {
			AddItem(e->itemID);
			Message pkup;
			pkup.col = TCODColor::gold;
			pkup.msg = "You picked up a " + itemManager.GetItemData(e->itemID)->name + " off the ground!";
			engine.console.push_back(pkup);
			engine.npcs.erase(engine.npcs.begin() + i);
			break;
		}
		i++;
	}
}