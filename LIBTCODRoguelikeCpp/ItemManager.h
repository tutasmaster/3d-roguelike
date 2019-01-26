#pragma once
#include <string>
#include <vector>
#include <libtcod.hpp>

class Item {
public:
	std::string name = "Item";
	std::string shrtnm = "ITM";
	char ch = 'I';

	TCODColor bgCol = TCODColor::black;
	TCODColor chCol = TCODColor::white;

	unsigned char maxStack = 100;
	int ATK = 0;
	bool isAttackItem = false;
	
};

typedef unsigned char ItemID;

class ItemManager
{
public:
	ItemManager();
	Item* GetItemData(ItemID item);
	std::vector<Item> items;
	~ItemManager();

	int total = 0;

	enum itemIDs : unsigned char{
		item_potion,
		item_gold
	};
};

extern ItemManager itemManager;