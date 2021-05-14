#include "FileManager.h"
#include "Character.h"

#define FILE_COUNT	5
void FileManager::Init()
{
	vFileNames.resize(FILE_COUNT);
	
	vFileNames[0] = "Save/characterData.map";
	vFileNames[1] = "Save/monsterData.map";
	vFileNames[2] = "Save/skillData.map";
	vFileNames[3] = "Save/npcData.map";
	vFileNames[4] = "Save/mapData.map";
}

void FileManager::ReadCharacterData(Character* character)
{
	FILE* fp = nullptr;
	fopen_s(&fp, vFileNames[0].c_str(), "r");

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

void FileManager::ReadMonsterData()
{

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
