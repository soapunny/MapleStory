#pragma once
#include "GameNode.h"

class Unit;
class MonsterManager : public GameNode
{
private:
	vector<Unit*> vMonsters;

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	vector<Unit*>* GetVMonsters() { return &vMonsters; }


	~MonsterManager() {}
};

