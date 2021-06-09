#pragma once
#include "config.h"
#include "Singleton.h"
using namespace std;

class CalcUtil : public Singleton<CalcUtil>
{
public:
	pair<bool, int> CalcDamage(float myDefense, float myProtection, float yourDamage, float yourCriticalPercentage, float yourCriticalDamageRate);
	float CalcCritical(int mainStat, int subStat);
	int GetNumberLength(int number);
	string TurnIntegerIntoStr(int number);
	int AbilityToDamage(int mainStat, int subStat);
	float GetDistance(FPOINT posA, FPOINT posB);
	float GetDistance(float differenceX, float differenceY);
	string AddComma(int number);

	inline int CalcHP(int level) { return level * 50; }
	inline int CalcMP(int level) { return level * 50; }
	inline float CalcBasicDefense(int mainStat, int subStat){ return mainStat * 4.0f + subStat;}
	inline float CalcBasicProtection(int level){return level / 4.0f;}

	inline void Release() { ReleaseSingleton(); }
};