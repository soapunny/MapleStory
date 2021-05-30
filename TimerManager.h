#pragma once
#include "config.h"
#include "Singleton.h"

class Timer;
class TimerManager : public Singleton<TimerManager>
{
private:
	Timer* timer;
	char szText[128];
	HFONT font;

public:
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	float GetElapsedTime();
	float GetCurrTime();
};

