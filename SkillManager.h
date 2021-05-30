#pragma once
#include "GameNode.h"

enum class SKILL_TYPE{DEFAULT_SHURIKEN_ATTACK, FLASH_JUMP, LUCKY_SEVEN, SHADE_SPLIT, SHURIKEN_BURST, SUDDEN_RAID, WIND_TALISMAN, END_OF_SKILL_TYPE};

class Skill;
class Unit;
enum class MOVE_DIRECTION;
class SkillManager:public GameNode
{
private:
	map<SKILL_TYPE, Skill*> mSkillData;
	SKILL_TYPE currentSkillType;
	Skill* currentSkill;
	float currentSkillTimer;

	bool isOn;

public:
	virtual HRESULT Init();			
	virtual void Release();			
	virtual void Update();			
	virtual void Render(HDC hdc);

	bool FireSkill(SKILL_TYPE skillType, Unit* owner, MOVE_DIRECTION direction);

	inline SKILL_TYPE GetCurrentSkillType() { return currentSkillType; }
	inline Skill* GetCurrentSkill() { return currentSkill; }
	inline bool GetOn() { return isOn; }

	virtual ~SkillManager() {}
};

