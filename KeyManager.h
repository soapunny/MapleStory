#pragma once
#include "Singleton.h"

#define MAX_KEY_COUNT	256	

using namespace std;

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<MAX_KEY_COUNT>	keyUp;		// 현재 키가 위로 올라와 있는지
	bitset<MAX_KEY_COUNT>	keyDown;	// 현재 키가 눌려 있는지

public:
	HRESULT Init();
	void Release();

	bool IsOnceKeyDown(int key);	// 키가 처음 눌렸는지 판단
	bool IsOnceKeyUp(int key);		// 키가 눌렸다가 처음 떼어졌는지 판단
	bool IsStayKeyDown(int key);	// 키가 계속 눌려있는지 판단
	bool IsKeyUp(int key);			// 키가 떼어져 있는 상태

	void SetKeyDown(int key, bool state) { this->keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { this->keyUp.set(key, state); }
};

