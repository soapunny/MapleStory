#pragma once
#include <string>
#include "Singleton.h"
using namespace std;

class CalcUtil : public Singleton<CalcUtil>
{
public:
	pair<bool, int> CalcDamage(float myDefense, float myProtection, float yourDamage, float yourCriticalPercentage, float yourCriticalDamageRate);
	int GetNumberLength(int number);
	string TurnIntegerIntoStr(int number);
	int AbilityToDamage(int mainStat, int subStat);

	inline float CalcBasicDefense(int mainStat, int subStat){ return mainStat * 4.0f + subStat;}
	inline float CalcBasicProtection(int level){return level / 4.0f;}

	inline void Release() { ReleaseSingleton(); }
};