#pragma once
#include "Singleton.h"

#define MAX_KEY_COUNT	256	

using namespace std;

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<MAX_KEY_COUNT>	keyUp;		// ���� Ű�� ���� �ö�� �ִ���
	bitset<MAX_KEY_COUNT>	keyDown;	// ���� Ű�� ���� �ִ���

public:
	HRESULT Init();
	void Release();

	bool IsOnceKeyDown(int key);	// Ű�� ó�� ���ȴ��� �Ǵ�
	bool IsOnceKeyUp(int key);		// Ű�� ���ȴٰ� ó�� ���������� �Ǵ�
	bool IsStayKeyDown(int key);	// Ű�� ��� �����ִ��� �Ǵ�
	bool IsKeyUp(int key);			// Ű�� ������ �ִ� ����

	void SetKeyDown(int key, bool state) { this->keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { this->keyUp.set(key, state); }
};

