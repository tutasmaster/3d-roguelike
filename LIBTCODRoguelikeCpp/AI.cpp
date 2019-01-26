#include "Entity.h"
#include <algorithm>
#include "engine.hpp"

bool Ai::Move(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	Map::Pos p(x, y, z);
	if (engine.map->isPosValid(p) && (!entity->isColliding || engine.map->GetTileAt(p)->type != TileManager::tile_wall) && !engine.checkEntityCollisionAtPos(p)) {
		entity->pos = p;
		return true;
	}
	return false;
}

bool Ai::MoveRelative(std::shared_ptr<Entity> entity, int x, int y, int z)
{
	return Move(entity, x + entity->pos.w, y + entity->pos.h, z + entity->pos.d);
}

void Ai::Follow(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> follower, int step = 1) {
	Map::Pos movement;

	if (entity->pos.w < follower->pos.w) {
		movement.w = step;
	}
	else if (entity->pos.w > follower->pos.w) {
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

bool Ai::OnMoveSideways(std::shared_ptr<Entity> entity, int x, int y)
{
	Map::Pos p(entity->pos.w + x, entity->pos.h + y, entity->pos.d);

	if (entity->isColliding) {
		Map::Pos top(entity->pos.w + x, entity->pos.h + y, entity->pos.d + 1);
		Map::Pos bot(entity->pos.w + x, entity->pos.h + y, entity->pos.d - 1);
		Map::Pos botB(entity->pos.w + x, entity->pos.h + y, entity->pos.d - 2);
		Map::Pos topP(entity->pos.w, entity->pos.h, entity->pos.d + 1);

		bool cB, tB, bB, bbB, tpB; //yarr yarr fiddle dee dee, shitty variable names is alright with me.

		cB = engine.map->isTilePosWalkable(p);

		tB = engine.map->isTilePosWalkable(top);
		bB = engine.map->isTilePosWalkable(bot);
		bbB = engine.map->isTilePosWalkable(botB);
		tpB = engine.map->isTilePosWalkable(topP);

		if (!bbB && bB && cB)
			entity->pos = bot;
		else if (!bB && cB)
			entity->pos = p;
		else if (tB && tpB && !cB)
			entity->pos = top;
		else if (cB && bB && bbB) {
			/*Message msg;
			msg.msg = "You tried to end it all. But failed.";
			msg.col = TCODColor::red;

			engine.console.push_back(msg);*/
			return false;
		}
		return true;

	}
	else {
		entity->pos = p;
		return true;
	}
}

bool Ai::OnMoveSidewaysAtAngle(std::shared_ptr<Entity> ptr, int x, int y, int angle) {
	switch (angle) {
	case 0:
		return OnMoveSideways(ptr, x, y);
		break;
	case 1:
		return OnMoveSideways(ptr, -y, x);
		break;
	case 2:
		return OnMoveSideways(ptr, -x, -y);
		break;
	case 3:
		return OnMoveSideways(ptr, y, -x);
		break;
	default:
		return false;
		break;
	}
}

void FriendlyAi::OnTick(std::shared_ptr<Entity> entity) {
	Follow(entity, engine.player);
}

void WanderingAi::OnTick(std::shared_ptr<Entity> entity) {
	if (!isFollowing) {
		int x = entity->pos.w - ((rand() % 10) - 5);
		int y = entity->pos.h - ((rand() % 10) - 5);

		nextPosX = x;
		nextPosY = y;

		isFollowing = true;


	}
	else {

		int relativeX = nextPosX - entity->pos.w;
		int relativeY = nextPosY - entity->pos.h;

		if (relativeX > 0)
			relativeX = 1;
		else if (relativeX < 0)
			relativeX = -1;
		else
			relativeX = 0;

		if (relativeY > 0)
			relativeY = 1;
		else if (relativeY < 0)
			relativeY = -1;
		else
			relativeY = 0;

		if (!OnMoveSideways(entity, relativeX, relativeY)) {
			isFollowing = false;
		}

		if (entity->pos.w == nextPosX && entity->pos.h == nextPosY) {
			isFollowing = false;
		}
	}
}

void PlayerAi::OnTick(std::shared_ptr<Entity> entity)
{
	auto lastKey = engine.keyboardInput.pressed ? engine.keyboardInput : TCOD_key_t();

	if (engine.mouseInput.lbutton && engine.mouseInput.cx > 26 && engine.mouseInput.cx < 38 && engine.mouseInput.cy > 26 && engine.mouseInput.cy < 38) {
		engine.map->SetTileAt(Map::Pos(engine.player->pos.w + engine.mouseInput.cx - 32, engine.player->pos.h + engine.mouseInput.cy - 32, engine.player->pos.d),
			tileManager.tile_empty);

		engine.map->SetGroundAt(Map::Pos(engine.player->pos.w + engine.mouseInput.cx - 32, engine.player->pos.h + engine.mouseInput.cy - 32, engine.player->pos.d + 1),
			tileManager.ground_empty);
	}

	if (isDigging)
		engine.map->SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::tile_empty);
	else if (isBuilding)
		engine.map->SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::tile_wall);
	else if (isBuildingNB)
		engine.map->SetTileAt(entity->pos.w, entity->pos.h, entity->pos.d, TileManager::tile_wallNB);


		/*case 0:
			return OnMoveSideways(ptr, x, y);
			break;
		case 1:
			return OnMoveSideways(ptr, -y, x);
			break;
		case 2:
			return OnMoveSideways(ptr, -x, -y);
			break;
		case 3:
			return OnMoveSideways(ptr, y, -x);
			break;*/

		if(isAttacking){


			int enemyY = 0;
			int enemyX = 0;

			switch (lastKey.vk){
				case TCODK_UP:
					enemyY = -1;
					isAttacking = false;
					break;
				case TCODK_LEFT:
					enemyX = -1;
					isAttacking = false;
					break;
				case TCODK_RIGHT:
					enemyX = 1;
					isAttacking = false;
					break;
				case TCODK_DOWN:
					enemyY = 1;
					isAttacking = false;
					break;
			}

			for (auto &npc : engine.npcs) {
				if (npc->meta != nullptr && npc->pos == entity->pos + Map::Pos(enemyX, enemyY, 0)) {
					if (entity->wep != nullptr) {
						npc->meta->DoDamage(entity->wep->ATK);
						
						Message msg{ "You hit the " + npc->name + " with the " + entity->wep->name + " for " + std::to_string(entity->wep->ATK) + "!" };
						engine.console.push_back(msg);
					}else{
						npc->meta->DoDamage(20);
						Message msg{ "You punch the " + npc->name + "!", TCODColor::red };
						engine.console.push_back(msg);
					}
					break;
				}
			}
			return;
	}

	switch (lastKey.c) {
	case 'i':
		engine.GUI_ID = 0;
		break;
	case 'd':
		engine.GUI_ID = 1;
		break;
	case 'a':
		engine.GUI_ID = 2;
		break;
	case 'g':
		entity->inv->PickupItem(entity->pos);
		break;
	case 'c':
		isAttacking = true;
		break;
		/*case 'c':
			engine.GUI_ID = 3;
			break;*/
	}

	/*std::shared_ptr<Effect> f = std::make_shared<FireEffect>();
	f->pos = entity->pos;*/
	switch (lastKey.vk) {
	case TCODK_SPACE:
		hasUpdated = true;
		break;
	case TCODK_UP:
		//engine.effects.push_back(f);
		OnMoveSidewaysAtAngle(entity, 0, -1, engine.angle);
		hasUpdated = true;
		break;
	case TCODK_DOWN:
		//engine.effects.push_back(f);
		OnMoveSidewaysAtAngle(entity, 0, 1, engine.angle);
		hasUpdated = true;
		break;
	case TCODK_LEFT:
		//engine.effects.push_back(f);
		OnMoveSidewaysAtAngle(entity, -1, 0, engine.angle);
		hasUpdated = true;
		break;
	case TCODK_RIGHT:
		//engine.effects.push_back(f);
		OnMoveSidewaysAtAngle(entity, 1, 0, engine.angle);
		hasUpdated = true;
		break;
	case TCODK_PAGEUP:
		MoveRelative(entity, 0, 0, 1);
		hasUpdated = true;
		break;
	case TCODK_PAGEDOWN:
		MoveRelative(entity, 0, 0, -1);
		hasUpdated = true;
		break;
		/*case TCODK_DELETE:
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
			break;*/
	case TCODK_F1:
		engineRenderer.layerSize--;
		break;
	case TCODK_F2:
		engineRenderer.layerSize++;
		break;
	case TCODK_F3:
		engine.angle++;
		if (engine.angle > 3)
			engine.angle = 0;
		break;
	case TCODK_F11:
		engine.saveMap();
		break;
	case TCODK_F12:
		engine.loadMap();
		break;
	default:
		hasUpdated = false;
		break;
	}



}

