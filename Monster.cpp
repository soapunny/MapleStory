#pragma once
#include "Monster.h"
#include "Managers.h"
#include "DamageQueue.h"
#include "Image.h"


//몬스터 이동
void Monster::MoveMonster()
{
    //플레이어 걷기
    if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
    {
        if (center.x - moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() <= width / 2.0f || blockedState == BLOCKED_STATE::LEFT)
            return;
        center.x -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
    {
        if (center.x + moveSpeed * TimerManager::GetSingleton()->GetElapsedTime() >= CameraManager::GetSingleton()->GetBG()->GetWidth() || blockedState == BLOCKED_STATE::RIGHT)
            return;
        center.x += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
}

void Monster::Release() 
{
    if (damageQueue)
        SAFE_RELEASE(damageQueue);
}