#pragma once
#include "config.h"

class CharacterDTO;
class CharacterEntity;
class CharDataTellerMachine
{
private:
	void UpdateCharacterData(CharacterEntity* characterEntity);
public:
	void SaveCharacter(CharacterDTO* character);
	map<string, CharacterDTO*>* LoadCharacter(string name);
	void Release();
};

