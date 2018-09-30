#include "ItemManager.h"



ItemManager::ItemManager()
{
	Item potion;
	potion.name = "Magic Potion";
	potion.ch = '?';
	potion.chCol = TCODColor::pink;
	potion.shrtnm = "PTN";
	items.push_back(potion);

	Item gold;
	gold.name = "Gold";
	gold.ch = 'O';
	gold.chCol = TCODColor::gold;
	gold.bgCol = TCODColor::darkOrange;
	gold.shrtnm = "PTN";
	items.push_back(gold);
	
}

Item * ItemManager::GetItemData(ItemID item) {
	if (item < items.size() && item > -1)
		return &items.at(item);
	return nullptr;
}

ItemManager::~ItemManager()
{
}
