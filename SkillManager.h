#pragma once
#include "GameNode.h"

class Skill;
class SkillManager:public GameNode
{
private:
	map<string, Skill*> mSkillData;

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	void FireSkill(string skillName, RECT playerShape);
	
	float GetSkillDelay(string skillName);

	virtual ~SkillManager() {}
};

