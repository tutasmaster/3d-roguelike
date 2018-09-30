#include "GUI.h"
#include "engine.hpp"
#include <string>

AnnouncementsGUI::AnnouncementsGUI() : GUI(62, 62) {
	x = 1; y = 1;
	isFullscreen = false;
}

void AnnouncementsGUI::Update() {
	auto last_key = engine.keyboardInput.pressed ? engine.keyboardInput : TCOD_key_t();


	switch (last_key.vk) {
	case TCODK_ESCAPE:
		engine.GUI_ID = -1;
		break;
	case TCODK_UP:
		cursorPos--;
		break;
	case TCODK_DOWN:
		cursorPos++;
		break;
	}

	if (cursorPos < 0)
		cursorPos = 0;
	else if (cursorPos > (engine.console.size()) - 1)
		cursorPos = (engine.console.size()) - 1;

	console.clear();

	int i = 0;
	for (auto announcement : engine.console) {
		console.setDefaultForeground(announcement.col);
		console.print(1, ((i - cursorPos) * 2) + 5, announcement.msg.c_str());
		i++;
	}

	console.rect(0, 0, 62, 5, true);

	console.setDefaultForeground(TCODColor::white);
	console.print(1, 1, "Announcements");

}

InventoryGUI::InventoryGUI() : GUI(24,62){
	isFullscreen = false;
	x = 39; y = 1;
	mapOffsetX = 12;
}

void InventoryGUI::Update() {
	auto last_key = engine.keyboardInput.pressed ? engine.keyboardInput : TCOD_key_t();

	int invSize = engine.player->inv->item_vector.size();

	switch (last_key.vk){
	case TCODK_ESCAPE:
		engine.GUI_ID = -1;
		break;
	case TCODK_UP:
		cursorPos--;
		break;
	case TCODK_DOWN:
		cursorPos++;
		break;
	case TCODK_ENTER:
	case TCODK_KPENTER:
		if (invSize > 0) {
			Use(engine.player->inv->item_vector.at(cursorPos).first, cursorPos);
		}
		engine.GUI_ID = -1;
		break;
	}

	if (cursorPos < 0)
		cursorPos = 0;
	else if (cursorPos > engine.player->inv->item_vector.size() - 1)
		cursorPos = engine.player->inv->item_vector.size() - 1;

	console.setDefaultBackground(TCODColor::black);
	console.clear();
	console.print(1, 1, menuName.c_str());

	int i = 0;
	for (auto item : engine.player->inv->item_vector) {
		bool isSelected = i == cursorPos;
		console.print(1 + isSelected, i + 3, itemManager.GetItemData(item.first)->name.c_str());
		char num[4];
		sprintf_s(num, "%03d", item.second);

		if (isSelected)
			console.print(1, i + 3, ">");

		console.printRectEx(22, i + 3, 3, 1, TCOD_BKGND_DEFAULT, TCOD_RIGHT, num);

		i++;
	}

	if (invSize == 0) {
		console.print(1, 3, "Your inventory seems\nto be empty!\nGo grab some loot!");
	}
}

void InventoryGUI::Use(int itemID, int itemPos) {
	Message msg;
	switch (itemID) {
	case 0:
		msg.col = TCODColor::cyan;
		msg.msg = "You used the " + itemManager.GetItemData(itemID)->name + ". Refreshing...";
		engine.console.push_back(msg);
		break;
	default:
		msg.col = TCODColor::cyan;
		msg.msg = "You tried using " + itemManager.GetItemData(itemID)->name + " but you don't really know how.";
		engine.console.push_back(msg);
		break;
	}
}

GUI::GUI(int w, int h) : console(w , h)
{
	width = w;
	height = h;
}


GUI::~GUI()
{
}

DropGUI::DropGUI() : InventoryGUI()
{
	menuName = "Drop";
}

void DropGUI::Use(int itemID, int itemPos){
	if (engine.player->inv->item_vector.at(itemPos).second < 1) {
		return;
	}
	
	engine.player->inv->item_vector.at(itemPos).second--;
	engine.player->inv->DeleteRemainingItems();


	Message msg;
	msg.col = TCODColor::gold;
	msg.msg = "You drop a " + itemManager.GetItemData(itemID)->name + "!";
	engine.console.push_back(msg);


	std::shared_ptr<Entity> item = std::make_shared<Entity>();
	item->c = itemManager.GetItemData(itemID)->ch;
	item->col = itemManager.GetItemData(itemID)->chCol;
	item->isColliding = false;
	item->pos = engine.player->pos;
	item->isItem = true;
	item->itemID = itemID;
	engine.npcs.push_back(item);
}