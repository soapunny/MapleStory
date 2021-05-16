#pragma once
#include "GameNode.h"

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

	float skillDelay;
	float skillRange;
	bool needShuriken;

	Image* image1;
	Image* image2;

	RECT shape;
	POINT frame;
	POINT weaponFrame;

	FPOINT ownerPos;

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	bool GetFired() { return isFired; }
	float GetSkillDelay() { return skillDelay; }

	void FireSkill(RECT ownerShape);
};

