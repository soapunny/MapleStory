#pragma once
#include "GameNode.h"
#include "Unit.h"

class Image;
class FirstTownScene : public GameNode
{
private:
	Unit* character;
	HBRUSH hBrush;

	Image* map;
	Image* minimapUI;
	Image* minimap;

	virtual void RenderMiniMap(HDC hdc);
public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	virtual ~FirstTownScene() {}
};

