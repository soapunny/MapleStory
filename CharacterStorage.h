#pragma once
#include "Singleton.h"
#include "config.h"

class CharacterEntity;
class CharacterStorage : public Singleton<CharacterStorage>
{
private:
	map<string, CharacterEntity*>* mCharacterStorage;

public:
	void SaveCharacter();
	void LoadCharacter(string name);
	void Release();

	inline map<string, CharacterEntity*>* GetMCharacterStore() { return mCharacterStorage; }
};

