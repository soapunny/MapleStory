#pragma once
#include "Singleton.h"
#include "config.h"

enum class MAP_NAME;
class NPCEntity;
class NPCStorage : public Singleton<NPCStorage>
{
private:
	map<string, NPCEntity*>* mNPCStorage;

public:
	void LoadLocalNPC(MAP_NAME mapName);
	void Release();

	NPCEntity* FindNPCBy(string name);
	map<string, NPCEntity*>* GetMNPCStorage() { return mNPCStorage; }
};

