#pragma once
#include "GameNode.h"

enum class MAP_NAME;
class ConsumableItem;
class Quest;
class Image;
class NPC : public GameNode
{
private:
	//key
	string npcName;	//npc 이름
	POINT worldPos;	//맵에서 npc의 위치
	Image* image;	//npc 이미지

	//SHOP
	list<ConsumableItem*> lStockConsumableItems;//소비 아이템

	//QUEST
	list<Quest*> lQuests;//퀘스트


	POINT localPos;//카메라가 해당 worldPos내에 들어오면 계산

public:
	virtual ~NPC() {}
	inline virtual string InstanceOf() { return string{ typeid(this).name() }; }

	virtual HRESULT Init();			
	virtual void Release();			
	virtual void Update();			
	virtual void Render(HDC hdc);

	//Setter
	inline void SetNPCName(string npcName) { this->npcName; }
	inline void SetWorldPos(int posX, int posY) { this->worldPos.x = posX; this->worldPos.y = posY; }
	inline void SetImage(Image* image) { this->image = image; }


	//Getter
	inline string GetNPCName() { return npcName; }
	inline POINT GetWorldPos() { return worldPos; }
	inline Image* GetImage() { return image; }

	inline list<ConsumableItem*> GetLStockConsumableItems() { return lStockConsumableItems; }
	inline list<Quest*> GetQuest() { return lQuests; }
};

