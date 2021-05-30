#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
using namespace std;

enum class FILE_NAME{CHARACTER, MONSTER, SKILL, NPC, MAP, END_OF_FILE_NAME};

class Character;
class Monster;
class FileManager : public Singleton<FileManager>
{
private:
	/*unordered_map<string, string> mCharacterData;
	unordered_map<string, string> mMonsterData;
	unordered_map<string, string> mSkillData;
	unordered_map<string, string> mNPCData;
	unordered_map<string, string> mMapData;*/
	vector<string> vFileNames;

public:
	void Init();

	void ReadCharacterData(Character* character);
	void ReadMonsterData(Monster* monster);
	void ReadSkillData();
	void ReadNPCData();
	void ReadMapData();

	void SaveCharacterData();
	void SaveMonsterData();
	void SaveSkillData();
	void SaveNPCData();
	void SaveMapData();
};

