#pragma once
#include "Mushmom.h"
#include "Managers.h"
#include "DamageQueue.h"
#include "Image.h"

HRESULT Mushmom::Init()
{
    image = ImageManager::GetSingleton()->FindImage("Mushmom");
    attackImage = ImageManager::GetSingleton()->FindImage("Mushmom_Skill");
    timer = 0.0f;

    FileManager::GetSingleton()->ReadMonsterData(this);

    width = image->GetFrameWidth() * image->GetRenderRatio();
    height = image->GetFrameHeight() * image->GetRenderRatio();

    shape.left = center.x - width / 2.0f;
    shape.top = center.y - height / 2.0f;
    shape.right = shape.left + width;
    shape.bottom = shape.top + height;

    moveDirection = MOVE_DIRECTION::MOVE_LEFT;

    frame.x = 0;
    frame.y = 1;

    //HP
    hp = maxHp;

    //캐릭터를 처음 시작 했을 때 낙하하게 세팅
    velocity = 60.0f;
    state = UNIT_STATE::JUMPING_STATE;
    jumpingState = JUMPING_STATE::JUMPING_DOWN;
    jumpTimer = 0.0f;
    priorPosY = center.y;
    isJumpingDown = false;

    animationTimer = 0.0f;

    blockedState = BLOCKED_STATE::END_OF_BLOCKED_STATE;

    isAttack = false;
    isReadyToAttack = false;
    attackRange = 600.0f;
    attackTimer = 0.0f;
    hitTimer = 0.0f;

    damageQueue = new DamageQueue(
        RGB(200, 200, 0), RGB(250, 0, 250),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("타이포_꼬맹이마음 L")),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("타이포_꼬맹이마음 B"))
    );

    criticalDamage = 1.5f;
    criticalPercentage = 0.3f;

    return S_OK;
}

void Mushmom::Update()
{
    if (state == UNIT_STATE::END_OF_UNIT_STATE)
        return;

    HandleJumpProcess();
    HandleEarthQuakeAttack();

    //유한상태 기계 만들기
    switch (state)
    {
    case UNIT_STATE::DEFAULT_STATE:
        HandleDefaultState();
        break;
    case UNIT_STATE::WALKING_STATE:
        HandleWalkingState();
        break;
    case UNIT_STATE::ATTACK_STATE:
        HandleAttackState();
        break;
    case UNIT_STATE::JUMPING_STATE:
        HandleJumpingState();
    case UNIT_STATE::HIT_STATE:
        HandleHitState();
    case UNIT_STATE::DEAD_STATE:
        HandleDeadState();
    default:
        break;
    }

    timer += TimerManager::GetSingleton()->GetElapsedTime();

    shape.left = center.x - width / 2.0f;
    shape.top = center.y - height / 2.0f;
    shape.right = shape.left + width;
    shape.bottom = shape.top + height;

    //맞았을때 HitTimer 실행
    CountHitTimer();
    damageQueue->Update();
    ShowAnimation(state);
}

