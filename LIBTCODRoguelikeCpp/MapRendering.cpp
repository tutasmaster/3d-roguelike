#include "engine.hpp"

std::vector<int> Engine::GetAngles(char angle) {
	std::vector<int> angles = std::vector<int>();
	int xAngleOffset, yAngleOffset;
	switch (angle) {
	case 0: //NORTH
		xAngleOffset = -1;
		yAngleOffset = -1;
		break;
	case 1: //EAST
		xAngleOffset = 1;
		yAngleOffset = -1;
		break;
	case 2: //SOUTH
		xAngleOffset = 1;
		yAngleOffset = 1;
		break;
	case 3: //WEST
		xAngleOffset = -1;
		yAngleOffset = 1;
		break;
	}

	angles.push_back(xAngleOffset);
	angles.push_back(yAngleOffset);
}

void EngineRenderer::renderMap(int mOffX, int mOffY, int angle, int width, int height) {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {	
			int xAngleOffset, yAngleOffset;
			
			switch (angle) {
			case 0: //NORTH
				xAngleOffset = -1;
				yAngleOffset = -1;
				break;
			case 1: //EAST
				xAngleOffset = 1;
				yAngleOffset = -1;
				break;
			case 2: //SOUTH
				xAngleOffset = 1;
				yAngleOffset = 1;
				break;
			case 3: //WEST
				xAngleOffset = -1;
				yAngleOffset = 1;
				break;
			}

			int xAngleOffsetTrunc = xAngleOffset < 0 ? 0 : 1;
			int yAngleOffsetTrunc = yAngleOffset < 0 ? 0 : 1;

			int curI = xAngleOffset < 0 ? (width-1) - i : i;
			int curJ = yAngleOffset < 0 ? (height-1) - j : j;

			int xPosAngleOffset = xAngleOffset;
			int yPosAngleOffset = yAngleOffset;

			if (xAngleOffset >= 0 != yAngleOffset >= 0) {
				std::swap(curI, curJ);
				//std::swap(xPosAngleOffset, yPosAngleOffset);
				std::swap(mOffX, mOffY);
			}

			int xDrawPosition = curI;
			int yDrawPosition = curJ;

			/*auto val = GetAngles(angle);
			xAngleOffset = val.at(val.begin() + 0);
			yAngleOffset = val.at(val.begin() + 1);*/

			int currentXPos = i + (((engine.player->pos.w + (mOffX) - (width / 2)) - xAngleOffsetTrunc)+1);
			int currentYPos = j + (((engine.player->pos.h + (mOffY) - (height / 2)) - yAngleOffsetTrunc)+1);

			Map::Pos curPosition(currentXPos, currentYPos, engine.player->pos.d + 1);

			int depth = 0;
			for (int z = curPosition.d ; z >= 0; z--) {
				curPosition = curPosition + Map::Pos(xAngleOffset, yAngleOffset, -1);
								
				

				/*if (depth > 10)
					break;*/

				Tile * curTile = engine.map->GetTileAt(curPosition);
				Ground * curGround = engine.map->GetGroundAt(curPosition);

				char c = ' ';

				TCODColor bg = TCODColor::white;
				TCODColor color = TCODColor::white;

				bool hasGround = false;

				if (curGround != NULL) {
					if (curGround->type != TileManager::ground_empty) {
						TCODColor bg = curGround->bg;
						TCODColor color = curGround->color;

						bg.setHSV(
							bg.getHue() - (depth * 0.05),
							bg.getSaturation() - (depth * 0.02),
							bg.getValue() - (depth * 0.05));
						color.setHSV(
							color.getHue() - (depth * 0.05),
							color.getSaturation() - (depth * 0.02),
							color.getValue() - ((depth) * 0.05));


						c = curGround->c;

						drawFullCharacter(width - xDrawPosition, height - yDrawPosition, c, color, bg);

						hasGround = true;
						break;
					}
				}
				if (curTile != NULL && !hasGround){
					if (curTile->type != TileManager::tile_empty) {
						TCODColor bg = curTile->bg;
						TCODColor color = curTile->color;

						bg.setHSV(
							bg.getHue() - (depth * 0.05),
							bg.getSaturation() - (depth * 0.02),
							bg.getValue() - (depth * 0.05));
						color.setHSV(
							color.getHue() - (depth * 0.05),
							color.getSaturation() - (depth * 0.02),
							color.getValue() - ((depth) * 0.05));

						c = curTile->c;



						drawFullCharacter(width - xDrawPosition, height - yDrawPosition, c, color, bg);
						break;
					}
					else {
						depth++;
					}
				}
				else {
					break;
				}				
			}
		}																				 
	}																					 
}																						 
																						 
