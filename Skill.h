#pragma once
#include "GameNode.h"

class Image;
class Shuriken;
class Unit;
enum class MOVE_DIRECTION;
class Skill:public GameNode
{
private:
	bool isFired;

	string name;
	int damage;
	float damageRatio;
	int hpRecovery;
	int mpRecovery;
	int hpConsumption;
	int mpConsumption;

	int attackCount;
	float skillDelay;
	float skillRange;
	bool needShuriken;
	int shurikenCnt;

	MOVE_DIRECTION direction;

	vector<Shuriken*> vShurikens;

	float skillTimer;

	Image* image;

	RECT shape;
	POINT frame;

	FPOINT ownerPos;

public:
	inline virtual ~Skill() {}

	virtual HRESULT Init(string skillName);			
	virtual void Release();			
	virtual void Update();			
	virtual void Render(HDC hdc);	

	virtual inline bool GetFired() { return isFired; }
	virtual inline float GetSkillDelay() { return skillDelay; }
	virtual inline vector<Shuriken*> GetVShurikens() { return vShurikens; }
	virtual inline bool GetNeedShuriken() { return needShuriken; }
	virtual inline float GetDamageRatio() { return damageRatio; }
	virtual inline float GetMpConsumption() { return mpConsumption; }

	void FireSkill(Unit* ownerShape, MOVE_DIRECTION direction);
	
};

