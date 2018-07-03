#include "engine.hpp"

Engine::Engine() : map(64,64,64)
{
	map.GenerateTerrain(0);

	TCODConsole::initRoot(64, 80, "Roguelike");

	player->ai = std::make_shared<PlayerAi>();

	for (int i = 0; i < map.depth; i++) {
		if (map.GetTileAt(0, 0, i)->type != TileManager::wall) {
			player->pos.d = i;
			break;
		}
	}


	/*std::shared_ptr<Entity> npc = std::make_shared<Entity>();
	npc->pos.w = 10;
	npc->pos.h = 10;
	npc->ai = std::make_shared<FriendlyAi>();
	npcs.push_back(npc);*/

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
	TCODConsole::root->clear();

	if (betterRenderer) {
		renderMap();
	}else{
		renderMapStandard();
	}

	for (auto & e : npcs) {
		TCODColor c = e->col;
		TCODConsole::root->setCharForeground(e->pos.w, e->pos.h, e->col);
		TCODConsole::root->setChar(e->pos.w, e->pos.h, e->c);
	}

	TCODConsole::root->setCharBackground(player->pos.w, player->pos.h,map.GetTileAt(player->pos.w, player->pos.h, player->pos.d)->bg);
	TCODConsole::root->setCharForeground(player->pos.w, player->pos.h,TCODColor::gold);
	TCODConsole::root->setChar(player->pos.w, player->pos.h, '@');

	int j = 0;
	for (int i = console.size() - 1; i > -1; i--) {
		j++;
		TCODConsole::root->setDefaultForeground(console.at(i).col);
		TCODConsole::root->print(0, 65 + j, console.at(i).msg.c_str());
	}

}

void Engine::update()
{
	player->ai->OnTick(player);
	if(player->ai->hasUpdated){
		for (auto &e : npcs) {
			if(e->ai != nullptr){
				e->ai->OnTick(e);
			}
		}
	}

	/*Tile::Type value = map.GetTileAt(x,y,yPosition)->type;

	if (value == Tile::Type::wall) {
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

void Engine::renderMap() {
	for (int j = 0; j < map.height; j++) {
		for (int i = 0; i < map.width; i++) {
			Tile* r = map.GetTileAt(i, j, player->pos.d);

			if (r != nullptr && r->type != r->empty) {
				TCODConsole::root->setCharBackground(i, j, r->bg);
				TCODConsole::root->setCharForeground(i, j, r->color);
				TCODConsole::root->setChar(i, j, r->c);
			}
			else if (r != nullptr) {
				int g = 0;
				for (int z = player->pos.d; z > -1; --z) {
					Tile * t = map.GetTileAt(i - g, j - g, z);
					int temp = g;
					bool isHidden = false;
					if (g != 0 && map.GetTileAt((i - g) + 1, (j - g) + 1, z) != nullptr && map.GetTileAt((i - g) + 1, (j - g) + 1, z)->type == Tile::wall && map.GetTileAt((i - g) + 1, (j - g) + 1, z)->isBlocking) {
						t = map.GetTileAt((i - g) + 1, (j - g) + 1, z);
						temp--;
						isHidden = true;
					}
					if (t != nullptr && t->type != Tile::empty) {
						if (t->shadeLimit < g)
							break;

						TCODColor col = t->color;
						float a = col.getValue();
						col.setValue((a / (temp + 1 + (3 - ((t->shadeLimit) / 10)))) - (isHidden * 10));
						col.setHue(col.getHue() - (g*4.5f));


						TCODColor bg = t->bg;
						float b = bg.getValue();
						bg.setValue((b / (temp + 1 + (3 - ((t->shadeLimit) / 10)))) - (isHidden * 10));
						bg.setHue(bg.getHue() - (g*4.5f));



						TCODConsole::root->setCharForeground(i, j, col);
						TCODConsole::root->setCharBackground(i, j, bg);
						TCODConsole::root->setChar(i, j, t->c);
						break;
					}

					g++;

					if (g > layerSize)
						break;
				}
			}
			else {
				//Err::ConsoleLog("Something awfull happened while drawing the map... run!");
			}
		}
	}
}

void Engine::renderMapStandard() {
	for (int j = 0; j < map.height; j++) {
		for (int i = 0; i < map.width; i++) {
			auto layer = map.GetTileAt(i, j, player->pos.d);
			if(layer != nullptr){
				TCODColor col = layer->color;
				TCODColor bg = layer->bg;
				char c = layer->c;
				int temp = 2;
				if(layer->type == TileManager::empty){
					for (int h = player->pos.d - 1; h > -1; h--) {
						layer = map.GetTileAt(i, j, h);
						if (layer != nullptr) {
							if (layer->type != TileManager::empty) {
								col = layer->color;
								bg = layer->bg;
								c = layer->c;
								col.setValue(col.getValue() / temp);
								bg.setValue(bg.getValue() / temp);
								break;
							}
						}
						temp++;
						if (temp > layerSize)
							break;
					}
				}

				TCODConsole::root->setCharForeground(i, j, col);
				TCODConsole::root->setCharBackground(i, j, bg);
				TCODConsole::root->setChar(i, j, layer->c);
			}
		}
	}
}