void EngineRenderer::renderMapOLD(int mOffX, int mOffY, int angle) {

	/*Let's draw the entire viewable area*/

	for (int j = 0; j < 62; j++) {
		for (int i = 0; i < 62; i++) {
			int rotateX, rotateY;

			/*auto val = GetAngles(angle);
			xAngleOffset = val.at(val.begin() + 0);
			yAngleOffset = val.at(val.begin() + 1);*/
			switch (angle) {
			case 0: //NORTH
				rotateX = -1;
				rotateY = -1;
				break;
			case 1: //EAST
				rotateX = 1;
				rotateY = -1;
				break;
			case 2: //SOUTH
				rotateX = 1;
				rotateY = 1;
				break;
			case 3: //WEST
				rotateX = -1;
				rotateY = 1;
				break;
			}

			int curI = rotateX < 0 ? 61 - i : i;

			if (rotateX < 0) {
				curI = 61 - i;
			}
			else {
				curI = i;
			}

			int curJ = rotateX < 0 ? 61 - j : j;

			if (rotateY < 0) {
				curJ = 61 - j;
			}
			else {
				curJ = j;
			}
			//!A != !B == A ^ B
			if (rotateX >= 0 != rotateY >= 0) {
				std::swap(curI, curJ);
			}

			int rotateXNorm = (int)((rotateX / 2) + 0.5f);
			int rotateYNorm = (int)((rotateX / 2) + 0.5f);
			//Let's keep this here so that the code get's smaller.
			//Essentially just getting the position of the engine.player, and drawing everything 32 tiles to the left of him (assuming he is on the center of a screen 64x64).
			//This will serve to make the rest of the code more readable
			Map::Pos currentPosition(engine.player->pos.w + (i)+mOffX - 31, engine.player->pos.h + (j)+mOffY - 31, engine.player->pos.d);

			TCODColor effColor = TCODColor::white;
			TCODColor effBackground = TCODColor::white;
			char effCharacter = ' ';
			bool eff = false;
			for (auto &e : engine.effects) {
				if (e->pos == currentPosition) {
					effColor = e->col;
					effBackground = e->bg;
					effCharacter = e->ch;
					eff = true;
				}
			}

			if (eff == true) {
				TCODConsole::root->setCharBackground(i, j, effBackground);
				TCODConsole::root->setCharForeground(i, j, effColor);
				TCODConsole::root->setChar(i, j, effCharacter);
				continue;
			}

			bool hasNPC = false;

			for (auto &e : engine.npcs) {
				if (e->pos.w == currentPosition.w && e->pos.h == currentPosition.h && e->pos.d == currentPosition.d) {
					hasNPC = true;
					TCODConsole::root->setCharForeground(curI + 1, curJ + 1, e->col);
					TCODConsole::root->setChar(curI + 1, curJ + 1, e->c);
					break;
				}
			}

			//Let's start raycasting down, trying to find the first possible tile we can work with
			//This keeps how far down we went.
			int depth = 0;

			//We are going to start on top and slowly go down
			if (!hasNPC) {
				for (int z = engine.player->pos.d; z > -1; --z) {
					for (auto &e : engine.npcs) {
						if (depth > 0 && e->pos.w == currentPosition.w - (depth * rotateX) + 1 && e->pos.h == currentPosition.h - (depth * rotateY) + 1 && e->pos.d == currentPosition.d - depth) {
							hasNPC = true;
							TCODColor base = e->col;
							base.setValue(sqrt((base.getValue()) / (depth + 1)));
							TCODConsole::root->setCharForeground(i + 1, j + 1, base);
							TCODConsole::root->setChar(i + 1, j + 1, e->c);
							break;
						}
					}

					if (hasNPC)
						break;

					//Let's make sure we are looking at the right tile
					Map::Pos newPosition((currentPosition.w - (depth * rotateX)) + rotateX, (currentPosition.h - (depth * rotateY)) + rotateY, currentPosition.d - depth);


					if (z >= engine.player->pos.d - 1)
						newPosition = Map::Pos(currentPosition.w, currentPosition.h, currentPosition.d - depth);


					Tile * currentTile = engine.map->GetTileAt(newPosition);
					Ground * currentGround = engine.map->GetGroundAt(newPosition);

					//Let's skip invalid tiles (they only happen outside of the map so we will skip the rest of the depth search)
					//Or if we have looked farther than we currently allow by the engine.
					if (currentTile == nullptr || currentGround == nullptr) {
						break;
					}


					bool isGround = false;
					//No point in doing anything to an air tile
					//We also check for what we have found
					if (currentTile->type == TileManager::tile_empty && currentGround->type == TileManager::ground_empty) {
						depth++;
						continue;
					}
					else if (currentTile->type != TileManager::tile_empty) {
						isGround = false;
					}
					else {
						isGround = true;
					}


					//Let's try to fix some edges
					Tile * blockingTile = engine.map->GetTileAt(newPosition.w + rotateX, newPosition.h + rotateY, newPosition.d);

					//I can only fix edges on the bottom layers
					if (!isGround && z < engine.player->pos.d - 1 && depth != 0 && blockingTile != nullptr) {

						//I give priority to any tile to the bottom and right of the current block
						if (blockingTile->type == Tile::wall) {
							currentTile = blockingTile;
						}
						else {
							//Then I pick either the one to the right, or bottom. If both are just air, then the tile is clearly visible.
							Tile * eastTile = engine.map->GetTileAt(newPosition.w + rotateX, newPosition.h, newPosition.d);
							Tile * southTile = engine.map->GetTileAt(newPosition.w, newPosition.h + rotateY, newPosition.d);
							if (eastTile != nullptr && southTile != nullptr) {
								if (eastTile->type == Tile::wall)
									currentTile = eastTile;
								else if (southTile->type == Tile::wall)
									currentTile = southTile;
							}
						}
					}

					//Getting all the colors for posterior manipulation
					TCODColor tileColor = TCODColor::white;
					TCODColor tileBackground = TCODColor::white;
					char tileCharacter = ' ';

					if (isGround) {
						tileColor = currentGround->color;
						tileBackground = currentGround->bg;
						tileCharacter = currentGround->c;
					}
					else {
						tileColor = currentTile->color;
						tileBackground = currentTile->bg;
						tileCharacter = currentTile->c;
					}

					float bgValue = tileBackground.getValue();
					float bgHue = tileBackground.getHue();
					float bgSaturation = tileBackground.getSaturation();

					float colValue = tileColor.getValue();
					float colHue = tileColor.getHue();
					float colSaturation = tileColor.getSaturation();

					//Handling water
					/*if (currentTile->type == Tile::liquid) {
						//Using custom values to add to the current tile's color
						float value = 0;
						float hue = 0;
						float saturation = 0;
						int iter = 0;

						//Continuing the ray downwards
						//Grabbing all the colors along the way and averaging (probably not the correct way to do it)
						for (int g = engine.player->pos.d - depth; g > -1; --g) {
							iter++;
							Tile * currentTile = map->GetTileAt(newPosition.w - iter, newPosition.h - iter, newPosition.d - iter);
							if (currentTile != nullptr) {
								//We get the color of anything except air
								if (currentTile->type != Tile::empty) {
									//Add the values for averaging later
									value += currentTile->bg.getValue();
									hue += currentTile->bg.getHue();
									saturation += currentTile->bg.getSaturation();

									if (currentTile->type == Tile::wall)
										break;
								}
								else {
									value += 1;
								}
							}
							else {
								iter--;
								break;
							}
						}
						bgValue = ((bgValue + value) / (iter + 1));
						tileBackground.setValue(sqrt((bgValue) / (depth + 1)));

						bgHue = ((bgHue) + hue) / (iter + 1);
						tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));

						bgSaturation = ((bgSaturation * 2) + saturation) / (iter + 2);
						tileBackground.setSaturation(bgSaturation);

						TCODConsole::root->setCharForeground(i, j, tileColor);
						TCODConsole::root->setCharBackground(i, j, tileBackground);
						TCODConsole::root->setChar(i, j, tileCharacter);

						break;
					}*/


					//Doing the color calculations apropriately
					tileBackground.setValue(sqrt((bgValue) / (depth + 1)));
					tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));
					tileColor.setValue(sqrt(colValue) / (depth + 1));
					tileColor.setHue(colHue - (depth*(colHue / (layerSize * 20))));

					//Making sure we are counting the depth looked at.
					depth++;


					//Drawing the blocks now
					TCODConsole::root->setCharForeground(curI + 1, curJ + 1, tileColor);
					TCODConsole::root->setCharBackground(curI + 1, curJ + 1, tileBackground);
					TCODConsole::root->setChar(curI + 1, curJ + 1, tileCharacter);

					//Let's stop the search if we have hit a drawable object
					if (currentTile->type != TileManager::tile_empty || currentGround->type != TileManager::ground_empty)
						break;
				}
			}
		}
	}
}

