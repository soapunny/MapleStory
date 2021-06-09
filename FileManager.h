#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
using namespace std;

#define END_OF_ELEMENT '/'
#define END_OF_WORD ','
#define END_OF_LINE '#'
#define DOUBLE_QUOTATION_MARK '"'

enum class FILE_NAME{CHARACTER, MONSTER, ITEM_CONSUMABLE, ITEM_WEARABLE, INVENTORY_CONSUMABLE, INVENTORY_WEARABLE, SKILL, NPC, MAP, END_OF_FILE_NAME};

// Rule #1. A line ends with '#'
//

class Character;
class Monster;
class ConsumableItemEntity;
class NPCEntity;
class InventoryEntity;
class CharacterEntity;
enum class MAP_NAME;
enum class CONSUMABLE_ITEMS;
class FileManager : public Singleton<FileManager>
{
private:
	vector<string> vFileNames;

	void ReadLine(fstream& fp);
	string ReadWord(fstream& fp);
	string ReadElement(string& word);

	void WriteTitle(fstream& fp, string word);
	void WriteWord(fstream& fp, string word, bool isEndOfLine = false);

public:
	void Init();

	map<string, CharacterEntity*>* ReadCharacterData(string userName);
	void ReadMonsterData(Monster* monster);
	ConsumableItemEntity* ReadConsumableItem(CONSUMABLE_ITEMS consumableItem);
	void ReadConsumableItems(map<CONSUMABLE_ITEMS, ConsumableItemEntity*>* mConsumableItemStorage);
	void ReadSkillData();
	void ReadNPCData();
	void ReadNPCData(map<string, NPCEntity*>* mNPCStorage, MAP_NAME mapName);
	void ReadMapData();
	InventoryEntity* ReadInventoryData(string characterName);

	void SaveCharacterData(const map<string, CharacterEntity*>* character);
	void SaveMonsterData();
	void SaveSkillData();
	void SaveNPCData();
	void SaveMapData();
};

