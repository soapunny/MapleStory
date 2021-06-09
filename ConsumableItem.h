#pragma once
#include "Item.h"

class Image;
class ConsumableItemEntity;
class ConsumableItem : public Item
{
private:
	Image* image;
	POINT consumableItemFrame;
	float durationTimer;

	//Same fields in ConsumableItemEntity
	CONSUMABLE_ITEMS itemType;
	string itemName;
	string explanation;
	int price;
	int maxBundleCnt;

	int hpRecovery;
	int mpRecovery;
	float hpRecoveryRate;
	float mpRecoveryRate;

	float physicalDamage;
	float magicalDamage;
	float accuracy;
	float defense;
	float moveSpeed;

	int str;
	int dex;
	int intel;
	int luck;
	float duration;

public:
	ConsumableItem(ConsumableItemEntity* consumableItemEntity);

	//Setter
	inline void SetItemType(CONSUMABLE_ITEMS itemType) { this->itemType = itemType; }
	inline void SetItemName(string itemName) { this->itemName = itemName; }
	inline void SetExplanation(string explanation) { this->explanation = explanation; }
	inline void SetPrice(int price) { this->price = price; }
	inline void SetMaxBundleCnt(int maxBundleCnt) { this->maxBundleCnt = maxBundleCnt; }

	inline void SetHpRecovery(int hpRecovery) { this->hpRecovery = hpRecovery; }
	inline void SetMpRecovery(int mpRecovery) { this->mpRecovery = mpRecovery; }
	inline void SetHpRecoveryRate(float hpRecoveryRate) { this->hpRecoveryRate = hpRecoveryRate; }
	inline void SetMpRecoveryRate(float mpRecoveryRate) { this->mpRecoveryRate = mpRecoveryRate; }

	inline void SetPhysicalDamage(float physicalDamage) { this->physicalDamage = physicalDamage; }
	inline void SetMagicalDamage(float magicalDamage) { this->magicalDamage = magicalDamage; }
	inline void SetAccuracy(float accuracy) { this->accuracy = accuracy; }
	inline void SetDefense(float defense) { this->defense = defense; }
	inline void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }

	inline void SetStr(int str) { this->str = str; }
	inline void SetDex(int dex) { this->dex = dex; }
	inline void SetInt(int intel) { this->intel = intel; }
	inline void SetLuk(int luck) { this->luck = luck; }

	inline void SetDuration(float duration) { this->duration = duration; }

	//Getter
	inline CONSUMABLE_ITEMS GetItemType() { return itemType; }
	inline string GetItemName() { return itemName; }
	inline string GetExplanation() { return explanation; }
	inline int GetPrice() { return price; }
	inline int GetMaxBundleCnt() { return maxBundleCnt; }

	inline int GetHpRecovery() { return hpRecovery; }
	inline int GetMpRecovery() { return mpRecovery; }
	inline float GetHpRecoveryRate() { return hpRecoveryRate; }
	inline float GetMpRecoveryRate() { return mpRecoveryRate; }

	inline float GetPhysicalDamage() { return physicalDamage; }
	inline float GetMagicalDamage() { return magicalDamage; }
	inline float GetAccuracy() { return accuracy; }
	inline float GetDefense() { return defense; }
	inline float GetMoveSpeed() { return moveSpeed; }

	inline int GetStr() { return str; }
	inline int GetDex() { return dex; }
	inline int GetInt() { return intel; }
	inline int GetLuk() { return luck; }
	inline float GetDuration() { return this->duration; }

};

