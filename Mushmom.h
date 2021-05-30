#pragma once
#include "Monster.h"

class Image;
class Mushmom: public Monster
{
private:
	float jumpTimer;
	float attackTimer;
	float priorPosY;

	bool isReadyToAttack;

	void HandleJumpProcess();
	void HandleEarthQuakeAttack();
	void ShowAnimation(UNIT_STATE unitState);
	void HandleDefaultState();
	void HandleWalkingState();
	void HandleAttackState();
	void HandleJumpingState();
	void HandleHitState();
	void HandleDeadState();

public:
	virtual HRESULT Init();
	virtual void Update();


	inline virtual float GetCenterToLeft() { return width / 2.0f - 60.0f; };
	inline virtual float GetCenterToRight() { return width / 2.0f - 80.0f; };
	inline virtual float GetCenterToTop() { return height / 2.0f - 80.0f; };
	inline virtual float GetCenterToBottom() { return height / 2.0f - 10.0f; };
};

