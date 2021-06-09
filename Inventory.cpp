#include "Inventory.h"
#include "Item.h"
#include "Managers.h"
#include "Image.h"

HRESULT Inventory::Init()
{

	for (int i = 0;i < static_cast<int>(ITEM_TYPE::END_OF_ITEM_TYPE);i++)
	{
		vector<pair<Item*, int>>* vItems = new vector<pair<Item*, int>>;
		vItems->resize(static_cast<int>(CONSUMABLE_ITEMS::END_OF_CONSUMABLE_ITEMS));

		for (int j = 0; j < vItems->size(); j++)
		{
			Item* item = new Item;
			item->Init();
			(*vItems)[j] = make_pair(item, 2);//두개씩 아이템을 생성한다.
		}
		//현재 탭에 불러올 아이템 백터를 저장
		vCurrItems = vItems;
		currItemType = ITEM_TYPE::CONSUMABLE_ITEM;

		mItemStorage.insert(make_pair(ITEM_TYPE::CONSUMABLE_ITEM, vItems));//map에 타입별 아이템그룹을 저장한다.
	}

	return S_OK;
}

void Inventory::Release()
{
	//map<ITEM_TYPE, vector<pair<Item*, int>>* >
	for (auto itemSet : mItemStorage)
	{
		vector<pair<Item*, int>>* tmpVec = itemSet.second;

		for (auto itemPair : *tmpVec)
		{
			SAFE_DELETE(itemPair.first);
		}
		SAFE_DELETE(tmpVec);
	}
	mItemStorage.clear();
}

void Inventory::Update()
{
	if (!vCurrItems)
		return;

}

//TODO 아이템 정렬 함수들 구현하기


void Inventory::Render(HDC hdc, int startPosX, int startPosY)
{
	if (!vCurrItems)
		return;

	int idx = 0;//몇번째 아이템인지 세기위해

	
	for (auto itemsPair : *vCurrItems)
	{
		if (itemsPair.second > 0)
		{

			itemsPair.first->Render(hdc);//TODO 해당 아이템을 주어진 인벤토리 창에 제대로 출력되게 해야한다.
		}
		idx++;
	}
}
