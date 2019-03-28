#include "engine.hpp"
#include "Parser.h"

Engine::Engine() 
{
	Parser().Run();

	for (auto &npc : npcs) {
		npc->meta = std::make_shared<Metabolism>(npc);
	}

	UI_list.push_back(std::make_unique<InventoryGUI>());
	UI_list.push_back(std::make_unique<DropGUI>());
	UI_list.push_back(std::make_unique<AnnouncementsGUI>());
	UI_list.push_back(std::make_unique<CastingGUI>());

	map = new Map(256, 256, 64);
	map->GenerateTerrain(0);

	TCODConsole::initRoot(64, 80, "Roguelike");

	player->c = '@';
	/*player->ai = std::make_shared<WorldBuilderAi>(player);
	player->ren = std::make_shared<WorldBuilderRenderer>();*/
	player->ai = std::make_shared<PlayerAi>();
	player->inv = std::make_shared<PlayerInventory>();
	player->meta = std::make_shared<Metabolism>(player);

	for (int i = 0; i < itemManager.items.size(); i++) {
		player->inv->item_vector.push_back(std::make_pair(i, 5));
	}

	for (int i = 0; i < map->depth; i++) {
		if (map->GetTileAt(128, 128, i)->type != TileManager::tile_wall) {
			player->pos.z = i;
			player->pos.x = 128;
			player->pos.y = 128;
			break;
		}
	}

	for (auto &e : npcs) {
		e->pos = player->pos;
		e->pos.x += 10;
		e->pos.y += 10;

		for (int i = 0; i < map->depth; i++) {
			if (map->GetTileAt(e->pos.x, e->pos.y, i)->type != TileManager::tile_wall) {
				e->pos.z = i;
				break;
			}
		}
	}


	Message msg;
	msg.msg = "Player has entered the world!";
	msg.col = TCODColor::yellow;

	console.push_back(msg);
}




Engine::~Engine()
{

}

void Engine::term()
{

}

void Engine::init()
{

}

void Engine::render()
{
	switch (curScene) {
	case playing:
		playingRender();
		break;
	case gameover:
		gameoverRender();
		break;
	}
}

void Engine::playingRender() {

	int mapOffsetX = 0, mapOffsetY = 0;
	if (GUI_ID != -1) {
		mapOffsetX = UI_list.at(GUI_ID)->mapOffsetX;
		mapOffsetY = UI_list.at(GUI_ID)->mapOffsetY;
	}

	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->clear();

	/*if (betterRenderer) {
		renderMap(mapOffsetX, mapOffsetY,-1,-1);
	}else{
		renderMap(mapOffsetX, mapOffsetY,1,1);
	}*/

	//engineRenderer.renderMapOLD(mapOffsetX, mapOffsetY);
	//engineRenderer.renderMap(mapOffsetX, mapOffsetY, angle, 62, 62);
	engineRenderer.renderMapTesting(mapOffsetX, mapOffsetY, angle, 62, 62);


	engineRenderer.renderCompass(angle);

	TCODConsole::root->setCharBackground(32 - mapOffsetX, 32 - mapOffsetY, map->GetTileAt(player->pos.x, player->pos.y, player->pos.z)->bg);
	TCODConsole::root->setCharForeground(32 - mapOffsetX, 32 - mapOffsetY, TCODColor::gold);
	TCODConsole::root->setChar(32 - mapOffsetX, 32 - mapOffsetY, player->c);

	int j = 0;
	for (int i = console.size() - 1; i > -1; i--) {
		j++;
		TCODConsole::root->setDefaultForeground(console.at(i).col);
		TCODConsole::root->print(1, 64 + j, console.at(i).msg.c_str());
		if (j > 4)
			i = -1;
	}

	TCODConsole::root->setDefaultForeground(TCODColor::yellow);
	TCODConsole::root->printFrame(0, 0, 64, 64, false);

	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->printFrame(0, 64, 64, 7, false);

	TCODConsole::root->printFrame(0, 71, 64, 9, false);

	TCODConsole::root->print(1, 71, player->name.c_str());
	TCODConsole::root->print(1, 74, "Head\nTrunk\nArms\nLegs\nFeet");

	int i = 0;
	for (auto &item : player->inv->item_vector) {

		char str[6];
		sprintf_s(str, "%03d -", item.second);

		/*TCODConsole::root->setDefaultBackground(TCODColor::black);
		TCODConsole::root->setDefaultForeground(TCODColor::white);
		TCODConsole::root->print(8 + ((i / 5) * 7), 74 + (i % 5), str);*/
		/*TCODConsole::root->setDefaultBackground(itemManager.GetItemData(item.first)->bgCol);
		TCODConsole::root->setDefaultForeground(itemManager.GetItemData(item.first)->chCol);
		TCODConsole::root->print(14 + ((i / 5) * 4), 74 + (i % 5), itemManager.GetItemData(item.first)->shrtnm.c_str());*/

		for (int j = 0; j < item.second; j++) {
			TCODConsole::root->setCharBackground(8 + ((i / 4)), 74 + (i % 4), itemManager.GetItemData(item.first)->bgCol);
			TCODConsole::root->setCharForeground(8 + ((i / 4)), 74 + (i % 4), itemManager.GetItemData(item.first)->chCol);
			TCODConsole::root->setChar(8 + ((i / 4)), 74 + (i % 4), itemManager.GetItemData(item.first)->ch);
			i++;
		}

		//i++;
	}

	if (player->ren != nullptr)
		player->ren->OnRender(player);

	for (auto & e : npcs) {
		if (e->ren != nullptr)
			e->ren->OnRender(e);
	}

	if (GUI_ID != -1) {
		TCODConsole::blit(&UI_list.at(GUI_ID)->console, 0, 0, UI_list.at(GUI_ID)->width, UI_list.at(GUI_ID)->height, TCODConsole::root, UI_list.at(GUI_ID)->x, UI_list.at(GUI_ID)->y, 0.9f, 0.9f);
	}


}

