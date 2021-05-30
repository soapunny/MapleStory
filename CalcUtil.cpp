#include "CalcUtil.h"

pair<bool, int> CalcUtil::CalcDamage(float myDefense, float myProtection, float yourDamage, float yourCriticalPercentage, float yourCriticalDamageRate)
{
	int damage;
	bool isCritical = (rand() % 100 + 1) <= (yourCriticalPercentage * 100);

	if (isCritical)
	{
		damage = (yourDamage - myDefense) * yourCriticalDamageRate;
	}
	else
	{
		damage = (int)(yourDamage * (1.0f - (myProtection / 100.0f)) - myDefense);
	}

	return make_pair(isCritical, damage);
}

int CalcUtil::GetNumberLength(int number)
{
	int result = 0;
	while (number != 0)
	{
		number /= 10;
		result++;
	}

	return result;
}

string CalcUtil::TurnIntegerIntoStr(int number)
{
	string numberStr = "";
	int length = GetNumberLength(number);
	if (number <= 0)
	{
		return "MISS";
	}
	for (int i = 0; i < length; i++)
	{
		numberStr.insert(numberStr.end(), (char)((number / (int)pow(10, length - i - 1)) % 10 + '0'));
	}

	return numberStr;
}

int CalcUtil::AbilityToDamage(int mainStat, int subStat)
{
	return mainStat * 4 + subStat;
}