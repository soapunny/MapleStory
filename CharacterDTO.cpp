#include "CharacterDTO.h"
#include "CharacterEntity.h"

CharacterDTO::CharacterDTO(CharacterEntity* characterEntity)
{
	this->name = characterEntity->GetName();
	this->currentMap = characterEntity->GetCurrentMap();
	this->occupation = characterEntity->GetOccupation();
	this->worldPos = characterEntity->GetWorldPos();
	this->image = characterEntity->GetImage();

	this->level = characterEntity->GetLevel();
	this->exp = characterEntity->GetExp();
	this->str = characterEntity->GetStr();
	this->dex = characterEntity->GetDex();
	this->intel = characterEntity->GetInt();
	this->luck = characterEntity->GetLuk();

	this->currHP = characterEntity->GetCurrHP();
	this->currMP = characterEntity->GetCurrMP();
	this->currAP = characterEntity->GetCurrAP();
}

CharacterEntity* CharacterDTO::TurnItIntoCharacterEntity()
{
	CharacterEntity* characterEntity = new CharacterEntity;
	characterEntity->SetName(this->name);
	characterEntity->SetCurrentMap(this->currentMap);
	characterEntity->SetOccupation(this->occupation);
	characterEntity->SetWorldPos(this->worldPos.x, this->worldPos.y);
	characterEntity->SetImage(this->image);

	characterEntity->SetLevel(this->level);
	characterEntity->SetExp (this->exp);
	characterEntity->SetStr (this->str);
	characterEntity->SetDex (this->dex);
	characterEntity->SetInt (this->intel);
	characterEntity->SetLuk ( this->luck);

	characterEntity->SetCurrHP(this->currHP);
	characterEntity->SetCurrMP(this->currMP); 
	characterEntity->SetCurrAP(this->currAP);

	return characterEntity;
}
