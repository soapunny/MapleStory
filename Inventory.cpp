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
			(*vItems)[j] = make_pair(item, 2);//�ΰ��� �������� �����Ѵ�.
		}
		//���� �ǿ� �ҷ��� ������ ���͸� ����
		vCurrItems = vItems;
		currItemType = ITEM_TYPE::CONSUMABLE_ITEM;

		mItemStorage.insert(make_pair(ITEM_TYPE::CONSUMABLE_ITEM, vItems));//map�� Ÿ�Ժ� �����۱׷��� �����Ѵ�.
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

//TODO ������ ���� �Լ��� �����ϱ�


void Inventory::Render(HDC hdc, int startPosX, int startPosY)
{
	if (!vCurrItems)
		return;

	int idx = 0;//���° ���������� ��������

	
	for (auto itemsPair : *vCurrItems)
	{
		if (itemsPair.second > 0)
		{

			itemsPair.first->Render(hdc);//TODO �ش� �������� �־��� �κ��丮 â�� ����� ��µǰ� �ؾ��Ѵ�.
		}
		idx++;
	}
}
