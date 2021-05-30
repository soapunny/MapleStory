#pragma once
#include "config.h"

typedef struct _damageInfo
{
	float displayTime;
	string damage;
	bool isCritical;
}DamageInfo;

class DamageQueue
{
	//TODO 데미지를 stack방식으로 쌓아서 보여주고 사라지게하는 util 클래스 완성
private:
	vector<DamageInfo*> vDamageQueue;//displaySec, damage
	int maxSize;
	int front;
	int rear;
	
	float updateTimer;
	float displayTime;

	bool isMultiDamage;

	COLORREF damageColor;
	COLORREF criticalDamageColor;
	HFONT damageFont;
	HFONT criticalDamageFont;

public:
	DamageQueue(COLORREF damageColor, COLORREF criticalDamageColor, HFONT damageFont, HFONT criticalDamageFont);

	void Update();
	void Render(HDC hdc, FPOINT startPos);
	void Release();

	void push_back(string damage, bool isCritical);
	void pop_front();
	inline int size() { if (rear == -1 && front == -1) return 0; return rear - front + 1; }
	bool empty();
};