/*Probably the 1 file that's actually commented in the entire code*/

void EngineRenderer::renderMapOLD(int mOffX, int mOffY) {

	/*Let's draw the entire viewable area*/

	for (int j = 0; j < 62; j++) {
		for (int i = 0; i < 62; i++) {

			//Let's keep this here so that the code get's smaller.
			//Essentially just getting the position of the player, and drawing everything 32 tiles to the left of him (assuming he is on the center of a screen 64x64).
			//This will serve to make the rest of the code more readable
			Map::Pos currentPosition(engine.player->pos.w + i + mOffX - 31, engine.player->pos.h + j + mOffY - 31, engine.player->pos.d);

			TCODColor effColor = TCODColor::white;
			TCODColor effBackground = TCODColor::white;
			char effCharacter = ' ';
			bool eff = false;
			for (auto &e : engine.effects) {
				if (e->pos == currentPosition) {
					effColor = e->col;
					effBackground = e->bg;
					effCharacter = e->ch;
					eff = true;
				}
			}

			if (eff == true) {
				TCODConsole::root->setCharBackground(i, j, effBackground);
				TCODConsole::root->setCharForeground(i, j, effColor);
				TCODConsole::root->setChar(i, j, effCharacter);
				continue;
			}

			bool hasNPC = false;

			for (auto &e : engine.npcs) {
				if (e->pos.w == currentPosition.w && e->pos.h == currentPosition.h && e->pos.d == currentPosition.d) {
					hasNPC = true;
					TCODConsole::root->setCharForeground(i + 1, j + 1, e->col);
					TCODConsole::root->setChar(i + 1, j + 1, e->c);
					break;
				}
			}

			//Let's start raycasting down, trying to find the first possible tile we can work with
			//This keeps how far down we went.
			int depth = 0;

			//We are going to start on top and slowly go down
			if (!hasNPC) {
				for (int z = engine.player->pos.d; z > -1; --z) {
					for (auto &e : engine.npcs) {
						if (depth > 0 && e->pos.w == currentPosition.w - depth + 1 && e->pos.h == currentPosition.h - depth + 1 && e->pos.d == currentPosition.d - depth) {
							hasNPC = true;
							TCODColor base = e->col;
							base.setValue(sqrt((base.getValue()) / (depth + 1)));
							TCODConsole::root->setCharForeground(i + 1, j + 1, base);
							TCODConsole::root->setChar(i + 1, j + 1, e->c);
							break;
						}
					}

					if (hasNPC)
						break;

					//Let's make sure we are looking at the right tile
					Map::Pos newPosition(currentPosition.w - depth + 1, currentPosition.h - depth + 1, currentPosition.d - depth);

					if (z >= engine.player->pos.d - 1)
						newPosition = Map::Pos(currentPosition.w, currentPosition.h, currentPosition.d - depth);


					Tile * currentTile = engine.map->GetTileAt(newPosition);
					Ground * currentGround = engine.map->GetGroundAt(newPosition);

					//Let's skip invalid tiles (they only happen outside of the map so we will skip the rest of the depth search)
					//Or if we have looked farther than we currently allow by the engine.
					if (currentTile == nullptr || currentGround == nullptr) {
						break;
					}


					bool isGround = false;
					//No point in doing anything to an air tile
					//We also check for what we have found
					if (currentTile->type == TileManager::tile_empty && currentGround->type == TileManager::ground_empty) {
						depth++;
						continue;
					}
					else if (currentTile->type != TileManager::tile_empty) {
						isGround = false;
					}
					else {
						isGround = true;
					}


					//Let's try to fix some edges
					Tile * blockingTile = engine.map->GetTileAt(newPosition.w + 1, newPosition.h + 1, newPosition.d);

					//I can only fix edges on the bottom layers
					if (!isGround && z < engine.player->pos.d - 1 && depth != 0 && blockingTile != nullptr) {

						//I give priority to any tile to the bottom and right of the current block
						if (blockingTile->type == Tile::wall) {
							currentTile = blockingTile;
						}
						else {
							//Then I pick either the one to the right, or bottom. If both are just air, then the tile is clearly visible.
							Tile * eastTile = engine.map->GetTileAt(newPosition.w + 1, newPosition.h, newPosition.d);
							Tile * southTile = engine.map->GetTileAt(newPosition.w, newPosition.h + 1, newPosition.d);
							if (eastTile != nullptr && southTile != nullptr) {
								if (eastTile->type == Tile::wall)
									currentTile = eastTile;
								else if (southTile->type == Tile::wall)
									currentTile = southTile;
							}
						}
					}

					//Getting all the colors for posterior manipulation
					TCODColor tileColor = TCODColor::white;
					TCODColor tileBackground = TCODColor::white;
					char tileCharacter = ' ';

					if (isGround) {
						tileColor = currentGround->color;
						tileBackground = currentGround->bg;
						tileCharacter = currentGround->c;
					}
					else {
						tileColor = currentTile->color;
						tileBackground = currentTile->bg;
						tileCharacter = currentTile->c;
					}

					float bgValue = tileBackground.getValue();
					float bgHue = tileBackground.getHue();
					float bgSaturation = tileBackground.getSaturation();

					float colValue = tileColor.getValue();
					float colHue = tileColor.getHue();
					float colSaturation = tileColor.getSaturation();

					//Handling water
					/*if (currentTile->type == Tile::liquid) {
						//Using custom values to add to the current tile's color
						float value = 0;
						float hue = 0;
						float saturation = 0;
						int iter = 0;

						//Continuing the ray downwards
						//Grabbing all the colors along the way and averaging (probably not the correct way to do it)
						for (int g = player->pos.d - depth; g > -1; --g) {
							iter++;
							Tile * currentTile = map->GetTileAt(newPosition.w - iter, newPosition.h - iter, newPosition.d - iter);
							if (currentTile != nullptr) {
								//We get the color of anything except air
								if (currentTile->type != Tile::empty) {
									//Add the values for averaging later
									value += currentTile->bg.getValue();
									hue += currentTile->bg.getHue();
									saturation += currentTile->bg.getSaturation();

									if (currentTile->type == Tile::wall)
										break;
								}
								else {
									value += 1;
								}
							}
							else {
								iter--;
								break;
							}
						}
						bgValue = ((bgValue + value) / (iter + 1));
						tileBackground.setValue(sqrt((bgValue) / (depth + 1)));

						bgHue = ((bgHue) + hue) / (iter + 1);
						tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));

						bgSaturation = ((bgSaturation * 2) + saturation) / (iter + 2);
						tileBackground.setSaturation(bgSaturation);

						TCODConsole::root->setCharForeground(i, j, tileColor);
						TCODConsole::root->setCharBackground(i, j, tileBackground);
						TCODConsole::root->setChar(i, j, tileCharacter);

						break;
					}*/


					//Doing the color calculations apropriately
					tileBackground.setValue(sqrt((bgValue) / (depth + 1)));
					tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));
					tileColor.setValue(sqrt(colValue) / (depth + 1));
					tileColor.setHue(colHue - (depth*(colHue / (layerSize * 20))));

					//Making sure we are counting the depth looked at.
					depth++;

					//Drawing the blocks now
					TCODConsole::root->setCharForeground(i + 1, j + 1, tileColor);
					TCODConsole::root->setCharBackground(i + 1, j + 1, tileBackground);
					TCODConsole::root->setChar(i + 1, j + 1, tileCharacter);

					//Let's stop the search if we have hit a drawable object
					if (currentTile->type != TileManager::tile_empty || currentGround->type != TileManager::ground_empty)
						break;
				}
			}
		}
	}
}

