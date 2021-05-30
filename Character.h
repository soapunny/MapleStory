#pragma once
#include "Unit.h"
enum class CHARACTER_FRAME_Y {
	DEFAULT, LEFT_WALK, RIGHT_WALK,
	LEFT_ATTACK_1, LEFT_ATTACK_2, LEFT_ATTACK_3,
	RIGHT_ATTACK_1, RIGHT_ATTACK_2, RIGHT_ATTACK_3,
	JUMP, LYING, HANGING_1, HANGING_2, DEATH
};

class Image;
enum class SKILL_TYPE;
class Character:public Unit
{
private:
	float exp;

	int str;
	int dex;
	int intel;
	int luck;

	float jumpTimer;
	FPOINT priorPos;

	Image* deathImage;
	POINT deathFrame;
	FPOINT tombstonePos;
	float deathTimer;

	HFONT font;

	void HandleDefaultState();
	void HandleWalkingState();
	void HandleAttackState(SKILL_TYPE skillType);
	void HandleJumpingState();
	void HandleJumpingAttackState(SKILL_TYPE skillType);
	void HandleLyingState();
	void HandleLyingAttackState();
	void HandleHangingState();
	void HandleHangingMoveState();
	void HandleSkillState();
	void HandleDeadState();

	void ShowAnimation(UNIT_STATE unitState);
	void DropTheTombstone();
	CHARACTER_FRAME_Y UnitStateToCharacterFrameY(UNIT_STATE unitState);

public:
	virtual ~Character() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Release(); 
	virtual void Render(HDC hdc);

	virtual void SetExp(float exp) { this->exp = exp; }
	virtual void SetStr(int str) { this->str = str; }
	virtual void SetDex(int dex) { this->dex = dex; }
	virtual void SetIntel(int intel) { this->intel = intel; }
	virtual void SetLuck(int luck) { this->luck = luck; }


	inline virtual float GetCenterToLeft() { return width / 2.0f - 15.0f; };
	inline virtual float GetCenterToRight() { return width / 2.0 - 15.0f; };
	inline virtual float GetCenterToTop() { return height / 2.0f - 10.0f; };
	inline virtual float GetCenterToBottom() { return height / 2.0f; };

};

