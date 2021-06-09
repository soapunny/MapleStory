#pragma once
#include "config.h"
using namespace std;

class ConsumableItemEntity;
class QuestEntity;
class Image;
class NPCEntity
{
private:
	//key
	string npcName;	//npc �̸�
	POINT worldPos;	//�ʿ��� npc�� ��ġ
	Image* image;	//npc �̹���
	bool isNPCClicked;

	//SHOP
	list<ConsumableItemEntity*> lStockConsumableItems;//�Һ� ������
	
	//QUEST
	list<QuestEntity*> lQuests;//����Ʈ


public:

	//Setter
	inline void SetNPCName(string npcName) { this->npcName = npcName; }
	inline void SetWorldPos(int posX, int posY) { this->worldPos.x = posX; this->worldPos.y = posY; }
	inline void SetImage(Image* image) { this->image = image; }
	inline void SetNPCClicked(bool isNPCClicked) { this->isNPCClicked = isNPCClicked; }
	
	inline void push_back_consumable_item(ConsumableItemEntity* consumableItemEntity) { lStockConsumableItems.push_back(consumableItemEntity); }
	inline void push_back_quest(QuestEntity* questEntity) { lQuests.push_back(questEntity); }

	//Getter
	inline string GetNPCName() { return npcName; }
	inline POINT GetWorldPos() { return worldPos; }
	inline Image* GetImage() { return image; }
	inline bool GetNPCClicked() { return isNPCClicked; }

	inline list<ConsumableItemEntity*> GetLStockConsumableItems() { return lStockConsumableItems; }
	inline list<QuestEntity*> GetQuest() { return lQuests; }

};

