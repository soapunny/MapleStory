#pragma once
#include "GameNode.h"

class Image;
class Unit;
class Shuriken :public GameNode
{
private:
	Image* image;
	FPOINT pos;
	float moveSpeed;
	POINT frame;

	Unit* target;
	float angle;
	float range;

	float width;
	float height;

	bool isFired;

public:
	virtual HRESULT Init();		
	virtual void Release();		
	virtual void Update();		
	virtual void Render(HDC hdc);
	~Shuriken() {}

	inline virtual void SetTartget(Unit* target) { this->target = target; }
	inline virtual void SetPos(FPOINT pos) { this->pos = pos; }
	inline virtual void SetFired(bool isFired) { this->isFired = isFired; }
	inline virtual void SetAngle(float angle) { this->angle = angle; }
	inline virtual void SetRange(float range) { this->range = range; }

	inline virtual float GetWidth() { return width; }
	inline virtual float GetHeight() { return height; }
	inline virtual FPOINT GetPos() { return pos; }
	inline virtual bool GetFired() { return isFired; }
};

