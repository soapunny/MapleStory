#pragma once
#include "Character.h"
#include "Managers.h"
#include "Monster.h"
#include "Item.h"
#include "Map.h"
#include "ConsumableItemEntity.h"
#include "NPCEntity.h"
#include "InventoryEntity.h"
#include "CharacterEntity.h"
#include <fstream>

void FileManager::ReadLine(fstream& fp)
{
	char tmpChar = '\0';
	while (tmpChar != END_OF_LINE)
	{
		fp.get(tmpChar);
	}
	//Skip '\n', '#', ' '
	while (fp.peek() == '\n' || fp.peek() == END_OF_LINE || fp.peek() == ' ')
		fp.get(tmpChar);
}

string FileManager::ReadWord(fstream& fp)
{
	string result = "";

	char tmpChar = '\0';
	//comma이거나 #을 만나면 skip
	while (tmpChar != END_OF_WORD && tmpChar != END_OF_LINE)
	{
		fp.get(tmpChar);
		if(tmpChar != DOUBLE_QUOTATION_MARK)
			result.push_back(tmpChar);
	}
	while (fp.peek() == '\n' || fp.peek() == END_OF_LINE || fp.peek() == ' ')
		fp.get(tmpChar);
	//Delete '#' or ',' from the back
	if (result.length() == 0)
		return "";
	if(result.back() == END_OF_WORD || result.back() == END_OF_LINE)
		result.pop_back();
	//Skip '\n', '#', ' ' ...

	return result;
}

string FileManager::ReadElement(string& word)
{
	if (word.empty())
		return "";

	string result = "";
	char tmpChar = '\0';
	//element, comma, #을 만나면 skip
	while (!word.empty() && tmpChar != END_OF_ELEMENT && tmpChar != END_OF_WORD && tmpChar != END_OF_LINE)
	{
		tmpChar = word[0];
		word.erase(word.begin());
		if (tmpChar != DOUBLE_QUOTATION_MARK)
			result.push_back(tmpChar);
	}
	if (result.length() == 0)
		return "";
	//Delete '/', '#' or ',' from the back
	if (result.back() == END_OF_ELEMENT || result.back() == END_OF_WORD || result.back() == END_OF_LINE)
		result.pop_back();

	return result;
}

void FileManager::WriteTitle(fstream& fp, string word)
{
	word.append("#\n");
	fp.write(word.c_str(), strlen(word.c_str()));
}

void FileManager::WriteWord(fstream& fp, string word, bool isEndOfLine)
{
	if (isEndOfLine)
	{
		word.append(",#\n");
		fp.write(word.c_str(), strlen(word.c_str()));
	}
	else
	{
		word.append(",");
		fp.write(word.c_str(), strlen(word.c_str()));
	}
}

void FileManager::Init()
{
	vFileNames.resize(static_cast<int>(FILE_NAME::END_OF_FILE_NAME));
	
	vFileNames[static_cast<int>(FILE_NAME::CHARACTER)] = "Save/characterData.ini";
	vFileNames[static_cast<int>(FILE_NAME::MONSTER)] = "Save/monsterData.ini";
	vFileNames[static_cast<int>(FILE_NAME::ITEM_CONSUMABLE)] = "Save/consumableItemData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::ITEM_WEARABLE)] = "Save/wearableItemData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::INVENTORY_CONSUMABLE)] = "Save/consumableInventoryData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::INVENTORY_WEARABLE)] = "Save/wearableInventoryData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::SKILL)] = "Save/skillData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::SKILL)] = "Save/skillData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::NPC)] = "Save/npcData.CSV";
	vFileNames[static_cast<int>(FILE_NAME::MAP)] = "Save/mapData.CSV";
}

