#pragma once
#include "Character.h"
#include "Managers.h"
#include "Monster.h"

void FileManager::Init()
{
	vFileNames.resize(static_cast<int>(FILE_NAME::END_OF_FILE_NAME));
	
	vFileNames[static_cast<int>(FILE_NAME::CHARACTER)] = "Save/characterData.map";
	vFileNames[static_cast<int>(FILE_NAME::MONSTER)] = "Save/monsterData.ini";
	vFileNames[static_cast<int>(FILE_NAME::SKILL)] = "Save/skillData.map";
	vFileNames[static_cast<int>(FILE_NAME::NPC)] = "Save/npcData.map";
	vFileNames[static_cast<int>(FILE_NAME::MAP)] = "Save/mapData.map";
}

void FileManager::ReadCharacterData(Character* character)
{
	FILE* fp = nullptr;
	fopen_s(&fp, vFileNames[static_cast<int>(FILE_NAME::CHARACTER)].c_str(), "r");

	while (!feof(fp))
	{
		int sharpCnt = 0;
		char annotation[64] = "";
		while(sharpCnt < 2)
		{ 
			fscanf_s(fp, "%s", annotation, sizeof(annotation));
			if (string{annotation} == "#") sharpCnt++;
		}

		char name[128] = "";
		fscanf_s(fp, "%s", name, sizeof(name));
		character->SetName(name);

		int level;
		fscanf_s(fp, "%d", &level);
		character->SetLevel(level);

		float exp;
		fscanf_s(fp, "%f", &exp);
		character->SetExp(exp);

		int str;
		fscanf_s(fp, "%d", &str);
		character->SetStr(str);

		int dex;
		fscanf_s(fp, "%d", &dex);
		character->SetDex(dex);

		int intel;
		fscanf_s(fp, "%d", &intel);
		character->SetIntel(intel);

		int luck;
		fscanf_s(fp, "%d", &luck);
		character->SetLuck(luck);

		float moveSpeed;
		fscanf_s(fp, "%f", &moveSpeed);
		character->SetMoveSpeed(moveSpeed);

		FPOINT center;
		fscanf_s(fp, "%f %f", &center.x, &center.y);
		character->SetCenter(center);
	}
}

void FileManager::ReadMonsterData(Monster* monster)
{
	FILE* fp = nullptr;
	fopen_s(&fp, vFileNames[static_cast<int>(FILE_NAME::MONSTER)].c_str(), "r");

	while (!feof(fp))
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
		fscanf_s(fp, "%f", &exp);
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

		FPOINT center;
		fscanf_s(fp, "%f %f", &center.x, &center.y);
		monster->SetCenter(center);

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
}

void FileManager::ReadSkillData()
{
}

void FileManager::ReadNPCData()
{
}

void FileManager::ReadMapData()
{
}

void FileManager::SaveCharacterData()
{

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
