#include "engine.hpp"


Engine::Engine() 
{
	UI_list.push_back(std::make_unique<InventoryGUI>());
	UI_list.push_back(std::make_unique<DropGUI>());
	UI_list.push_back(std::make_unique<AnnouncementsGUI>());

	map = new Map(1024, 1024, 64);
	map->GenerateTerrain(0);

	TCODConsole::initRoot(64, 80, "Roguelike");

	player->c = '@';
	player->ai = std::make_shared<PlayerAi>();
	player->inv = std::make_shared<Inventory>();
	player->inv->item_vector.push_back(std::make_pair(itemManager.item_potion, 10));
	player->inv->item_vector.push_back(std::make_pair(itemManager.item_gold, 10));

	for (int i = 0; i < map->depth; i++) {
		if (map->GetTileAt(128, 128, i)->type != TileManager::tile_wall) {
			player->pos.d = i;
			player->pos.w = 128;
			player->pos.h = 128;
			break;
		}
	}


	std::shared_ptr<Entity> b = std::make_shared<Entity>();
	b->pos = player->pos;
	b->isColliding = false;
	npcs.push_back(b);

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
	int mapOffsetX = 0, mapOffsetY = 0;
	if (GUI_ID != -1) {
		mapOffsetX = UI_list.at(GUI_ID)->mapOffsetX;
		mapOffsetY = UI_list.at(GUI_ID)->mapOffsetY;
	}

	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->clear();

	if (betterRenderer) {
		renderMapStandard();
	}else{
		renderMap(mapOffsetX, mapOffsetY);
	}

	TCODConsole::root->setCharBackground(32 - mapOffsetX, 32 - mapOffsetY,map->GetTileAt(player->pos.w, player->pos.h, player->pos.d)->bg);
	TCODConsole::root->setCharForeground(32 - mapOffsetX, 32 - mapOffsetY,TCODColor::gold);
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

void Engine::update()
{
	TCODSystem::checkForEvent(TCOD_EVENT_KEY | TCOD_EVENT_MOUSE, &keyboardInput, &mouseInput);

	if (GUI_ID == -1) { //MAIN UPDATE
		player->ai->OnTick(player);
		if(player->ai->hasUpdated){
			for (auto &e : npcs) {
				if(e->ai != nullptr){
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
}

std::shared_ptr<Entity> Engine::checkEntityAtPos(Map::Pos p) {
	for (auto &e : npcs) {
		if (e->pos == p){
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

void Engine::renderMapStandard() {
	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < 64; i++) {
			auto layer = map->GetTileAt(player->pos.w + i - 32, player->pos.h + j - 32, player->pos.d);
			if(layer != nullptr){
				TCODColor col = layer->color;
				TCODColor bg = layer->bg;
				char c = layer->c;
				int temp = 2;
				if(layer->type == TileManager::tile_empty){
					for (int h = player->pos.d - 1; h > -1; h--) {
						layer = map->GetTileAt(player->pos.w + i - 32, player->pos.h + j - 32, h);
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

	for (auto & e : npcs) {
		if (e->pos.d >= player->pos.d) {
			TCODColor c = e->col;
			TCODConsole::root->setCharForeground(e->pos.w, e->pos.h, e->col);
			TCODConsole::root->setChar(e->pos.w, e->pos.h, e->c);
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
			for (int h = 0; h < map->depth; h++) {
				zip.putInt(map->arr[(i)+((j)*map->width) + ((h)*map->height * map->width)]);
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
		
		for (int j = 0; j < map->height; j++) {
			for (int i = 0; i < map->width; i++) {
				for (int h = 0; h < map->depth; h++) {
					map->arr[(i)+((j)*map->width) + ((h)*map->height * map->width)] = zip.getInt();
				}
			}
		}
	}
}