map<string, CharacterEntity*>* FileManager::ReadCharacterData(string userName)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::CHARACTER)].c_str(), ios_base::in);//읽기모드로 파일을 연다.

	if (fp.is_open())
	{
		//Read a title(annotation) line
		ReadLine(fp);

		string name = "";
		map<string, CharacterEntity*>* mCharacters = nullptr;
		CharacterEntity* character = nullptr;
		float worldPosX = 0.0f;
		float worldPosY = 0.0f;

		while (!fp.eof())
		{
			name = ReadWord(fp);

			if (name == userName)
			{
				//In case the mCharacters empty
				if (mCharacters == nullptr)
					mCharacters = new map<string, CharacterEntity*>;

				character = new CharacterEntity;
				//Name
				character->SetName(name);
				//CurrentMap
				character->SetCurrentMap(static_cast<MAP_NAME>(stoi(ReadWord(fp))));
				//Occupation
				character->SetOccupation(static_cast<Occupation>( stoi(ReadWord(fp)) ));
				//WorldPosX, Y
				worldPosX = stoi(ReadWord(fp));
				worldPosY = stoi(ReadWord(fp));
				character->SetWorldPos(worldPosX, worldPosY);
				//Level
				character->SetLevel(stoi(ReadWord(fp)));
				//Exp
				character->SetExp(stoi(ReadWord(fp)));
				//Str
				character->SetStr(stoi(ReadWord(fp)));
				//Dex
				character->SetDex(stoi(ReadWord(fp)));
				//Int
				character->SetInt(stoi(ReadWord(fp)));
				//Luk;
				character->SetLuk(stoi(ReadWord(fp)));
				//currHP;
				character->SetCurrHP(stoi(ReadWord(fp)));
				//currMP;
				character->SetCurrMP(stoi(ReadWord(fp)));
				//currAP;
				character->SetCurrAP(stoi(ReadWord(fp)));
				
				mCharacters->insert(make_pair(character->GetName(), character));
			}
		}
		fp.close();
		return mCharacters;
	}

	return nullptr;
}

void FileManager::ReadMonsterData(Monster* monster)
{
	FILE* fp = nullptr;
	fopen_s(&fp, vFileNames[static_cast<int>(FILE_NAME::MONSTER)].c_str(), "r");

	while (fp && !feof(fp))
	{
		int sharpCnt = 0;
		char annotation[64] = "";
		while (sharpCnt < 2)
		{
			fscanf_s(fp, "%s", annotation, sizeof(annotation));
			if (string{ annotation } == "#") sharpCnt++;
		}

		char name[128] = "";
		fscanf_s(fp, "%s", name, sizeof(name));
		monster->SetName(name);

		int level;
		fscanf_s(fp, "%d", &level);
		monster->SetLevel(level);

		int exp;
		fscanf_s(fp, "%d", &exp);
		monster->SetExp(exp);

		int money;
		fscanf_s(fp, "%d", &money);
		monster->SetMoney(money);

		int hp;
		fscanf_s(fp, "%d", &hp);
		monster->SetMaxHp(hp);

		float moveSpeed;
		fscanf_s(fp, "%f", &moveSpeed);
		monster->SetMoveSpeed(moveSpeed);

		FPOINT worldPos;
		fscanf_s(fp, "%f %f", &worldPos.x, &worldPos.y);
		monster->SetWorldPos(worldPos);

		int responTime;
		fscanf_s(fp, "%d", &responTime);
		monster->SetResponTime(responTime);

		float damage;
		fscanf_s(fp, "%f", &damage);
		monster->SetDamage(damage);

		float defense;
		fscanf_s(fp, "%f", &defense);
		monster->SetDefense(defense);

		float protection;
		fscanf_s(fp, "%f", &protection);
		monster->SetProtection(protection);

	}
	if (fp)
		fclose(fp);
}

ConsumableItemEntity* FileManager::ReadConsumableItem(CONSUMABLE_ITEMS consumableItem)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::ITEM_CONSUMABLE)].c_str(), ios_base::in);//읽기모드로 파일을 연다.
	if (fp.is_open())
	{
		//Read a title(annotation) line
		ReadLine(fp);
		
		int itemIdx = static_cast<int>(consumableItem);
		ConsumableItemEntity* consumableItemEntity = nullptr;

		for (int i = 0; i < itemIdx; i++)
		{
			ReadLine(fp);//Read the formal items;
		}
		
		if (!fp.eof())
		{
			consumableItemEntity = new ConsumableItemEntity;

			consumableItemEntity->SetItemType(static_cast<CONSUMABLE_ITEMS>(stoi(ReadWord(fp))));
			consumableItemEntity->SetItemName(ReadWord(fp));
			consumableItemEntity->SetExplanation(ReadWord(fp));
			consumableItemEntity->SetPrice(stoi(ReadWord(fp)));
			consumableItemEntity->SetMaxBundleCnt(stoi(ReadWord(fp)));

			consumableItemEntity->SetHpRecovery(stoi(ReadWord(fp)));
			consumableItemEntity->SetMpRecovery(stoi(ReadWord(fp)));
			consumableItemEntity->SetHpRecoveryRate(stof(ReadWord(fp)));
			consumableItemEntity->SetMpRecoveryRate(stof(ReadWord(fp)));

			consumableItemEntity->SetPhysicalDamage(stof(ReadWord(fp)));
			consumableItemEntity->SetMagicalDamage(stof(ReadWord(fp)));
			consumableItemEntity->SetAccuracy(stof(ReadWord(fp)));
			consumableItemEntity->SetDefense(stof(ReadWord(fp)));
			consumableItemEntity->SetMoveSpeed(stof(ReadWord(fp)));

			consumableItemEntity->SetStr(stoi(ReadWord(fp)));
			consumableItemEntity->SetDex(stoi(ReadWord(fp)));
			consumableItemEntity->SetInt(stoi(ReadWord(fp)));
			consumableItemEntity->SetLuk(stoi(ReadWord(fp)));

			consumableItemEntity->SetDuration(stof(ReadWord(fp)));
		}

		fp.close();

		return consumableItemEntity;
	}
}

