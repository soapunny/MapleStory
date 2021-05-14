#pragma once
#include "config.h"

class Timer
{
private:
	bool isHardware;		// ���� Ÿ�̸Ӹ� �� �� �ִ��� ����
	float timeScale;		// ��� �������� �ʴ� �������� ������ �������� �ð����� ȯ��
	float timeElapsed;		// ������ �ð��� ���� �ð��� �����
	__int64 currTime;		// ���� �ð� ( ���� Ÿ�̸� ������ )
	__int64 lastTime;		// ���� �ð� ( ���� Ÿ�̸� ������ )
	__int64 periodFrequency;// ���� Ÿ�̸� ���ļ� (�ʴ� ������)

	float fpsTimeElapsed;			// ������ ��� �ð� (1�� Ȯ��)
	unsigned long fpsFrameCount;	// �ʴ� ������ ��
	unsigned long FPS;

public:
	HRESULT Init();
	void Tick();
	float GetCurrTime();

	inline unsigned long GetFPS() { return this->FPS; }
	inline float GetElapsedTime() { return this->timeElapsed; }
};

