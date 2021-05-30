#pragma once
#include "GameNode.h"

class Unit;
class NPCManager;
class MonsterManager;
class InGameScene : public GameNode
{
protected:
	Unit* character;
	NPCManager* npcManager;
	MonsterManager* monsterManager;
	vector<POINT*>* vPortalLoc;

	virtual void MoveSceneUsingPortal();

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	virtual ~InGameScene() {}
};