WorldBuilderAi::WorldBuilderAi(std::shared_ptr<Entity> entity) {
	entity->isColliding = false;
	entity->c = 'X';

	entity->ren = std::make_shared<WorldBuilderRenderer>();
}

void WorldBuilderAi::OnTick(std::shared_ptr<Entity> entity) {

	auto key = engine.keyboardInput.pressed ? engine.keyboardInput : TCOD_key_t();
	switch (key.vk) {

	case TCODK_F1:
		engineRenderer.layerSize--;
		break;
	case TCODK_F2:
		engineRenderer.layerSize++;
		break;
	case TCODK_F3:
		engine.angle++;
		if (engine.angle > 3)
			engine.angle = 0;
		break;

	case TCODK_UP:
		MoveRelative(entity, 0, -1, 0);
		break;
	case TCODK_DOWN:
		MoveRelative(entity, 0, 1, 0);
		break;
	case TCODK_LEFT:
		MoveRelative(entity, -1, 0, 0);
		break;
	case TCODK_RIGHT:
		MoveRelative(entity, 1, 0, 0);
		break;
	case TCODK_PAGEUP:
		MoveRelative(entity, 0, 0, 1);
		break;
	case TCODK_PAGEDOWN:
		MoveRelative(entity, 0, 0, -1);
		break;

	case TCODK_KPSUB:
		block--;
		break;

	case TCODK_KPADD:
		block++;
		break;

	case TCODK_ENTER:
		if (buildState != idle) {
			if (args == 0) {
				pos1 = entity->pos;
				args++;
			}
			else if (args == 1) {
				pos2 = entity->pos;
				args = 0;
				Build();
				buildState = idle;
				entity->c = 'X';
			}
		}
		break;
	case TCODK_F11:
		engine.saveMap();
		break;
	case TCODK_F12:
		engine.loadMap();
		break;
	}

	switch (key.c) {
	case 'b':
		buildState = BuildState::parallelogram;
		entity->c = 'b';
		break;
	case 's':
		buildState = BuildState::sphere;
		entity->c = 's';
		break;
	case 'c':
		buildState = BuildState::cylinder;
		entity->c = 'c';
		break;
	}
}

