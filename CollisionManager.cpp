#pragma once
#include "Managers.h"
#include "Character.h"
#include "Monster.h"
#include "SkillManager.h"
#include "Skill.h"
#include "Shuriken.h"
#include "DamageQueue.h"
#include "CalcUtil.h"
#include "Image.h"

void CollisionManager::RegisterPlayer(Unit* player)
{
	this->player = player;
}

void CollisionManager::RegisterVEnemyUnits(vector<Unit*>* vEnemyUnits)
{
	this->vEnemyUnits = vEnemyUnits;
}

HRESULT CollisionManager::Init()
{
    hitDamageImg = ImageManager::GetSingleton()->AddImage("hitDamage", "Image/Number/numbers.bmp", 212, 51, 10, 1, true, RGB(255, 255, 255), true);
    //attackDamageImg = ImageManager::GetSingleton()->FindImage("attackDamage");

    isHitDamageReady = false;
    hitDamageTimer = 0.0f;
	return S_OK;
}

void CollisionManager::Update()
{
    //Player Pixel Collision
    CheckPixelCollisionFor(player, true, true);//unit, check rope near

    //Enemy Pixel Collision
    if(vEnemyUnits)
    { 
        for (Unit* monster : *vEnemyUnits)
            CheckPixelCollisionFor(monster, false, false);

        //player vs enenmies collision
        CheckPlayerEnemiesCollision();
        //player's protile vs enemies collision
        CheckPlayerProtileEnemiesCollision();
    }
}

void CollisionManager::Render(HDC hdc)
{

}

void CollisionManager::Release()
{

}

void CollisionManager::CheckPixelCollision(Unit* unit, CHECK_DIRECTION checkDirection, float r)
{
    switch (checkDirection)
    {
    case CHECK_DIRECTION::LEFT:
        hasLeftCollided = CheckLeftPixelCollision(unit, r);
        break;
    case CHECK_DIRECTION::RIGHT:
        hasRightCollided = CheckRightPixelCollision(unit, r);
        break;
    case CHECK_DIRECTION::TOP:
        hasTopCollided = CheckTopPixelCollision(unit, r);
        break;
    case CHECK_DIRECTION::BOTTOM:
        hasBottomCollided = CheckBottomPixelCollision(unit, r);
        break;
    case CHECK_DIRECTION::ROPE:
        hasRopeCollided = CheckRopePixelCollision(unit);
        break;
    case CHECK_DIRECTION::PORTAL:
        hasPortalCollided = CheckPortalPixelCollision(unit);
        break;
    default:
        return;
    }
}

bool CollisionManager::CheckLeftPixelCollision(Unit* unit, float r)
{
    if (unit->GetJumpingState() == JUMPING_STATE::JUMPING_UP)
        return false;
    int R = 0;
    int G = 0;
    int B = 0;
    float currPosX = unit->GetCenter().x - (unit->GetCenterToLeft());
    float currPosY = unit->GetCenter().y + unit->GetCenterToBottom() / 2.0f;

    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        for(int j = currPosY - 1; j <= currPosY + 1; j++)
        {
            COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, j);

            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if (CheckBlock(R, G, B) )
            {
                //위치 고정
                //unit->SetCenterX(i + (unit->GetWidth() / 2.0f));
                player->SetBlockedState(BLOCKED_STATE::LEFT);

                return true;
            }
        }
    }
    if (player->GetBlockedState() == BLOCKED_STATE::LEFT)
    {
        player->SetBlockedState(BLOCKED_STATE::END_OF_BLOCKED_STATE);
    }
    return false;
}

bool CollisionManager::CheckRightPixelCollision(Unit* unit, float r)
{
    if (unit->GetJumpingState() == JUMPING_STATE::JUMPING_UP)
        return false;
    int R = 0;
    int G = 0;
    int B = 0;
    float currPosX = unit->GetCenter().x + (unit->GetCenterToRight());

    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, unit->GetCenter().y + unit->GetCenterToBottom() / 2.0f);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckBlock(R, G, B))
        {
            //위치 고정
            //unit->SetCenterX(i - (unit->GetWidth() / 2.0f));
            player->SetBlockedState(BLOCKED_STATE::RIGHT);

            return true;
        }
    }
    if (player->GetBlockedState() == BLOCKED_STATE::RIGHT)
    {
        player->SetBlockedState(BLOCKED_STATE::END_OF_BLOCKED_STATE);
    }
    return false;
}

bool CollisionManager::CheckTopPixelCollision(Unit* unit, float r)
{
    int R = 0;
    int G = 0;
    int B = 0;
    float currPosY = unit->GetCenter().y - r;

    for (int i = currPosY - 2; i <= currPosY + 2; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), unit->GetCenter().x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckBlock(R, G, B))
        {
            //위치 고정
            unit->SetCenterY(i + r);

            return true;
        }
    }
    return false;
}