void Mushmom::HandleJumpProcess()
{
    //점프상태
    if (jumpingState == JUMPING_STATE::JUMPING_UP)
    {
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        center.x = center.x + velocity * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (velocity * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);// 점프타이머(-0.5*GA*점프타이머 + velocity * sin(PI/2.0f))

        if (jumpTimer >= velocity * sin(PI / 2.0f) / (0.5f * GA) / 2.0f)//낙하시간대로 들어서면
        {
            jumpingState = JUMPING_STATE::JUMPING_DOWN;
        }
    }
    else if (jumpingState == JUMPING_STATE::JUMPING_DOWN)
    {
        //점프가 아니라 위에서 갑자기 뚝 떨어질 경우
        if (jumpTimer < velocity * sin(PI / 2.0f) / (0.5f * GA) / 2.0f) {//낙하시간이 아니라 이륙시간대이면
            jumpTimer = velocity * sin(PI / 2.0f / (0.5f * GA) / 2.0f);//낙하 시간대으로 바꿔주고
            //현재 위치에서 포물선의 최고점 위치 만큼 빼준다. -> 아래식서 현재위치가 포물선의 최고점이되게
            priorPosY = center.y + (velocity * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
        }
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime() * 10;
        center.x = center.x + velocity * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (velocity * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
    }
    else if (jumpingState == JUMPING_STATE::JUST_LANDED)//방금 착지했을 때
    {
        if (state != UNIT_STATE::ATTACK_STATE)
            state = UNIT_STATE::DEFAULT_STATE;

        jumpTimer = 0.0f;
        priorPosY = center.y;
        jumpingState = JUMPING_STATE::END_OF_JUMPING_STATE;

    }
    else
    {
        jumpTimer = 0.0f;
    }
}

void Mushmom::HandleEarthQuakeAttack()
{
    if (isReadyToAttack && jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)
    {
        isAttack = true;
        attackTimer += TimerManager::GetSingleton()->GetElapsedTime();
        if (attackTimer >= 0.6f)
        {
            isAttack = false;
            isReadyToAttack = false;
            attackFrame.x = 0;
            attackTimer = 0.0f;
        }
    }
}

void Mushmom::ShowAnimation(UNIT_STATE unitState)
{
    animationTimer += TimerManager::GetSingleton()->GetElapsedTime();

    if(animationTimer >= 0.2f)
    {
        animationTimer = 0.0f;

        if (isAttack)
        {
            attackFrame.x++;
        }

        if (state == UNIT_STATE::JUMPING_STATE)
        {
            frame.x = 3;
            frame.y = static_cast<int>(UNIT_STATE::WALKING_STATE) + static_cast<int>(moveDirection) * 4;
            return;
        }
        else if (state == UNIT_STATE::HIT_STATE)
        {
            frame.x = 1;
            frame.y = static_cast<int>(UNIT_STATE::DEFAULT_STATE) + static_cast<int>(moveDirection) * 4;
            return;
        }

        //이전과 프레임 y가 같으면(상태변경이 없었으면)
        if (frame.y == (static_cast<int>(unitState) + static_cast<int>(moveDirection) * 4))
        {
            frame.x++;
            if (unitState == UNIT_STATE::DEFAULT_STATE)
                frame.x = 0;
            else if (frame.x >= image->GetVMaxFrameX(frame.y))
            {
                if (unitState == UNIT_STATE::ATTACK_STATE)
                {
                    frame.x = image->GetVMaxFrameX(frame.y) - 1;
                    return;
                }
                frame.x = 0;
            }
            return;
        }
        else if ((frame.y == 3 || frame.y == 7) && unitState == UNIT_STATE::DEAD_STATE)
        {
            if(frame.x < image->GetVMaxFrameX(frame.y))
                frame.x++;
            return;
        }

        //상태변경이 있었으면
        frame.x = 0;
        if (unitState == UNIT_STATE::DEFAULT_STATE)
        {
            frame.y = static_cast<int>(UNIT_STATE::DEFAULT_STATE);
        }
        else if (unitState == UNIT_STATE::WALKING_STATE)
        {
            frame.y = static_cast<int>(UNIT_STATE::WALKING_STATE);
        }
        else if (unitState == UNIT_STATE::ATTACK_STATE)
        {
            frame.y = static_cast<int>(UNIT_STATE::ATTACK_STATE);
        }
        else if (unitState == UNIT_STATE::DEAD_STATE)
        {
            frame.y = 3;
        }

        if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
        {
            frame.y += 4;
        }
    }
}

void Mushmom::HandleDefaultState()
{
    if (timer >= 2.0f) {
        state = UNIT_STATE::WALKING_STATE;

        timer = 0.0f;
    }
}

void Mushmom::HandleWalkingState()
{
    Move();
    if (timer >= 3.0f) {
        if (player != NULL)
        {
            state = UNIT_STATE::ATTACK_STATE;
            if (player->GetCenter().x < center.x)
                moveDirection = MOVE_DIRECTION::MOVE_LEFT;
            else
                moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        }
        else
        {
            state = UNIT_STATE::JUMPING_STATE;
            if (rand() % 2 == 0)
            {
                moveDirection = MOVE_DIRECTION::MOVE_LEFT;
            }
            else
            {
                moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
            }
        }
            
        timer = 0.0f;
    }
}

void Mushmom::HandleAttackState()
{
    if (timer >= 3.0f) {
        state = UNIT_STATE::DEFAULT_STATE;

        if (player != nullptr)
        {
            if (player->GetCenter().x < center.x)
                moveDirection = MOVE_DIRECTION::MOVE_LEFT;
            else
                moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        }
        else
        {
            state = UNIT_STATE::JUMPING_STATE;
            if (rand() % 2 == 0)
            {
                moveDirection = MOVE_DIRECTION::MOVE_LEFT;
            }
            else
            {
                moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
            }
        }
        timer = 0.0f;
    }
    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE && frame.x == 7)//처음 점프를 했을 때
    {
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
        
        //Attack
        attackTimer = 0.0f;
        isReadyToAttack = true;
        SetRect(&attackBox, center.x - attackRange, center.y + GetCenterToBottom() - 50, center.x + attackRange, center.y + GetCenterToBottom() + 50);
    }
    else if (frame.x == image->GetVMaxFrameX(frame.y) - 1)
    {
        jumpingState = JUMPING_STATE::JUMPING_DOWN;
    }
}

void Mushmom::HandleJumpingState()
{
    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//처음 점프를 했을 때
    {
        //d = v * t; 포물선 운동
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
    }
    timer = 1.0f;
}

void Mushmom::HandleHitState()
{
    if (isHit)
    {
        isHit = false;
        hitTimer = 0.0f;
    }
    hitTimer += TimerManager::GetSingleton()->GetElapsedTime();
    if (hitTimer >= 0.6f)
    {
        state = UNIT_STATE::WALKING_STATE;
        hitTimer = 0.0f;
        return;
    }
}

void Mushmom::HandleDeadState()
{
    //TODO 돈과 아이템을 바닥에 드랍한다

    //TODO 종료한다.
    if (frame.x >= image->GetVMaxFrameX(frame.y))
    {
        state = UNIT_STATE::END_OF_UNIT_STATE;
    }
}
