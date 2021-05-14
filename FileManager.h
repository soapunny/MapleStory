#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
using namespace std;

class Character;
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
	void ReadMonsterData();
	void ReadSkillData();
	void ReadNPCData();
	void ReadMapData();

	void SaveCharacterData();
	void SaveMonsterData();
	void SaveSkillData();
	void SaveNPCData();
	void SaveMapData();
};