bool CollisionManager::CheckBottomPixelCollision(Unit* unit, float r)
{
    if (unit->GetUnitState() == UNIT_STATE::HANGING_STATE)
        return true;
    if (unit->GetJumpingState() == JUMPING_STATE::JUMPING_UP)
        return false;

    int R = 0;
    int G = 0;
    int B = 0;
    float currPosY = unit->GetCenter().y + r;

    for (int i = currPosY - 2; i <= currPosY + 2; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), unit->GetCenter().x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckBlock(R, G, B) || (CheckIsland(R, G, B) && !unit->GetJumpingDown()))
        {
            //위치 고정
            unit->SetCenterY(i - r);

            return true;
        }
    }
    return false;
}

bool CollisionManager::CheckRopePixelCollision(Unit* unit)
{
    int R = 0;
    int G = 0;
    int B = 0;

    //왼쪽 로프확인
    float currPosX = unit->GetCenter().x - (unit->GetCenterToLeft()*3.0f / 5.0f);
    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, unit->GetCenter().y + 20);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckLadder(R, G, B))
        {
            if (unit->GetUnitState() == UNIT_STATE::HANGING_STATE || unit->GetUnitState() == UNIT_STATE::HANGING_MOVE_STATE)
            {
                unit->SetCenterX(i);
            }
            return true;
        }
    }

    //오른쪽 로프확인
    currPosX = unit->GetCenter().x + (unit->GetCenterToRight() * 3.0f / 5.0f);
    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, unit->GetCenter().y + 20);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckLadder(R, G, B))
        {
            if (unit->GetUnitState() == UNIT_STATE::HANGING_STATE || unit->GetUnitState() == UNIT_STATE::HANGING_MOVE_STATE)
            {
                unit->SetCenterX(i);
            }
            return true;
        }
    }

    //중앙 로프확인
    currPosX = unit->GetCenter().x;
    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, unit->GetCenter().y + 20);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckLadder(R, G, B))
        {
            if (unit->GetUnitState() == UNIT_STATE::HANGING_STATE || unit->GetUnitState() == UNIT_STATE::HANGING_MOVE_STATE)
            {
                unit->SetCenterX(i);
            }
            return true;
        }
    }

    // 아래쪽 로프 확인
    float currPosY = 0.0f;
    if (unit->GetUnitState() == UNIT_STATE::LYING_STATE)
        currPosY = unit->GetCenter().y + unit->GetCenterToBottom() + 20;
    else
        currPosY = unit->GetCenter().y + unit->GetCenterToBottom();


    for (int i = currPosY - 1; i <= currPosY + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), unit->GetCenter().x, i);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckLadder(R, G, B))
        {
            if (unit->GetUnitState() == UNIT_STATE::LYING_STATE){
                unit->SetState(UNIT_STATE::HANGING_MOVE_STATE);
                unit->SetCenterY(i + 20);
            }
            //unit->SetHangingState(HANGING_STATE::BOTTOM_ALLOWED);
            return true;
        }
    }
    return false;
}

bool CollisionManager::CheckPortalPixelCollision(Unit* unit)
{
    int R = 0;
    int G = 0;
    int B = 0;
    float currPosX = unit->GetCenter().x;

    for (int i = currPosX - 1; i <= currPosX + 1; i++)
    {
        COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), i, unit->GetCenter().y + 20);

        R = GetRValue(color);
        G = GetGValue(color);
        B = GetBValue(color);

        if (CheckPortal(R, G, B))
        {
            return true;
        }
    }
    return false;
}

