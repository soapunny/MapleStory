#include "NPCStorage.h"
#include "NPCEntity.h"
#include "FileManager.h"

//TODO NPC ���� ������ ����� ������ ��ġ, ��ũ�� ��� �߰��ϱ�

void NPCStorage::LoadLocalNPC(MAP_NAME mapName)
{
	if (!mNPCStorage)
	{
		mNPCStorage = new map<string, NPCEntity*>;
		FileManager::GetSingleton()->ReadNPCData(mNPCStorage, mapName);
	}
}

void NPCStorage::Release()
{
	if (mNPCStorage)
	{
		NPCEntity* tmpEntity = nullptr;
		for (auto npcPair : *mNPCStorage)
		{
			tmpEntity = npcPair.second;
			if (tmpEntity)
			{
				SAFE_DELETE(tmpEntity);
			}
		}
		mNPCStorage->clear();
		SAFE_DELETE(mNPCStorage);
	}
	ReleaseSingleton();
}


NPCEntity* NPCStorage::FindNPCBy(string name)
{
	auto npcPair = mNPCStorage->find(name);
	if (npcPair != mNPCStorage->end())
	{
		return npcPair->second;
	}

	return nullptr;
}
