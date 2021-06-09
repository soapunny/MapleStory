#pragma once
#include "config.h"
#include "Singleton.h"

class ConsumableItemEntity;
enum class CONSUMABLE_ITEMS;
class ItemStorage : public Singleton<ItemStorage>
{
private:
	map<CONSUMABLE_ITEMS, ConsumableItemEntity*>* mConsumableItemStorage;
	//TODO Develop other itemStorages

public:
	void LoadAll();
	void Release();

	const ConsumableItemEntity* FindConsumableItem(CONSUMABLE_ITEMS itemType);
	//TODO Develop other Find...() funcions by their type
};

