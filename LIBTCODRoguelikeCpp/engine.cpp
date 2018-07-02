#include "engine.hpp"

Engine::Engine() : map(64,64,64)
{

	TCODConsole::initRoot(128, 80, "Roguelike");

	player->ai = std::make_shared<PlayerAi>();

	SetMapLayer(map, 0, TileManager::floor);
	
	DrawSquareOnMap(map, 5, 5, 10, 10, 1, TileManager::wall);
	DrawSquareOnMap(map, 5, 5, 10, 10, 2, TileManager::wall);
	DrawSquareOnMap(map, 5, 5, 10, 10, 3, TileManager::wall);
	DrawSquareOnMap(map, 5, 5, 10, 10, 4, TileManager::wall);
	DrawSquareOnMap(map, 5, 5, 10, 10, 5, TileManager::wall);

	DrawSquareOnMap(map, 6, 6, 8, 8, 1, TileManager::floor);
	DrawSquareOnMap(map, 6, 6, 8, 8, 2, TileManager::floor);
	DrawSquareOnMap(map, 6, 6, 8, 8, 3, TileManager::floor);
	DrawSquareOnMap(map, 6, 6, 8, 8, 4, TileManager::floor);
	DrawSquareOnMap(map, 6, 6, 8, 8, 5, TileManager::floor);

	DrawSquareOnMap(map, 20, 5, 10, 10, 1, TileManager::wall);
	DrawSquareOnMap(map, 20, 5, 10, 10, 2, TileManager::wall);
	DrawSquareOnMap(map, 20, 5, 10, 10, 3, TileManager::wall);
	DrawSquareOnMap(map, 20, 5, 10, 10, 4, TileManager::wall);
	DrawSquareOnMap(map, 20, 5, 10, 10, 5, TileManager::wall);

	DrawSquareOnMap(map, 21, 6, 8, 8, 1, TileManager::floor);
	DrawSquareOnMap(map, 21, 6, 8, 8, 2, TileManager::floor);
	DrawSquareOnMap(map, 21, 6, 8, 8, 3, TileManager::floor);
	DrawSquareOnMap(map, 21, 6, 8, 8, 4, TileManager::floor);
	DrawSquareOnMap(map, 21, 6, 8, 8, 5, TileManager::floor);

	DrawSquareOnMap(map, 14, 8, 7, 4, 3, TileManager::wall);
	DrawSquareOnMap(map, 14, 8, 2, 4, 4, TileManager::wall);
	DrawSquareOnMap(map, 14, 9, 7, 2, 4, TileManager::floor);

	DrawSquareOnMap(map, 21, 6, 8, 8, 6, TileManager::wall);
	DrawSquareOnMap(map, 22, 7, 6, 6, 7, TileManager::wall);
	DrawSquareOnMap(map, 23, 8, 4, 4, 8, TileManager::wall);
	DrawSquareOnMap(map, 24, 9, 2, 2, 9, TileManager::wall);

	DrawSquareOnMap(map, 22, 7, 6, 6, 6, TileManager::floor);
	DrawSquareOnMap(map, 23, 8, 4, 4, 7, TileManager::floor);
	DrawSquareOnMap(map, 24, 9, 2, 2, 8, TileManager::floor);

	map.SetTileAt(14, 9, 1, TileManager::floor);
	map.SetTileAt(14, 10, 1, TileManager::floor);

	map.SetTileAt(14, 9, 2, TileManager::empty);
	map.SetTileAt(14, 10, 2, TileManager::empty);

	map.SetTileAt(20, 9, 1, TileManager::floor);
	map.SetTileAt(20, 10, 1, TileManager::floor);

	map.SetTileAt(20, 9, 2, TileManager::empty);
	map.SetTileAt(20, 10, 2, TileManager::empty);
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


	for (int j = 0; j < map.height; j++) {
		for (int i = 0; i < map.width; i++) {
			Tile* r = map.GetTileAt(i,j,player->pos.d);

			if(r != nullptr && r->type != r->empty){
				TCODConsole::root->setCharBackground(i, j, r->bg);
				TCODConsole::root->setCharForeground(i,j,r->color);
				TCODConsole::root->setChar(i, j, r->c);
			}
			else if(r != nullptr){
				int g = 0;
				for (int z = player->pos.d; z > -1; --z) {
					Tile * t = map.GetTileAt(i - g, j - g, z);
					int temp = g;
					bool isHidden = false;
					if (g != 0 && map.GetTileAt((i - g) + 1, (j - g) + 1, z) != nullptr && map.GetTileAt((i - g) + 1, (j - g) + 1, z)->type == Tile::wall && map.GetTileAt((i - g) + 1, (j - g) + 1, z)->isBlocking){
						t = map.GetTileAt((i - g) + 1, (j - g) + 1, z);
						temp--;
						isHidden = true;
					}
					if (t != nullptr && t->type != Tile::empty) {
						if (t->shadeLimit < g)
							break;

						TCODColor col = t->color;
						float a = col.getValue();
						col.setValue((a/(temp+1 + (3 - ((t->shadeLimit) / 2)))) - (isHidden * 20));


						TCODColor bg = t->bg;
						float b = bg.getValue();
						bg .setValue((b/(temp+1 + (3 - ((t->shadeLimit) / 2)))) - (isHidden*20));

						

						TCODConsole::root->setCharForeground(i, j, col);
						TCODConsole::root->setCharBackground(i, j, bg);
						TCODConsole::root->setChar(i, j, t->c);
						break;
					}

					g++;

					if (g > 5)
						break;
				}
			}
			else {
				//Err::ConsoleLog("Something awfull happened while drawing the map... run!");
			}
		}
	}

	TCODConsole::root->setCharBackground(player->pos.w, player->pos.h,map.GetTileAt(player->pos.w, player->pos.h, player->pos.d)->bg);
	TCODConsole::root->setCharForeground(player->pos.w, player->pos.h,TCODColor::gold);
	TCODConsole::root->setChar(player->pos.w, player->pos.h, '@');
}

void Engine::update()
{
	player->ai->OnTick(player);

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