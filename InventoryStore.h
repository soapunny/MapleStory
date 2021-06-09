#pragma once
#include "Singleton.h"
#include "config.h"

class InventoryEntity;
class InventoryStore : public Singleton<InventoryStore>
{
private:
	map<string, InventoryEntity*>* mInventoryStore;

public:
	void SaveInventory();
	void LoadInventory(string characterName);
	void Release();

	map<string, InventoryEntity*>* GetMInventoryStore() { return mInventoryStore; }
};

