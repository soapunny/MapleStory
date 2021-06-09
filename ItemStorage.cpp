#include "ItemStorage.h"
#include "FileManager.h"

void ItemStorage::LoadAll()
{
	if (!mConsumableItemStorage)//In case the storage is not empty
	{
		mConsumableItemStorage = new map<CONSUMABLE_ITEMS, ConsumableItemEntity*>;
		FileManager::GetSingleton()->ReadConsumableItems(mConsumableItemStorage);
	}
}

void ItemStorage::Release()
{
	if (mConsumableItemStorage)
	{
		ConsumableItemEntity* item;
		for (auto itemPair : *mConsumableItemStorage)
		{
			if ((item = itemPair.second) )
			{
				SAFE_DELETE(item);
			}
		}
		mConsumableItemStorage->clear();
		SAFE_DELETE(mConsumableItemStorage);
	}

	ReleaseSingleton();
}

const ConsumableItemEntity* ItemStorage::FindConsumableItem(CONSUMABLE_ITEMS itemType)
{
	if (mConsumableItemStorage && !mConsumableItemStorage->empty())
	{
		auto iter = mConsumableItemStorage->find(itemType);
		if (iter->second)
		{
			return iter->second;
		}
	}
	return nullptr;
}
