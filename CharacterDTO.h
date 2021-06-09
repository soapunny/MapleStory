#pragma once
#include "config.h"

enum class Occupation;
enum class MAP_NAME;
class Image;
class CharacterEntity;
class CharacterDTO
{
	//key
	string name;

	MAP_NAME currentMap;
	Occupation occupation;
	FPOINT worldPos;
	Image* image;

	int level;
	int exp;
	int str;
	int dex;
	int intel;
	int luck;

	int currHP;
	int currMP;
	int currAP;
	//TODO skill 관련은 characterSkill에서

public:
	CharacterDTO(CharacterEntity* characterEntity);
	CharacterEntity* TurnItIntoCharacterEntity();

	//Setter
	inline void SetName(string name) { this->name = name; }
	inline void SetCurrentMap(MAP_NAME mapName) { this->currentMap = mapName; }
	inline void SetOccupation(Occupation occupation) { this->occupation = occupation; }
	inline void SetWorldPos(float worldPosX, float worldPosY) { this->worldPos.x = worldPosX, this->worldPos.y = worldPosY; }
	inline void SetImage(Image* image) { this->image = image; }

	inline void SetLevel(int level) { this->level = level; }
	inline void SetExp(int exp) { this->exp = exp; }
	inline void SetStr(int str) { this->str = str; }
	inline void SetDex(int dex) { this->dex = dex; }
	inline void SetInt(int intel) { this->intel = intel; }
	inline void SetLuk(int luck) { this->luck = luck; }

	inline void SetCurrHP(int hp) { this->currHP = hp; }
	inline void SetCurrMP(int mp) { this->currMP = mp; }
	inline void SetCurrAP(int currAP) { this->currAP = currAP; }

	//Getter
	inline string GetName() { return name; }
	inline MAP_NAME GetCurrentMap() { return currentMap; }
	inline Occupation GetOccupation() { return occupation; }
	inline FPOINT GetWorldPos() { return worldPos; }
	inline Image* GetImage() { return image; }

	inline int GetLevel() { return level; }
	inline int GetExp() { return exp; }
	inline int GetStr() { return str; }
	inline int GetDex() { return dex; }
	inline int GetInt() { return intel; }
	inline int GetLuk() { return luck; }

	inline int GetCurrHP() { return currHP; }
	inline int GetCurrMP() { return currMP; }
	inline int GetCurrAP() { return currAP; }
};

