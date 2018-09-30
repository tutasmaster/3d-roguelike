#include "ItemManager.h"



ItemManager::ItemManager()
{
	
}

Item * ItemManager::GetItemData(ItemID item) {
	if (item < items.size() && item > -1)
		return &items.at(item);
	return nullptr;
}

ItemManager::~ItemManager()
{
}
