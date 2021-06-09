#include "InventoryStore.h"
#include "FileManager.h"
#include "InventoryEntity.h"
#include "ConsumableItemEntity.h"

void InventoryStore::SaveInventory()
{
	//TODO scene 이동 할 때마다 저장되게 한다.
}

void InventoryStore::LoadInventory(string characterName)
{
	if (!mInventoryStore)
	{
		mInventoryStore = new map<string, InventoryEntity*>;
	}
	//TODO Make ReadInventoryData function in FileManager
	mInventoryStore->insert(make_pair(characterName, FileManager::GetSingleton()->ReadInventoryData(characterName)) );
}

void InventoryStore::Release()
{
	if (mInventoryStore)
	{
		for (auto inventoryPair : *mInventoryStore)
		{
			if (inventoryPair.second)
			{
				for (auto entityPair : (inventoryPair.second)->GetConsumableItem())
				{
					if (entityPair.first)
					{
						ConsumableItemEntity* consumableItemEntity = entityPair.first;
						(inventoryPair.second)->GetConsumableItem().erase(consumableItemEntity);
						SAFE_DELETE(consumableItemEntity);
					}
				}
				SAFE_DELETE(inventoryPair.second);
			}
		}
		mInventoryStore->clear();
		SAFE_DELETE(mInventoryStore);
	}
	
	ReleaseSingleton();
}
