#pragma once
#include "InGameScene.h"
#include "Unit.h"


class MushroomFieldScene : public InGameScene
{
private:
	virtual void MoveSceneUsingPortal();

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	virtual ~MushroomFieldScene() {}
};

