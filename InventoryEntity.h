#pragma once
#include <string>
#include <unordered_map>
using namespace std;

class ConsumableItemEntity;
class InventoryEntity
{
private:
	//key
	string characterName;

	int money;

	//Consumable items
	unordered_map<ConsumableItemEntity*, int> hmConsumableItems;

public:
	//Setter
	inline void SetCharacterName(string name) { this->characterName = name; }
	inline void SetMoney(int money) { this->money = money; }
	void insert_consumable_item(ConsumableItemEntity* consumableItemEntity, int count);
	unordered_map<ConsumableItemEntity*, int>::iterator Find(string name);

	//Getter
	inline string GetCharacterName() { return characterName; }
	inline int GetMoney() { return money; }
	inline unordered_map<ConsumableItemEntity*, int> GetConsumableItem() { return hmConsumableItems; }

	//TODO Inventory class에 다음에 정렬기능 만들어주기
};

