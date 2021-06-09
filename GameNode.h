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
	inline virtual HRESULT Init() { return S_OK; }	// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	inline virtual void Release() {}			// �޸� ����
	inline virtual void Update() {}			// ������ ������ ���� ���� ���� (������ ����)
	inline virtual void Render(HDC hdc) {}	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	inline virtual string InstanceOf() { return string{ typeid(this).name() }; }
	inline GameNode() {};
	inline virtual ~GameNode() {};
};

