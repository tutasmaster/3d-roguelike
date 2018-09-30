#pragma once
#include "libtcod.hpp"
#include "Entity.h"


class GUI
{
public:
	GUI(int w, int h);
	~GUI();

	TCODConsole console;

	bool isFullscreen;

	int x = 0, y = 0, width = 0, height = 0;
	int mapOffsetX = 0, mapOffsetY = 0;

	virtual void Update() = 0;
};


class InventoryGUI : public GUI{
public:
	InventoryGUI();

	std::string menuName = "Inventory";

	void Update() override;
	virtual void Use(int itemID);
	int cursorPos = 0;
};

class DropGUI : public InventoryGUI {
public:
	DropGUI();

	void Use(int itemID) override;
};

class AnnouncementsGUI : public GUI {
public:
	AnnouncementsGUI();

	int cursorPos = 0;
	void Update() override;
};