void CollisionManager::CheckPlayerEnemiesCollision()
{
    if (player && player->GetUnitState() != UNIT_STATE::DEAD_STATE && !player->GetHit())
    {
        RECT playerRect = { player->GetCenter().x - player->GetCenterToLeft(),
                            player->GetCenter().y - player->GetCenterToTop(),
                            player->GetCenter().x + player->GetCenterToRight(),
                            player->GetCenter().y + player->GetCenterToBottom() };
        for (Unit* enemy : *vEnemyUnits)
        {
            if (enemy->GetUnitState() == UNIT_STATE::DEAD_STATE || enemy->GetUnitState() == UNIT_STATE::END_OF_UNIT_STATE)
                continue;
            RECT enemyRect = {  enemy->GetCenter().x - enemy->GetCenterToLeft(),
                                enemy->GetCenter().y - enemy->GetCenterToTop(),
                                enemy->GetCenter().x + enemy->GetCenterToRight(),
                                enemy->GetCenter().y + enemy->GetCenterToBottom() };
            if (IntersectRect(&commonRect, &playerRect, &enemyRect) )
            {
                if(player->GetCenter().x < enemy->GetCenter().x)
                { 
                    if(player->GetCenter().x - player->GetWidth() / 2.0f >= 0 )
                        player->SetCenterX(player->GetCenter().x - (player->GetWidth()/2.0f));
                }
                else
                {
                    if (player->GetCenter().x + player->GetWidth() / 2.0f <= CameraManager::GetSingleton()->GetBG()->GetWidth())
                        player->SetCenterX(player->GetCenter().x + (player->GetWidth() / 2.0f));
                }
                player->SetCenterY(player->GetCenter().y - (player->GetHeight()));
                //TODO 플레이어 에너지 감소 및 데미지 이펙트
                ProcessDamage(enemy, player);
                player->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
                return;
            }
            else if (enemy->GetAttack() && (player->GetJumpingState() == JUMPING_STATE::JUST_LANDED || player->GetJumpingState() == JUMPING_STATE::END_OF_JUMPING_STATE) )
            {
                RECT attackBox = enemy->GetAttackBox();
                if (IntersectRect(&commonRect, &playerRect, &attackBox))//몬스터의 공격에 맞았을 경우
                {
                    if (player->GetCenter().x < enemy->GetCenter().x)
                    {
                        if (player->GetCenter().x - player->GetWidth() / 2.0f >= 0)
                            player->SetCenterX(player->GetCenter().x - (player->GetWidth() / 2.0f));
                    }
                    else
                    {
                        if (player->GetCenter().x + player->GetWidth() / 2.0f <= CameraManager::GetSingleton()->GetBG()->GetWidth())
                            player->SetCenterX(player->GetCenter().x + (player->GetWidth() / 2.0f));
                    }
                    player->SetCenterY(player->GetCenter().y - (player->GetHeight()));
                    //TODO 플레이어 에너지 감소 및 데미지 이펙트
                    ProcessDamage(enemy, player);
                    player->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
                    return;
                }
            }
        }
    }
}

void CollisionManager::CheckPlayerProtileEnemiesCollision()
{
    if (!player)
        return;
    RECT shurikenRect;
    RECT shurikenRange;
    RECT enemyRect;
    for (auto enemy : *vEnemyUnits)
    {
        if (enemy->GetUnitState() == UNIT_STATE::DEAD_STATE)
            continue;
        if(player->GetSkillManager()->GetCurrentSkill())
        {
            if (player->GetSkillManager()->GetCurrentSkill()->GetNeedShuriken())//수리검이 필요한 스킬
            {
                for (auto shuriken : player->GetSkillManager()->GetCurrentSkill()->GetVShurikens())
                {
                    if (shuriken->GetFired())
                    {
                        shurikenRect = { 
                                (LONG)(shuriken->GetPos().x - shuriken->GetWidth() / 2.0f),
                                (LONG)(shuriken->GetPos().y - shuriken->GetHeight() / 2.0f),
                                (LONG)(shuriken->GetPos().x + shuriken->GetWidth() / 2.0f),
                                (LONG)(shuriken->GetPos().y + shuriken->GetHeight() / 2.0f) };
                        shurikenRange = {
                                (LONG)(shuriken->GetPos().x - shuriken->GetWidth() * 2.0f),
                                (LONG)(shuriken->GetPos().y - shuriken->GetHeight() * 3.0f),
                                (LONG)(shuriken->GetPos().x + shuriken->GetWidth() * 2.0f),
                                (LONG)(shuriken->GetPos().y + shuriken->GetHeight() * 3.0f) };
                        enemyRect = { 
                                (LONG)(enemy->GetLocalPos().x - enemy->GetCenterToLeft()),
                                (LONG)(enemy->GetLocalPos().y - enemy->GetCenterToTop()),
                                (LONG)(enemy->GetLocalPos().x + enemy->GetCenterToRight()),
                                (LONG)(enemy->GetLocalPos().y + enemy->GetCenterToBottom()) };
                        if (IntersectRect(&commonRect, &shurikenRect, &enemyRect))
                        {
                            enemy->SetState(UNIT_STATE::HIT_STATE);
                            ((Monster*)enemy)->SetTarget(player);
                            ProcessDamage(player, enemy);
                            shuriken->SetFired(false);
                        }
                        else if (IntersectRect(&commonRect, &shurikenRange, &enemyRect) )
                        {
                            shuriken->SetTartget(enemy);
                        }
                    }

                }
            }
            else
            {
                //TODO 수리검이 아닌 스킬 범위로 충돌체크, 한번만 충돌처리되어야
            }
        }
    }
}

