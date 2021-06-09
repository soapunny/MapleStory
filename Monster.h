#pragma once
#include "Unit.h"

class Monster : public Unit
{
protected:
	//Name Level EXP MONEY HP MOVESPEED CENTER_X CENTER_Y RESPON
	Unit* player;

	int level;
	int exp;
	int money;
	int responTime;

	float animationTimer;

public:
	virtual void MoveMonster();
	virtual void Release();

	virtual void SetLevel(int level) { this->level = level; }
	virtual void SetExp(int exp) { this->exp = exp; }
	virtual void SetMoney(int money) { this->money = money; }
	virtual void SetResponTime(int responTime) { this->responTime = responTime; }
	virtual void SetTarget(Unit* player) { this->player = player; }

	virtual int GetEXP() { return exp; }
	virtual int GetMoeny() { return money; }
};