void FileManager::ReadConsumableItems(map<CONSUMABLE_ITEMS, ConsumableItemEntity*>* mConsumableItemStorage)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::ITEM_CONSUMABLE)].c_str(), ios_base::in);//읽기모드로 파일을 연다.
	if (fp.is_open())
	{
		//Read a title(annotation) line
		ReadLine(fp);

		while (!fp.eof())
		{
			ConsumableItemEntity* consumableItemEntity = new ConsumableItemEntity;

			consumableItemEntity->SetItemType(static_cast<CONSUMABLE_ITEMS>(stoi(ReadWord(fp))));
			consumableItemEntity->SetItemName(ReadWord(fp));
			consumableItemEntity->SetExplanation(ReadWord(fp));
			consumableItemEntity->SetPrice(stoi(ReadWord(fp)));
			consumableItemEntity->SetMaxBundleCnt(stoi(ReadWord(fp)));

			consumableItemEntity->SetHpRecovery(stoi(ReadWord(fp)));
			consumableItemEntity->SetMpRecovery(stoi(ReadWord(fp)));
			consumableItemEntity->SetHpRecoveryRate(stof(ReadWord(fp)));
			consumableItemEntity->SetMpRecoveryRate(stof(ReadWord(fp)));

			consumableItemEntity->SetPhysicalDamage(stof(ReadWord(fp)));
			consumableItemEntity->SetMagicalDamage(stof(ReadWord(fp)));
			consumableItemEntity->SetAccuracy(stof(ReadWord(fp)));
			consumableItemEntity->SetDefense(stof(ReadWord(fp)));
			consumableItemEntity->SetMoveSpeed(stof(ReadWord(fp)));

			consumableItemEntity->SetStr(stoi(ReadWord(fp)));
			consumableItemEntity->SetDex(stoi(ReadWord(fp)));
			consumableItemEntity->SetInt(stoi(ReadWord(fp)));
			consumableItemEntity->SetLuk(stoi(ReadWord(fp)));

			consumableItemEntity->SetDuration(stof(ReadWord(fp)));

			mConsumableItemStorage->insert(make_pair(consumableItemEntity->GetItemType(), consumableItemEntity));
		}
		
		fp.close();
	}
}

void FileManager::ReadSkillData()
{
}

void FileManager::ReadNPCData()
{
}

void FileManager::ReadNPCData(map<string, NPCEntity*>* mNPCStorage, MAP_NAME mapName)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::NPC)].c_str(), ios_base::in);//읽기모드로 파일을 연다.
	if (fp.is_open())
	{
		//Read a title(annotation) line
		ReadLine(fp);

		string npcName = "";
		MAP_NAME npcLocation;
		string word = "";
		string element = "";
		while (!fp.eof())
		{
			npcName = ReadWord(fp);
			npcLocation = static_cast<MAP_NAME>(stoi(ReadWord(fp)));
			if (npcLocation != mapName) {
				ReadLine(fp);//Read the rest till the end of the line
				continue;
			}

			NPCEntity* npcEntity = new NPCEntity;
			npcEntity->SetNPCName(npcName);
			int posX = stoi(ReadWord(fp));
			int posY = stoi(ReadWord(fp));
			npcEntity->SetWorldPos(posX, posY);//posX, posY로 저장되어있음
			npcEntity->SetNPCClicked(false);
			//Read ConsumableItems
			word = ReadWord(fp);
			while ((element = ReadElement(word)) != "")
			{
				ConsumableItemEntity* consumableItemEntity = ReadConsumableItem(static_cast<CONSUMABLE_ITEMS>(stoi(element)));
				npcEntity->push_back_consumable_item(consumableItemEntity);
			}
			//Read Quests
			word = ReadWord(fp);
			while ((element = ReadElement(word)) != "")
			{
				//QuestEntity클래스를 만들어서 위의 ConsumableItems와 같이 구현
			}

			//npcEntity->SetImage() TODO Image 어떻게 받아올지 처리, FILE에 NPC정보 쓰기

			mNPCStorage->insert(make_pair(npcEntity->GetNPCName(), npcEntity) );
		}

		fp.close();
	}
}

