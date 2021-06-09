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
	string npcName;	//npc 이름
	POINT worldPos;	//맵에서 npc의 위치
	Image* image;	//npc 이미지
	bool isNPCClicked;

	//SHOP
	list<ConsumableItemEntity*> lStockConsumableItems;//소비 아이템
	
	//QUEST
	list<QuestEntity*> lQuests;//퀘스트


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

