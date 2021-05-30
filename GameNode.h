#pragma once
#include "config.h"
#include <typeinfo>
// ��� ����, ��� �Լ� ���� ���� ����

/*
	������ ������� ���ӱ����� ���� �⺻�� �Ǵ� Ŭ����
*/
class GameNode
{
protected:
	float timer;
public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	inline virtual string InstanceOf() { return string{ typeid(this).name() }; }
	GameNode();
	virtual ~GameNode();
};

