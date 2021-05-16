#pragma once
#include "GameNode.h"

enum class MOVE_DIRECTION { MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, END_OF_MOVE_DIRECTION };
enum class UNIT_STATE{DEFAULT_STATE, WALKING_STATE, ATTACK_STATE, JUMPING_STATE, JUMPING_ATTACK_STATE, LYING_STATE, LYING_ATTACK_STATE, HANGING_STATE, HANGING_MOVE_STATE, END_OF_UNIT_STATE};
enum class JUMPING_STATE{JUMPING_UP, JUMPING_DOWN, JUST_LANDED, END_OF_JUMPING_STATE};
enum class BLOCKED_STATE{LEFT, RIGHT, TOP, BOTTOM, END_OF_BLOCKED_STATE};

class Image;
class SkillManager;
class Unit : public GameNode
{
protected:
	Image* image;

	string name;
	float moveSpeed;
	MOVE_DIRECTION moveDirection;
	UNIT_STATE state;
	JUMPING_STATE jumpingState;
	FPOINT center;
	float width;
	float height;
	POINT frame;
	RECT shape;
	float velocity;

	SkillManager* skillManager;

	int damage;
	bool isJumpingDown;

	BLOCKED_STATE blockedState;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void SetName(string name) { this->name = name; }
	virtual void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	virtual void SetMoveDirection(MOVE_DIRECTION moveDirection) { this->moveDirection = moveDirection; }
	virtual void SetCenter(FPOINT center) { this->center = center; }
	virtual void SetCenterX(float x) { this->center.x = x; }
	virtual void SetCenterY(float y) { this->center.y = y; }
	virtual void SetDamage(int damage) { this->damage = damage; }
	virtual void SetShape(RECT shape) { this->shape = shape; }
	virtual void SetJumpingState(JUMPING_STATE jumpingState) { this->jumpingState = jumpingState; }
	virtual void SetBlockedState(BLOCKED_STATE blockedState) { this->blockedState = blockedState; }
	virtual void SetState(UNIT_STATE state) { this->state = state; }

	virtual RECT GetShape() { return shape; }
	virtual FPOINT GetCenter() { return center; }
	virtual float GetWidth() { return width; }
	virtual float GetHeight() { return height; }
	virtual bool GetJumpingDown() { return isJumpingDown; }
	virtual JUMPING_STATE GetJumpingState() { return jumpingState; }
	virtual BLOCKED_STATE GetBlockedState() { return blockedState; }
	virtual UNIT_STATE GetUnitState() { return state; }

	virtual ~Unit() {};
};

