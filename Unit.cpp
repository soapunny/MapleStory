#pragma once
#include "Unit.h"
#include "Managers.h"
#include "DamageQueue.h"
#include "CalcUtil.h"
#include "Image.h"

void Unit::InitHitTimer(bool isHit, float hitTimer, float maxHitTime)
{
    this->isHit = isHit;
    this->hitTimer = hitTimer;
    this->maxHitTime = maxHitTime;
}

void Unit::CountHitTimer()
{
    if(isHit)
    { 
        hitTimer += TimerManager::GetSingleton()->GetElapsedTime();
        if (hitTimer >= maxHitTime)
        {
            InitHitTimer(false, 0.0f, maxHitTime);
        }
    }
}

HRESULT Unit::Init()
{
    return S_OK;
}

void Unit::Release()
{
}

void Unit::Update()
{
}

void Unit::Render(HDC hdc)
{
    if (isHit && (int)(hitTimer*10.0f) % 2 == 0)
    {
        //image->AlphaFrameRender(hdc, shape.left, shape.top, frame.x, frame.y, false);
    }
    else
    {
        image->FrameRender(hdc, shape.left, shape.top, frame.x, frame.y, false);
    }
}

void Unit::Render(HDC hdc, int pasteX, int pasteY)
{
    if (state != UNIT_STATE::END_OF_UNIT_STATE)
    {
        image->FrameRender(hdc, pasteX, pasteY, frame.x, frame.y, true);
        if (isAttack)
        {
            attackImage->FrameRender(hdc, pasteX - 20, pasteY + GetCenterToBottom() - 20, attackFrame.x, attackFrame.y, true);
        }
    }
}

void Unit::Move()
{
    //플레이어 걷기
    if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
    {
        if (worldPos.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() <= GetCenterToLeft() || blockedState == BLOCKED_STATE::LEFT)
            return;
        worldPos.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
    {
        if (worldPos.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() >= CameraManager::GetSingleton()->GetBG()->GetWidth() - GetCenterToRight() || blockedState == BLOCKED_STATE::RIGHT)
            return;
        worldPos.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_UP)
    {
        worldPos.y -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_DOWN)
    {
        worldPos.y += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
}

void Unit::InitHpRecovery(int hpRecovery, bool isHpRecovered, float hpRecoveryTimer, float hpRecoveryTime, float hpRecoveryDuration, COLORREF hpRecoveryColor)
{
    recoveryInfo->hpRecovery = hpRecovery;
    recoveryInfo->isHpRecovered = isHpRecovered;
    recoveryInfo->hpRecoveryTimer = hpRecoveryTimer;
    recoveryInfo->hpRecoveryTime = hpRecoveryTime;
    recoveryInfo->hpRecoveryDuration = hpRecoveryDuration;
    recoveryInfo->hpRecoveryColor = hpRecoveryColor;
}

void Unit::InitMpRecovery(int mpRecovery, bool isMpRecovered, float mpRecoveryTimer, float mpRecoveryTime)
{
    recoveryInfo->mpRecovery = mpRecovery;
    recoveryInfo->isMpRecovered = isMpRecovered;
    recoveryInfo->mpRecoveryTimer = mpRecoveryTimer;
    recoveryInfo->mpRecoveryTime = mpRecoveryTime;
}

void Unit::CountRecovery()
{
    if ((state != UNIT_STATE::DEFAULT_STATE && state != UNIT_STATE::HANGING_STATE) || isHit)
    {
        InitHpRecovery(recoveryInfo->hpRecovery, false, 0.0f, recoveryInfo->hpRecoveryTime, recoveryInfo->hpRecoveryDuration, recoveryInfo->hpRecoveryColor);
        InitMpRecovery(recoveryInfo->mpRecovery, false, 0.0f, recoveryInfo->mpRecoveryTime);
        return;
    }

    recoveryInfo->isHpRecovered = false;
    recoveryInfo->isMpRecovered = false;
    recoveryInfo->hpRecoveryTimer += TimerManager::GetSingleton()->GetElapsedTime();
    recoveryInfo->mpRecoveryTimer += TimerManager::GetSingleton()->GetElapsedTime();

    if (recoveryInfo->hpRecoveryTimer >= recoveryInfo->hpRecoveryTime)
    {
        //HP Recovery
        if(hp < maxHp)
        { 
            if (maxHp - hp < recoveryInfo->hpRecovery)
                SetHP(maxHp);
            else
                SetHP(hp + recoveryInfo->hpRecovery);
            recoveryQueue->push_back(CalcUtil::GetSingleton()->TurnIntegerIntoStr(recoveryInfo->hpRecovery), false);
            InitHpRecovery(recoveryInfo->hpRecovery, true, 0.0f, recoveryInfo->hpRecoveryTime, recoveryInfo->hpRecoveryDuration, recoveryInfo->hpRecoveryColor);
        }

    }

    if (recoveryInfo->mpRecoveryTimer >= recoveryInfo->mpRecoveryTime)
    {
        //MP Recovery
        if (mp < maxMp)
        {
            if (maxMp - mp < recoveryInfo->mpRecovery)
                SetMP(maxMp);
            else
                SetMP(mp + recoveryInfo->mpRecovery);
            InitMpRecovery(recoveryInfo->mpRecovery, true, 0.0f, recoveryInfo->mpRecoveryTime);
        }
    }
}