void EngineRenderer::renderMapInverted() {

	/*Let's draw the entire viewable area*/
	/*
	for (int j = 0; j < 64; j++) {
		for (int i = 0; i < 64; i++) {

			//Let's keep this here so that the code get's smaller.
			//Essentially just getting the position of the player, and drawing everything 32 tiles to the left of him (assuming he is on the center of a screen 64x64).
			//This will serve to make the rest of the code more readable
			Map::Pos currentPosition(player->pos.w - i + 32, player->pos.h - j + 32, player->pos.d);

			//Let's start raycasting down, trying to find the first possible tile we can work with
			//This keeps how far down we went.
			int depth = 0;

			//We are going to start on top and slowly go down
			for (int z = player->pos.d; z > -1; --z) {

				//Let's make sure we are looking at the right tile
				Map::Pos newPosition(currentPosition.w + depth + 1, currentPosition.h + depth + 1, currentPosition.d - depth);

				if (z >= player->pos.d - 1) {
					newPosition = Map::Pos(currentPosition.w, currentPosition.h, currentPosition.d - depth);
				}

				Tile * currentTile = map->GetTileAt(newPosition);

				//Let's skip invalid tiles (they only happen outside of the map so we will skip the rest of the depth search)
				//Or if we have looked farther than we currently allow by the engine.
				if (currentTile == nullptr)
					break;

				//No point in doing anything to an air tile
				if (currentTile->type == TileManager::tile_empty) {
					depth++;
					continue;
				}

				//Let's try to fix some edges
				Tile * blockingTile = map->GetTileAt(newPosition.w - 1, newPosition.h - 1, newPosition.d);

				//I can only fix edges on the bottom layers
				if (z < player->pos.d - 1 && depth != 0 && blockingTile != nullptr) {

					//I give priority to any tile to the bottom and right of the current block
					if (blockingTile->type == Tile::wall) {
						currentTile = blockingTile;
					}
					else {
						//Then I pick either the one to the right, or bottom. If both are just air, then the tile is clearly visible.
						Tile * eastTile = map->GetTileAt(newPosition.w - 1, newPosition.h, newPosition.d);
						Tile * southTile = map->GetTileAt(newPosition.w, newPosition.h - 1, newPosition.d);
						if (eastTile != nullptr && southTile != nullptr) {
							if (eastTile->type == Tile::wall)
								currentTile = eastTile;
							else if (southTile->type == Tile::wall)
								currentTile = southTile;
						}
					}
				}

				//Getting all the colors for posterior manipulation
				TCODColor tileColor = currentTile->color;
				TCODColor tileBackground = currentTile->bg;
				char tileCharacter = currentTile->c;

				float bgValue = tileBackground.getValue();
				float bgHue = tileBackground.getHue();
				float bgSaturation = tileBackground.getSaturation();

				float colValue = tileBackground.getValue();
				float colHue = tileBackground.getHue();
				float colSaturation = tileBackground.getSaturation();

				//Handling water
				if (currentTile->type == Tile::liquid) {
					//Using custom values to add to the current tile's color
					float value = 0;
					float hue = 0;
					float saturation = 0;
					int iter = 0;

					//Continuing the ray downwards
					//Grabbing all the colors along the way and averaging (probably not the correct way to do it)
					for (int g = player->pos.d - depth; g > -1; --g) {
						iter++;
						Tile * currentTile = map->GetTileAt(newPosition.w + iter, newPosition.h + iter, newPosition.d - iter);
						if (currentTile != nullptr) {
							//We get the color of anything except air
							if (currentTile->type != Tile::empty) {
								//Add the values for averaging later
								value += currentTile->bg.getValue();
								hue += currentTile->bg.getHue();
								saturation += currentTile->bg.getSaturation();

								if (currentTile->type == Tile::empty)
									break;
							}
							else {
								value += 1;
							}
						}
						else {
							iter--;
							break;
						}
					}
					bgValue = ((bgValue + value) / (iter + 1));
					tileBackground.setValue(sqrt((bgValue) / (depth + 1)));

					bgHue = ((bgHue)+hue) / (iter + 1);
					tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));

					bgSaturation = ((bgSaturation * 2) + saturation) / (iter + 2);
					tileBackground.setSaturation(bgSaturation);

					TCODConsole::root->setCharForeground(i, j, tileColor);
					TCODConsole::root->setCharBackground(i, j, tileBackground);
					TCODConsole::root->setChar(i, j, tileCharacter);

					break;
				}


				//Doing the color calculations apropriately
				tileBackground.setValue(sqrt((bgValue) / (depth + 1)));
				tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));
				tileColor.setValue(sqrt(colValue) / (depth + 1));
				tileColor.setHue(colHue - (depth*(colHue / (layerSize * 20))));

				//Making sure we are counting the depth looked at.
				depth++;

				//Drawing the blocks now
				TCODConsole::root->setCharForeground(i, j, tileColor);
				TCODConsole::root->setCharBackground(i, j, tileBackground);
				TCODConsole::root->setChar(i, j, tileCharacter);

				//Let's stop the search if we have hit a drawable object
				if (currentTile->type != TileManager::tile_empty)
					break;
			}
		}
	}*/
}

