#pragma once
#include "Unit.h"
enum class CHARACTER_FRAME_Y {
	DEFAULT, LEFT_WALK, RIGHT_WALK,
	LEFT_ATTACK_1, LEFT_ATTACK_2, LEFT_ATTACK_3,
	RIGHT_ATTACK_1, RIGHT_ATTACK_2, RIGHT_ATTACK_3,
	JUMP, LYING, HANGING_1, HANGING_2
};

class Image;
class Character:public Unit
{
private:
	float exp;
	int level;

	int str;
	int dex;
	int intel;
	int luck;

	float jumpTimer;
	float attackTimer;
	float priorPosY;

public:
	virtual ~Character() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Release();

	virtual void SetExp(float exp) { this->exp = exp; }
	virtual void SetLevel(int level) { this->level = level; }
	virtual void SetStr(int str) { this->str = str; }
	virtual void SetDex(int dex) { this->dex = dex; }
	virtual void SetIntel(int intel) { this->intel = intel; }
	virtual void SetLuck(int luck) { this->luck = luck; }

	JUMPING_STATE CheckPixelCollision();

	void HandleDefaultState();
	void HandleWalkingState();
	void HandleAttackState();
	void HandleJumpingState();
	void HandleJumpingAttackState();
	void HandleLyingState();
	void HandleLyingAttackState();
	void HandleHangingState();

	void MoveCharacter();

	void ShowAnimation(UNIT_STATE unitState);
	CHARACTER_FRAME_Y UnitStateToCharacterFrameY(UNIT_STATE unitState);
};

