#pragma once
#include "GameNode.h"

enum class ITEM_TYPE{CONSUMABLE_ITEM, END_OF_ITEM_TYPE};
enum class SORT_BY{NAME, COUNT, END_OF_SORT_BY};

class Image;
class Item;
class Inventory : public GameNode
{
private:
	map<ITEM_TYPE, vector<pair<Item*, int>>* > mItemStorage;
	vector<pair<Item*, int>>* vCurrItems;
	ITEM_TYPE currItemType;


public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc, int startPosX, int startPosY);
	virtual ~Inventory(){}
};