void WorldBuilderAi::Build() {
	switch (buildState) {
	case BuildState::parallelogram:
		OrderPositions();
		BuildBlock();
		break;
	case BuildState::cylinder:
		BuildCylinder();
		break;
	case BuildState::sphere:
		BuildSphere();
		break;
	}
}

void WorldBuilderAi::OrderPositions() {
	Map::Pos temp1;
	Map::Pos temp2;

	if (pos1.w > pos2.w) {
		temp1.w = pos2.w;
		temp2.w = pos1.w;
	}
	else {
		temp1.w = pos1.w;
		temp2.w = pos2.w;
	}

	if (pos1.h > pos2.h) {
		temp1.h = pos2.h;
		temp2.h = pos1.h;
	}
	else {
		temp1.h = pos1.h;
		temp2.h = pos2.h;
	}

	if (pos1.d > pos2.d) {
		temp1.d = pos2.d;
		temp2.d = pos1.d;
	}
	else {
		temp1.d = pos1.d;
		temp2.d = pos2.d;
	}
	pos1 = temp1;
	pos2 = temp2;
}

void WorldBuilderAi::BuildBlock() {

	for (int j = pos1.h; j <= pos2.h; j++) {
		for (int i = pos1.w; i <= pos2.w; i++) {
			for (int h = pos1.d; h <= pos2.d; h++) {
				engine.map->SetTileAt(i, j, h, block);
			}
		}
	}
}

void WorldBuilderAi::BuildCylinder() {


	float radius = sqrt(((pos1.w - pos2.w) * (pos1.w - pos2.w)) + ((pos1.h - pos2.h) * (pos1.h - pos2.h)));

	std::cout << radius << "\n";



	float x = pos1.w;
	float y = pos1.h;

	for (int j = pos1.h - radius; j <= pos1.h + radius; j++) {
		for (int i = pos1.w - radius; i <= pos1.w + radius; i++) {
			for (int h = pos1.d; h <= pos2.d; h++) {
				float currentRadius = sqrt(((i - pos1.w) * (i - pos1.w)) + ((j - pos1.h) * (j - pos1.h)));

				//std::cout << currentRadius << "\n";

				if (currentRadius <= radius) {
					engine.map->SetTileAt(i, j, h, block);
				}
			}
		}
	}

}

void WorldBuilderAi::BuildSphere() {

	float radius = sqrt(((pos1.w - pos2.w) * (pos1.w - pos2.w)) + ((pos1.h - pos2.h) * (pos1.h - pos2.h)));

	float fullRadius = sqrt((radius*radius) + ((pos1.d - pos2.d) * (pos1.d - pos2.d)));

	std::cout << radius << "\n";



	float x = pos1.w;
	float y = pos1.h;

	for (int j = pos1.h - radius; j <= pos1.h + radius; j++) {
		for (int i = pos1.w - radius; i <= pos1.w + radius; i++) {
			for (int h = pos1.d - radius; h <= pos1.d + radius; h++) {
				float partialRadius = sqrt(((i - pos1.w) * (i - pos1.w)) + ((j - pos1.h) * (j - pos1.h)));
				float currentRadius = sqrt((partialRadius * partialRadius) + ((h - pos1.d) * (h - pos1.d)));
				//std::cout << currentRadius << "\n";

				if (currentRadius <= radius) {
					engine.map->SetTileAt(i, j, h, block);
				}
			}
		}
	}

}

void CastAi::OnTick(std::shared_ptr<Entity> entity) {
	bool hasHit = false;
	switch (dir) {
	case up:
		hasHit = MoveRelative(entity, 0, -1, 0);
		break;
	case down:
		hasHit = MoveRelative(entity, 0, 1, 0);
		break;
	case left:
		hasHit = MoveRelative(entity, -1, 0, 0);
		break;
	case right:
		hasHit = MoveRelative(entity, 1, 0, 0);
		break;
	}

	if (!hasHit) {
		Map::Pos p = entity->pos;

		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		p = entity->pos;
		p.d--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		p = entity->pos;
		p.d++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.h++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.w--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		engine.npcs.erase(std::remove(engine.npcs.begin(), engine.npcs.end(), entity), engine.npcs.end());
	}


}
