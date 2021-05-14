#pragma once
#include "GameNode.h"
#include "Unit.h"

class Image;
class BattleFieldScene : public GameNode
{
private:
	Unit* character;
	HBRUSH hBrush;

public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	virtual ~BattleFieldScene() {}
};

