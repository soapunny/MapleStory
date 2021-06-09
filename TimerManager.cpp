#pragma once
#include "Managers.h"
#include "Timer.h"

HRESULT TimerManager::Init()
{
    timer = new Timer();
    timer->Init();

    font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));//높이 50픽셀의

    return S_OK;
}

void TimerManager::Release()
{
    DeleteObject(font);
    SAFE_DELETE(timer);
}

void TimerManager::Update()
{
    if (timer)
    {
        timer->Tick();
    }
}

void TimerManager::Render(HDC hdc)
{
    SetBkMode(hdc, 0);//폰트 배경 제거
    return;
    if (timer)
    {
        SetBkMode(hdc, 0);//폰트 배경 제거
        SetTextColor(hdc, RGB(0, 0, 0)); //폰트 색상 흰색 지정
        HFONT oldFont;
        
        oldFont = (HFONT)SelectObject(hdc, font);
        wsprintf(szText, "FPS : %d", timer->GetFPS());
        TextOut(hdc, WINSIZE_X - 100, 20, szText, strlen(szText));
    }
}

float TimerManager::GetElapsedTime()
{
    return timer->GetElapsedTime();
}

float TimerManager::GetCurrTime()
{
    return timer->GetCurrTime();
}
