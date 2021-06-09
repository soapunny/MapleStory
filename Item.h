#pragma once
#include "GameNode.h"

enum class CONSUMABLE_ITEMS { 
	RED_POTION, ORANGE_POTION, WHITE_POTION, BLUE_POTION, ELIXIR, POWER_ELIXIR, MANA_ELIXIR, 
	RED_POTION_PILL, ORANGE_POTION_PILL, WHITE_POTION_PILL, BLUE_POTION_PILL, SPECIAL_PILL, FAKE_ELIXIR, 
	WATERMELON, FREEZIE, RED_BEAN_ICE_FLAKES, ARCHER_POTION, SPEED_POTION, WIZARD_POTION, THIEF_POTION,
	END_OF_CONSUMABLE_ITEMS 
};

class Item : public GameNode
{
protected:

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual ~Item() {}
};

