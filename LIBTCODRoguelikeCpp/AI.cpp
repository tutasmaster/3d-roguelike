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
	return Move(entity, x + entity->pos.x, y + entity->pos.y, z + entity->pos.z);
}

void Ai::Follow(std::shared_ptr<Entity> entity, std::shared_ptr<Entity> follower, int step = 1) {
	Map::Pos movement;

	if (entity->pos.x < follower->pos.x) {
		movement.x = step;
	}
	else if (entity->pos.x > follower->pos.x) {
		movement.x = -step;
	}

	if (entity->pos.y < follower->pos.y) {
		movement.y = step;
	}
	else if (entity->pos.y > follower->pos.y) {
		movement.y = -step;
	}

	if (entity->pos.z < follower->pos.z) {
		movement.z = step;
	}
	else if (entity->pos.z > follower->pos.z) {
		movement.z = -step;
	}

	MoveRelative(entity, movement.x, movement.y, movement.z);
}

bool Ai::OnMoveSideways(std::shared_ptr<Entity> entity, int x, int y)
{
	Map::Pos p(entity->pos.x + x, entity->pos.y + y, entity->pos.z);

	if (entity->isColliding) {
		Map::Pos top(entity->pos.x + x, entity->pos.y + y, entity->pos.z + 1);
		Map::Pos bot(entity->pos.x + x, entity->pos.y + y, entity->pos.z - 1);
		Map::Pos botB(entity->pos.x + x, entity->pos.y + y, entity->pos.z - 2);
		Map::Pos topP(entity->pos.x, entity->pos.y, entity->pos.z + 1);

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
		else if (!cB && !bB && !tB) {
			return false;
		}
		else if (tB && !cB && !tpB) {
			entity->pos = top;
		}
		else {
			std::cout << "Found unhandled type of block structure: cB:" << cB << ", tB:" << tB << ", bB:" << bB << ", bbB:" << bbB << ", tpB:" << tpB << "\n";
		}
		return true;

	}
	else {
		entity->pos = p;
		return false;
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
		int x = entity->pos.x - ((rand() % 10) - 5);
		int y = entity->pos.y - ((rand() % 10) - 5);

		nextPosX = x;
		nextPosY = y;

		isFollowing = true;


	}
	else {

		int relativeX = nextPosX - entity->pos.x;
		int relativeY = nextPosY - entity->pos.y;

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

		if (entity->pos.x == nextPosX && entity->pos.y == nextPosY) {
			isFollowing = false;
		}
	}
}

void EnemyAi::OnTick(std::shared_ptr<Entity> entity) {
	
	if(hasFoundPlayer){
		nextPosX = engine.player->pos.x;
		nextPosY = engine.player->pos.y;

		int relativeX = nextPosX - entity->pos.x;
		int relativeY = nextPosY - entity->pos.y;

		if (relativeX > 0)
			relativeX = 1;
		else if (relativeX < 0)
			relativeX = -1;

		if (relativeY > 0)
			relativeY = 1;
		else if (relativeY < 0)
			relativeY = -1;

		if (!OnMoveSideways(entity, relativeX, relativeY)) {
			hasFoundPlayer = false;
		}

		if (GetDistance(entity->pos, engine.player->pos) < 1.5) {
			engine.player->meta->DoDamage(20);
		}
	}
	else {
		if (!isFollowing) {
			int x = entity->pos.x - ((rand() % 10) - 5);
			int y = entity->pos.y - ((rand() % 10) - 5);

			nextPosX = x;
			nextPosY = y;

			isFollowing = true;


		}
		else {

			int relativeX = nextPosX - entity->pos.x;
			int relativeY = nextPosY - entity->pos.y;

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

			if (entity->pos.x == nextPosX && entity->pos.y == nextPosY) {
				isFollowing = false;
			}
		}

		if (GetDistance(entity->pos, engine.player->pos) < 7) {
			hasFoundPlayer = true;
		}
	}
	
}

