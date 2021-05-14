#pragma once
#include "GameNode.h"

enum class MOVE_DIRECTION { MOVE_LEFT, MOVE_RIGHT, END_OF_MOVE_DIRECTION };
enum class UNIT_STATE{DEFAULT_STATE, WALKING_STATE, ATTACK_STATE, JUMPING_STATE, JUMPING_ATTACK_STATE, LYING_STATE, LYING_ATTACK_STATE, HANGING_STATE, HANGING_MOVE_STATE, END_OF_UNIT_STATE};
enum class JUMPING_STATE{JUMPING_UP, JUMPING_DOWN, JUST_LANDED, END_OF_JUMPING_STATE};

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

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual void SetName(string name) { this->name = name; }
	virtual void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	virtual void SetMoveDirection(MOVE_DIRECTION moveDirection) { this->moveDirection = moveDirection; }
	virtual void SetCenter(FPOINT center) { this->center = center; }
	virtual void SetDamage(int damage) { this->damage = damage; }
	virtual void SetShape(RECT shape) { this->shape = shape; }

	virtual RECT GetShape() { return shape; }
	virtual FPOINT GetCenter() { return center; }
	virtual float GetWidth() { return width; }
	virtual float GetHeight() { return height; }

	virtual ~Unit() {};
};