InventoryEntity* FileManager::ReadInventoryData(string characterName)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::INVENTORY_CONSUMABLE)].c_str(), ios_base::in);//읽기모드로 파일을 연다.
	if (fp.is_open())
	{
		//Read a title(annotation) line
		ReadLine(fp);

		string name = "";
		InventoryEntity* inventoryEntity = nullptr;
		string word = "";
		string element = "";

		while (!fp.eof())
		{
			name = ReadWord(fp);

			if (characterName != name)
			{
				ReadLine(fp); //해당 라인을 전부 제거
				continue;
			}

			inventoryEntity = new InventoryEntity;
			inventoryEntity->SetCharacterName(name);
			inventoryEntity->SetMoney(stoi(ReadWord(fp)));

			//Read ConsumableItems
			word = ReadWord(fp);
			while ((element = ReadElement(word)) != "")
			{
				string itemNum = "";
				string count = "";
				while (element.size() > 0 && element[0] != '-')
				{
					itemNum.push_back(element[0]);
					element.erase(element.begin());
				}
				if(element[0] == '-')
					element.erase(element.begin());
				while (element.size() > 0)
				{
					count.push_back(element[0]);
					element.erase(element.begin());
				}

				
				ConsumableItemEntity* consumableItemEntity = ReadConsumableItem(static_cast<CONSUMABLE_ITEMS>(stoi(itemNum)));
				inventoryEntity->insert_consumable_item(consumableItemEntity, stoi(count));
			}
			break;
		}
		fp.close();
		return inventoryEntity;
	}
	return nullptr;
}

void FileManager::ReadMapData()
{
}

void FileManager::SaveCharacterData(const map<string, CharacterEntity*>* mCharacters)
{
	fstream fp;
	fp.open(vFileNames[static_cast<int>(FILE_NAME::CHARACTER)].c_str(), ios_base::out);//쓰기모드로 파일을 연다.
	if (fp.is_open())
	{
		//Write the title(annotation) line
		string content = "";
		content.append("[Name CurrentMap Occupation WorldPosX WorldPosY Level EXP STR DEX INT LUK Current_HP Current_MP Current_AP_Point]");
		WriteTitle(fp, content);

		CharacterEntity* character = nullptr;
		auto it = mCharacters->begin();
		while (it != mCharacters->end())
		{
			character = it->second;
			//Name
			WriteWord(fp, character->GetName() );
			//CurrentMap
			WriteWord(fp, to_string(static_cast<int>(character->GetCurrentMap())));
			//Occupation
			WriteWord(fp, to_string(static_cast<int>(character->GetOccupation())) );
			//WorldPos
			WriteWord(fp, to_string((int)character->GetWorldPos().x));
			WriteWord(fp, to_string((int)character->GetWorldPos().y));
			//Level
			WriteWord(fp, to_string(character->GetLevel()));
			//Exp
			WriteWord(fp, to_string(character->GetExp()));
			//Str
			WriteWord(fp, to_string(character->GetStr()));
			//Dex
			WriteWord(fp, to_string(character->GetDex()));
			//Int
			WriteWord(fp, to_string(character->GetInt()));
			//Luk
			WriteWord(fp, to_string(character->GetLuk()));
			//currHP
			WriteWord(fp, to_string(character->GetCurrHP()));
			//currMP
			WriteWord(fp, to_string(character->GetCurrMP()));
			//currAP
			WriteWord(fp, to_string(character->GetCurrAP()), true);
			
			it++;
		}
		fp.close();
	}
}

void FileManager::SaveMonsterData()
{

}

void FileManager::SaveSkillData()
{
}

void FileManager::SaveNPCData()
{
}

void FileManager::SaveMapData()
{
}