void EngineRenderer::renderMapZoomedOut() {

	/*Let's draw the entire viewable area*/
	/*
	for (int j = 0; j < 64; j ++) {
		for (int i = 0; i < 64; i++) {

			//Let's keep this here so that the code get's smaller.
			//Essentially just getting the position of the player, and drawing everything 32 tiles to the left of him (assuming he is on the center of a screen 64x64).
			//This will serve to make the rest of the code more readable
			Map::Pos currentPosition(player->pos.w + (i*4) - 128, player->pos.h + (j * 4) - 128, player->pos.d);

			//Let's start raycasting down, trying to find the first possible tile we can work with
			//This keeps how far down we went.
			int depth = 0;

			//We are going to start on top and slowly go down
			for (int z = player->pos.d; z > -1; --z) {

				//Let's make sure we are looking at the right tile
				Map::Pos newPosition(currentPosition.w - depth + 1, currentPosition.h - depth + 1, currentPosition.d - depth);

				if (z >= player->pos.d - 1) {
					newPosition = Map::Pos(currentPosition.w, currentPosition.h, currentPosition.d - depth);
				}

				Tile * currentTile = map->GetTileAt(newPosition);

				//Let's skip invalid tiles (they only happen outside of the map so we will skip the rest of the depth search)
				//Or if we have looked farther than we currently allow by the engine.
				if (currentTile == nullptr)
					break;

				//No point in doing anything to an air tile
				if (currentTile->type == TileManager::tile_empty) {
					depth++;
					continue;
				}

				//Let's try to fix some edges
				Tile * blockingTile = map->GetTileAt(newPosition.w + 1, newPosition.h + 1, newPosition.d);

				//I can only fix edges on the bottom layers
				if (z < player->pos.d - 1 && depth != 0 && blockingTile != nullptr) {

					//I give priority to any tile to the bottom and right of the current block
					if (blockingTile->type == Tile::wall) {
						currentTile = blockingTile;
					}
					else {
						//Then I pick either the one to the right, or bottom. If both are just air, then the tile is clearly visible.
						Tile * eastTile = map->GetTileAt(newPosition.w + 1, newPosition.h, newPosition.d);
						Tile * southTile = map->GetTileAt(newPosition.w, newPosition.h + 1, newPosition.d);
						if (eastTile != nullptr && southTile != nullptr) {
							if (eastTile->type == Tile::wall)
								currentTile = eastTile;
							else if (southTile->type == Tile::wall)
								currentTile = southTile;
						}
					}
				}

				//Getting all the colors for posterior manipulation
				TCODColor tileColor = currentTile->color;
				TCODColor tileBackground = currentTile->bg;
				char tileCharacter = currentTile->c;

				float bgValue = tileBackground.getValue();
				float bgHue = tileBackground.getHue();
				float bgSaturation = tileBackground.getSaturation();

				float colValue = tileBackground.getValue();
				float colHue = tileBackground.getHue();
				float colSaturation = tileBackground.getSaturation();

				//Handling water
				if (currentTile->type == Tile::liquid) {
					//Using custom values to add to the current tile's color
					float value = 0;
					float hue = 0;
					float saturation = 0;
					int iter = 0;

					//Continuing the ray downwards
					//Grabbing all the colors along the way and averaging (probably not the correct way to do it)
					for (int g = player->pos.d - depth; g > -1; --g) {
						iter++;
						Tile * currentTile = map->GetTileAt(newPosition.w - iter, newPosition.h - iter, newPosition.d - iter);
						if (currentTile != nullptr) {
							//We get the color of anything except air
							if (currentTile->type != Tile::empty) {
								//Add the values for averaging later
								value += currentTile->bg.getValue();
								hue += currentTile->bg.getHue();
								saturation += currentTile->bg.getSaturation();

								if (currentTile->type == Tile::wall)
									break;
							}
							else {
								value += 1;
							}
						}
						else {
							iter--;
							break;
						}
					}
					bgValue = ((bgValue + value) / (iter + 1));
					tileBackground.setValue(sqrt((bgValue) / (depth + 1)));

					bgHue = ((bgHue)+hue) / (iter + 1);
					tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));

					bgSaturation = ((bgSaturation * 2) + saturation) / (iter + 2);
					tileBackground.setSaturation(bgSaturation);

					TCODConsole::root->setCharForeground(i, j, tileColor);
					TCODConsole::root->setCharBackground(i, j, tileBackground);
					TCODConsole::root->setChar(i, j, tileCharacter);

					break;
				}


				//Doing the color calculations apropriately
				tileBackground.setValue(sqrt((bgValue) / (depth + 1)));
				tileBackground.setHue(bgHue - (depth*(bgHue / (layerSize * 20))));
				tileColor.setValue(sqrt(colValue) / (depth + 1));
				tileColor.setHue(colHue - (depth*(colHue / (layerSize * 20))));

				//Making sure we are counting the depth looked at.
				depth++;

				//Drawing the blocks now
				TCODConsole::root->setCharForeground(i, j, tileColor);
				TCODConsole::root->setCharBackground(i, j, tileBackground);
				TCODConsole::root->setChar(i, j, tileCharacter);

				//Let's stop the search if we have hit a drawable object
				if (currentTile->type != TileManager::tile_empty)
					break;
			}
		}
	}*/
}
