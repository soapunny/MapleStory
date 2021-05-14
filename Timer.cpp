#include "Timer.h"

HRESULT Timer::Init()
{
    isHardware = false;
    timeScale = 0.0f;
    timeElapsed = 0.0f;
    currTime = 0.0;
    lastTime = 0.0;
    periodFrequency = 0.0;
    fpsTimeElapsed = 0.0f;
    fpsFrameCount = 0;

    if (QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency))
    {
        isHardware = true;
        QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
        timeScale = 1.0f / periodFrequency;
    }
    else
    {
        isHardware = false;
        lastTime = timeGetTime();
        timeScale = 0.001f;
    }

    return S_OK;
}

void Timer::Tick()
{
    // ���� ������ or ���� �ð��� ���ؼ� ��� �ð��� ���
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    }
    else
    {
        currTime = timeGetTime();
    }

    timeElapsed = (currTime - lastTime) * timeScale;

    // ������ ���
    fpsTimeElapsed += timeElapsed;
    fpsFrameCount++;

    if (fpsTimeElapsed >= 1.0f)
    {
        FPS = fpsFrameCount;
        fpsFrameCount = 0.0f;
        fpsTimeElapsed -= 1.0f;
    }

    lastTime = currTime;
}

float Timer::GetCurrTime()
{
    __int64 time;
    if (isHardware)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&time);
    }
    else
    {
        time = timeGetTime();
    }


    return time / timeScale;
}
