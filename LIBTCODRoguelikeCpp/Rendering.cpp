#include "engine.hpp"


void EngineRenderer::renderCompass(char angle) {

	for (int i = 1; i < 6; i++) {
		for (int j = 1; j < 6; j++) {
			drawFullCharacter(i, j, ' ', TCODColor::black, TCODColor::black);
		}
	}

	drawFullCharacter(3, 1, 'N', TCODColor::yellow, TCODColor::black);
	drawFullCharacter(5, 3, 'E', TCODColor::yellow, TCODColor::black);
	drawFullCharacter(1, 3, 'W', TCODColor::yellow, TCODColor::black);
	drawFullCharacter(3, 5, 'S', TCODColor::yellow, TCODColor::black);

	switch (angle) {
	//NORTH
	case 0:
		drawFullCharacter(3, 2, '^', TCODColor::white, TCODColor::black);
		drawFullCharacter(3, 3, '|', TCODColor::white, TCODColor::black);
		break;
	//WEST
	case 1:
		drawFullCharacter(3, 3, '-', TCODColor::white, TCODColor::black);
		drawFullCharacter(2, 3, '<', TCODColor::white, TCODColor::black);
		break;
	//SOUTH
	case 2:
		drawFullCharacter(3, 3, '|', TCODColor::white, TCODColor::black);
		drawFullCharacter(3, 4, 'v', TCODColor::white, TCODColor::black);
		break;
	//EAST
	case 3:
		drawFullCharacter(3, 3, '-', TCODColor::white, TCODColor::black);
		drawFullCharacter(4, 3, '>', TCODColor::white, TCODColor::black);
		break;
	}
}

void EngineRenderer::drawFullCharacter(int x, int y, char c, const TCODColor col, const TCODColor bg) {
	TCODConsole::root->setCharForeground(x, y, col);
	TCODConsole::root->setCharBackground(x, y, bg);
	TCODConsole::root->setChar(x, y, c);
}