void PlayerAi::OnTick(std::shared_ptr<Entity> entity)
{
	auto lastKey = engine.keyboardInput.pressed ? engine.keyboardInput : TCOD_key_t();

	if (engine.mouseInput.lbutton && engine.mouseInput.cx > 26 && engine.mouseInput.cx < 38 && engine.mouseInput.cy > 26 && engine.mouseInput.cy < 38) {
		engine.map->SetTileAt(Map::Pos(engine.player->pos.x + engine.mouseInput.cx - 32, engine.player->pos.y + engine.mouseInput.cy - 32, engine.player->pos.z),
			tileManager.tile_empty);

		engine.map->SetGroundAt(Map::Pos(engine.player->pos.x + engine.mouseInput.cx - 32, engine.player->pos.y + engine.mouseInput.cy - 32, engine.player->pos.z + 1),
			tileManager.ground_empty);
	}

	if (isDigging)
		engine.map->SetTileAt(entity->pos.x, entity->pos.y, entity->pos.z, TileManager::tile_empty);
	else if (isBuilding)
		engine.map->SetTileAt(entity->pos.x, entity->pos.y, entity->pos.z, TileManager::tile_wall);
	else if (isBuildingNB)
		engine.map->SetTileAt(entity->pos.x, entity->pos.y, entity->pos.z, TileManager::tile_wallNB);


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
		std::cout << 'g\n';
		entity->inv->PickupItem(entity->pos);
		break;
	case 'c':
		isAttacking = true;
		break;
		/*case 'c':
			engine.GUI_ID = 3;
			break;*/
	case 'f':
		engine.console.push_back({ "You start flossing for no reason... You feel shame.", TCODColor::red });
		break;
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
	case TCODK_KP4:
		engine.FPSAngle += 0.25f;
		break;
	case TCODK_KP6:
		engine.FPSAngle -= 0.25f;
		break;
	case TCODK_KP8:
		engine.FPSAngleY -= 0.25f;
		break;
	case TCODK_KP2:
		engine.FPSAngleY += 0.25f;
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
	case TCODK_F4:
		engine.betterRenderer = !engine.betterRenderer;
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

	if (pos1.x > pos2.x) {
		temp1.x = pos2.x;
		temp2.x = pos1.x;
	}
	else {
		temp1.x = pos1.x;
		temp2.x = pos2.x;
	}

	if (pos1.y > pos2.y) {
		temp1.y = pos2.y;
		temp2.y = pos1.y;
	}
	else {
		temp1.y = pos1.y;
		temp2.y = pos2.y;
	}

	if (pos1.z > pos2.z) {
		temp1.z = pos2.z;
		temp2.z = pos1.z;
	}
	else {
		temp1.z = pos1.z;
		temp2.z = pos2.z;
	}
	pos1 = temp1;
	pos2 = temp2;
}

void WorldBuilderAi::BuildBlock() {

	for (int j = pos1.y; j <= pos2.y; j++) {
		for (int i = pos1.x; i <= pos2.x; i++) {
			for (int y = pos1.z; y <= pos2.z; y++) {
				engine.map->SetTileAt(i, j, y, block);
			}
		}
	}
}

void WorldBuilderAi::BuildCylinder() {


	float radius = sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x)) + ((pos1.y - pos2.y) * (pos1.y - pos2.y)));

	std::cout << radius << "\n";



	float x = pos1.x;
	float y = pos1.y;

	for (int j = pos1.y - radius; j <= pos1.y + radius; j++) {
		for (int i = pos1.x - radius; i <= pos1.x + radius; i++) {
			for (int y = pos1.z; y <= pos2.z; y++) {
				float currentRadius = sqrt(((i - pos1.x) * (i - pos1.x)) + ((j - pos1.y) * (j - pos1.y)));

				//std::cout << currentRadius << "\n";

				if (currentRadius <= radius) {
					engine.map->SetTileAt(i, j, y, block);
				}
			}
		}
	}

}

void WorldBuilderAi::BuildSphere() {

	float radius = sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x)) + ((pos1.y - pos2.y) * (pos1.y - pos2.y)));

	float fullRadius = sqrt((radius*radius) + ((pos1.z - pos2.z) * (pos1.z - pos2.z)));

	std::cout << radius << "\n";



	float x = pos1.x;
	float y = pos1.y;

	for (int j = pos1.y - radius; j <= pos1.y + radius; j++) {
		for (int i = pos1.x - radius; i <= pos1.x + radius; i++) {
			for (int y = pos1.z - radius; y <= pos1.z + radius; y++) {
				float partialRadius = sqrt(((i - pos1.x) * (i - pos1.x)) + ((j - pos1.y) * (j - pos1.y)));
				float currentRadius = sqrt((partialRadius * partialRadius) + ((y - pos1.z) * (y - pos1.z)));
				//std::cout << currentRadius << "\n";

				if (currentRadius <= radius) {
					engine.map->SetTileAt(i, j, y, block);
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
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		p = entity->pos;
		p.z--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		p = entity->pos;
		p.z++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.y++;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);
		p.x--;
		engine.map->SetAt(p, tileManager.tile_empty, tileManager.ground_empty);

		engine.npcs.erase(std::remove(engine.npcs.begin(), engine.npcs.end(), entity), engine.npcs.end());
	}


}