void CollisionManager::CheckPixelCollisionFor(Unit* unit, bool checkRope, bool checkPortal)
{
    //TODO 로프 체크도 해주기
    if (unit && (unit == player || unit->GetUnitState() != UNIT_STATE::DEAD_STATE) )
    {
        CheckPixelCollision(unit, CHECK_DIRECTION::LEFT, unit->GetCenterToLeft());
        CheckPixelCollision(unit, CHECK_DIRECTION::RIGHT, unit->GetCenterToRight());
        //CheckPixelCollision(unit, CHECK_DIRECTION::TOP, unit->GetCenterToTop());
        CheckPixelCollision(unit, CHECK_DIRECTION::BOTTOM, unit->GetCenterToBottom());
        if (checkRope)
        {
            CheckPixelCollision(unit, CHECK_DIRECTION::ROPE, 0.0f);
        }
        if (checkPortal)
        {
            CheckPixelCollision(unit, CHECK_DIRECTION::PORTAL, 0.0f);
        }

        //충돌 된 것들 처리
        if (hasLeftCollided)
        {
            //TODO 왼쪽 충돌일 때 처리
            hasLeftCollided = false;
        }
        else
        {

        }

        if (hasRightCollided)
        {
            hasRightCollided = false;
        }
        else
        {
        }

        if (hasTopCollided)
        {
            hasTopCollided = false;
        }
        else
        {

        }

        if (hasBottomCollided)
        {
            if (unit->GetJumpingState() == JUMPING_STATE::JUMPING_DOWN) 
            {
                if (unit->GetUnitState() == UNIT_STATE::DEAD_STATE)
                    unit->SetJumpingState(JUMPING_STATE::END_OF_JUMPING_STATE);
                else
                    unit->SetJumpingState(JUMPING_STATE::JUST_LANDED);
            }
            else
            {
                unit->SetJumpingState(JUMPING_STATE::END_OF_JUMPING_STATE);
                if (unit->GetUnitState() == UNIT_STATE::HANGING_MOVE_STATE && unit->GetHangingState() == HANGING_STATE::HANGED)
                {
                    unit->SetState(UNIT_STATE::DEFAULT_STATE);
                }
            }
            hasBottomCollided = false;
        }
        else
        { 
            if (unit->GetUnitState() != UNIT_STATE::HANGING_STATE && unit->GetUnitState() != UNIT_STATE::HANGING_MOVE_STATE && unit->GetJumpingState() == JUMPING_STATE::END_OF_JUMPING_STATE) {
                unit->SetState(UNIT_STATE::JUMPING_STATE);
                unit->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
            }
        }

        if (!checkRope)
            return;

        if (hasRopeCollided)
        {
            //TODO 로프가 근처에 있을 시 행동
            if(player->GetUnitState() != UNIT_STATE::HANGING_STATE && player->GetUnitState() != UNIT_STATE::HANGING_MOVE_STATE)
            {
                if(unit->GetHangingState() != HANGING_STATE::BOTTOM_ALLOWED)
                    unit->SetHangingState(HANGING_STATE::ALLOWED);
            }
            else
            {
                player->SetHangingState(HANGING_STATE::HANGED);
                player->SetJumpingState(JUMPING_STATE::END_OF_JUMPING_STATE);
            }
            hasRopeCollided = false;
        }
        else
        {
            //TODO 로프가 주변에 없을 시 행동
            if (player->GetUnitState() == UNIT_STATE::HANGING_STATE && player->GetJumpingState() == JUMPING_STATE::END_OF_JUMPING_STATE) {
                player->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
                //player->SetState(UNIT_STATE::JUMPING_STATE);
                player->SetCenterY(player->GetCenter().y - 30);
                return;
            }
            else if (player->GetUnitState() != UNIT_STATE::HANGING_STATE && player->GetUnitState() != UNIT_STATE::HANGING_MOVE_STATE)
                unit->SetHangingState(HANGING_STATE::NOT_ALLOWED);
        }

        if (!checkPortal)
            return;
        if (hasPortalCollided)
        {
            if (unit->GetPortalState() == PORTAL_STATE::NOT_ALLOWED)
            {
                unit->SetPortalState(PORTAL_STATE::ALLOWED);
            }
            hasPortalCollided = false;
        }
        else
        {
            unit->SetPortalState(PORTAL_STATE::NOT_ALLOWED);
        }
    }
}

void CollisionManager::ProcessDamage(Unit* attackUnit, Unit* defenseUnit)
{
    pair<bool, int> damagePair = CalcUtil::GetSingleton()->CalcDamage(defenseUnit->GetDefense(), defenseUnit->GetProtection(),
                        attackUnit->GetDamage(), attackUnit->GetCriticalPercentage(), attackUnit->GetCriticalDamage());
    bool isCritical = damagePair.first;
    int damage = damagePair.second;
    string damageStr = "";

    defenseUnit->SetHp(defenseUnit->GetHp() - damage);
    defenseUnit->CheckAlive();
    defenseUnit->SetHit(true);
    defenseUnit->GetDamageQueue()->push_back(CalcUtil::GetSingleton()->TurnIntegerIntoStr(damage), isCritical);
}