void Engine::gameoverRender() {
	TCODConsole::root->print(0, 0, "YOU HAVE FAILED!!!");
}

void Engine::update()
{
	switch (curScene) {
	case playing:
		playingUpdate();
		break;
	case gameover:
		gameoverUpdate();
		break;
	}
}

void Engine::playingUpdate() {
	TCODSystem::checkForEvent(TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &keyboardInput, &mouseInput);

	if (GUI_ID == -1) { //MAIN UPDATE
		player->ai->OnTick(player);
		if (player->ai->hasUpdated) {
			for (auto &e : npcs) {
				if (e->ai != nullptr) {
					e->ai->OnTick(e);
				}
				if (e->phys != nullptr) {
					e->phys->ApplyPhysics(e);
				}
			}
		}
	}
	else {
		UI_list.at(GUI_ID)->Update();
	}

	int i = 0;
	for (auto &effect : effects) {
		effect->Update(i);
		i++;
	}

	/*i = 0;
	for (auto &effect : effects) {
		if(effect->erase){
			effects.erase(effects.begin() + i);
			i--;
		}
		i++;
	}*/

	/*Tile::Type value = map.GetTileAt(x,y,yPosition)->type;

	if (value == Tile::Type::tile_wall) {
		yPosition++;
	}
	else if (value == 0) {
		Tile::Type bottomValue = map.GetTileAt(x, y, yPosition-1)->type;

		while (bottomValue == 0) {
			yPosition--;
			bottomValue = map.GetTileAt(x,y,yPosition)->type;
		}
	}*/

	if (player->meta->HP <= 0) {
		curScene = gameover;
	}
}

void Engine::gameoverUpdate() {
	TCODSystem::checkForEvent(TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &keyboardInput, &mouseInput);
}

std::shared_ptr<Entity> Engine::checkEntityAtPos(Map::Pos p) {
	for (auto &e : npcs) {
		if (e->pos == p) {
			return e;
		}
	}

	if (player->pos == p)
		return player;

	return nullptr;
}

bool Engine::checkEntityCollisionAtPos(Map::Pos p) {
	std::shared_ptr<Entity> s = checkEntityAtPos(p);
	if (s != nullptr) {
		return s->isColliding;
	}
	return false;
}

void EngineRenderer::renderMapStandard() {
	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < 64; i++) {
			auto layer = engine.map->GetTileAt(engine.player->pos.x + i - 32, engine.player->pos.y + j - 32, engine.player->pos.z);
			if (layer != nullptr) {
				TCODColor col = layer->color;
				TCODColor bg = layer->bg;
				char c = layer->c;
				int temp = 2;
				if (layer->type == TileManager::tile_empty) {
					for (int y = engine.player->pos.z - 1; y > -1; y--) {
						layer = engine.map->GetTileAt(engine.player->pos.x + i - 32, engine.player->pos.y + j - 32, y);
						if (layer != nullptr) {
							if (layer->type != TileManager::tile_empty) {
								col = layer->color;
								bg = layer->bg;
								c = layer->c;
								col.setValue(sqrt(col.getValue() / temp));
								bg.setValue(sqrt(bg.getValue() / temp));
								bg.setHue(bg.getHue() - (temp * 0.05));
								break;
							}
						}
						temp++;
					}
				}

				TCODConsole::root->setCharForeground(i, j, col);
				TCODConsole::root->setCharBackground(i, j, bg);
				TCODConsole::root->setChar(i, j, layer->c);
			}
		}
	}

	for (auto & e : engine.npcs) {
		if (e->pos.z >= engine.player->pos.z) {
			TCODColor c = e->col;
			TCODConsole::root->setCharForeground(e->pos.x, e->pos.y, e->col);
			TCODConsole::root->setChar(e->pos.x, e->pos.y, e->c);
		}
	}
}

void Engine::saveMap(const char * name) {
	TCODZip zip;
	zip.putInt(map->width);
	zip.putInt(map->height);
	zip.putInt(map->depth);
	for (int j = 0; j < map->height; j++) {
		for (int i = 0; i < map->width; i++) {
			for (int y = 0; y < map->depth; y++) {
				zip.putInt(map->arr[(i)+((j)*map->width) + ((y)*map->height * map->width)]);
				zip.putInt(map->groundArr[(i)+((j)*map->width) + ((y)*map->height * map->width)]);
				
			}
		}
	}
	zip.saveToFile(name);
}

void Engine::loadMap(const char * name) {
	TCODZip zip;
	if(TCODSystem::fileExists(name)){
		zip.loadFromFile(name);
		delete map;
		map = new Map(zip.getInt(), zip.getInt(), zip.getInt());
		//map = Map(zip.getInt(), zip.getInt(), zip.getInt());
		/*zip.getInt(); zip.getInt(); zip.getInt();*/
		
		for (int j = 0; j < map->height; j++) {
			for (int i = 0; i < map->width; i++) {
				for (int y = 0; y < map->depth; y++) {
					map->arr[(i)+((j)*map->width) + ((y)*map->height * map->width)] = zip.getInt();
					map->groundArr[(i)+((j)*map->width) + ((y)*map->height * map->width)] = zip.getInt();
				}
			}
		}
	}
}