#include "InventoryEntity.h"
#include "ConsumableItemEntity.h"

void InventoryEntity::insert_consumable_item(ConsumableItemEntity* consumableItemEntity, int count)
{
	unordered_map<ConsumableItemEntity*, int>::iterator it = Find(consumableItemEntity->GetItemName());
	if (it == hmConsumableItems.end())
		this->hmConsumableItems.insert(make_pair(consumableItemEntity, count));
	else
	{
		pair<ConsumableItemEntity*, int> itemPair = *it;
		this->hmConsumableItems.erase(it);
		this->hmConsumableItems.insert(make_pair(itemPair.first, itemPair.second + count));
	}
}

unordered_map<ConsumableItemEntity*, int>::iterator InventoryEntity::Find(string name)
{
	unordered_map<ConsumableItemEntity*, int>::iterator it = hmConsumableItems.begin();
	for (; it != hmConsumableItems.end(); it++)
	{
		if (it->first->GetItemName() == name)
			return it;
	}
	return it;
}
