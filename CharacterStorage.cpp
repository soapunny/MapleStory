#include "CharacterStorage.h"
#include "FileManager.h"

void CharacterStorage::SaveCharacter()
{
	if (mCharacterStorage && mCharacterStorage->size()>0)
	{
		FileManager::GetSingleton()->SaveCharacterData(mCharacterStorage);
	}
}

void CharacterStorage::LoadCharacter(string name)
{
	if (mCharacterStorage)
		Release();
	this->mCharacterStorage = FileManager::GetSingleton()->ReadCharacterData(name);
}

void CharacterStorage::Release()
{
	if (mCharacterStorage)
	{
		for (auto storagePair : *mCharacterStorage)
		{
			if (storagePair.second)
			{
				SAFE_DELETE(storagePair.second);
			}
		}
		mCharacterStorage->clear();
		SAFE_DELETE(mCharacterStorage);
	}
}
