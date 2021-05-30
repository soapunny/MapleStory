#pragma once
#include "Character.h"
#include "Managers.h"
#include "CalcUtil.h"
#include "Image.h"
#include "SkillManager.h"
#include "DamageQueue.h"

#define ATTACK_TIME 1.0f

HRESULT Character::Init()
{
    image = ImageManager::GetSingleton()->FindImage("Character");
    deathImage = ImageManager::GetSingleton()->FindImage("Death");
    timer = 0.0f; 
    isSkillOn = false;
    
    FileManager::GetSingleton()->ReadCharacterData(this);

    width = image->GetFrameWidth() * image->GetRenderRatio();
    height = image->GetFrameHeight() * image->GetRenderRatio();

    shape.left = center.x - width / 2.0f;
    shape.top = center.y - height / 2.0f;
    shape.right = shape.left + width;
    shape.bottom = shape.top + height;

    moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
    maxHp = 500;
    hp = maxHp;
    maxMp = 500;
    mp = maxMp;

    frame.x = 0;
    frame.y = 9;
    deathFrame.x = 0;
    deathFrame.y = 0;

    //캐릭터를 처음 시작 했을 때 낙하하게 세팅
    velocity = 60.0f;
    jumpAngle = PI / 2.0f;
    state = UNIT_STATE::JUMPING_STATE;
    jumpingState = JUMPING_STATE::JUMPING_DOWN;
    jumpTimer = 0.0f;
    priorPos.x = center.x;
    priorPos.y = center.y;
    isJumpingDown = false;
    hangingState = HANGING_STATE::NOT_ALLOWED;
    portalState = PORTAL_STATE::NOT_ALLOWED;
    blockedState = BLOCKED_STATE::END_OF_BLOCKED_STATE;

    damage = CalcUtil::GetSingleton()->AbilityToDamage(luck, dex);
    criticalDamage = 1.5f;

    skillManager = new SkillManager();
    skillManager->Init();

    InitHitTimer(false, 0.0f, 2.0f);

    font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
    damageQueue = new DamageQueue(
        RGB(127, 31, 245), RGB(77, 11, 155), 
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("타이포_꼬맹이마음 L")),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("타이포_꼬맹이마음 B"))
    );

    //Recovery
    recoveryInfo = new RecoveryInfo();
    InitHpRecovery(100, false, 0.0f, 5.0f, 2.0f);
    InitMpRecovery(100, false, 0.0f, 5.0f);
    recoveryQueue = new DamageQueue(
        RGB(0, 85, 255), RGB(0, 85, 255),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리")),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"))
    );

    CollisionManager::GetSingleton()->RegisterPlayer(this);

    return S_OK;
}

void Character::Release()
{
    if(font)
        DeleteObject(font);
    if (skillManager)
        SAFE_RELEASE(skillManager);
    if (recoveryInfo)
        SAFE_DELETE(recoveryInfo);
    if (damageQueue)
        SAFE_RELEASE(damageQueue);
}

//TODO 점프 매끄럽게 잘 되게 수정하기
//TODO 모든 실행에 elapsedTime을 곱해서 프레임에 관계없이 잘 실행되게 수정
void Character::Update()
{
    //점프상태
    if (jumpingState == JUMPING_STATE::JUMPING_UP)
    {
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = priorPos.x + velocity * cos(jumpAngle) * jumpTimer;
        center.y = priorPos.y - (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);// 점프타이머(-0.5*GA*점프타이머 + velocity * sin(PI/2.0f))
        
        if (jumpTimer >= velocity * sin(jumpAngle) / (0.5f * GA) / 2.0f)//낙하시간대로 들어서면
        {
            jumpingState = JUMPING_STATE::JUMPING_DOWN;
        }
    }
    else if (jumpingState == JUMPING_STATE::JUMPING_DOWN)
    {
        //점프가 아니라 위에서 갑자기 뚝 떨어질 경우
        if (jumpTimer < velocity * sin(jumpAngle) / (0.5f * GA) / 2.0f) {//낙하시간이 아니라 이륙시간대이면
            jumpTimer = velocity * sin(jumpAngle / (0.5f * GA) / 2.0f);//낙하 시간대으로 바꿔주고
            //현재 위치에서 포물선의 최고점 위치 만큼 빼준다. -> 아래식서 현재위치가 포물선의 최고점이되게
            priorPos.x = center.x;
            priorPos.y = center.y + (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
        }
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = priorPos.x + velocity * cos(jumpAngle) * jumpTimer;
        center.y = priorPos.y - (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
    }
    else if(jumpingState == JUMPING_STATE::JUST_LANDED)//방금 착지했을 때
    {
        state = UNIT_STATE::DEFAULT_STATE;
        priorPos.x = center.x;
        priorPos.y = center.y;
        jumpingState = JUMPING_STATE::END_OF_JUMPING_STATE;
    }
    else
    {
        isSkillOn = false;
        velocity = 60.0f;
        jumpAngle = PI / 2.0f;
        jumpTimer = 0.0f;
        isJumpingDown = false;
    }
    //이동 막아주기
    if (center.x <= GetCenterToLeft())
    {
        center.x = GetCenterToLeft();
    }
    else if (center.x >= CameraManager::GetSingleton()->GetBG()->GetWidth() - GetCenterToRight())
    {
        center.x = CameraManager::GetSingleton()->GetBG()->GetWidth() - GetCenterToRight();
    }


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
        HandleAttackState(skillManager->GetCurrentSkillType());
        break;
    case UNIT_STATE::JUMPING_STATE:
        HandleJumpingState();
        break;
    case UNIT_STATE::JUMPING_ATTACK_STATE:
        HandleJumpingAttackState(skillManager->GetCurrentSkillType());
        break;
    case UNIT_STATE::LYING_STATE:
        HandleLyingState();
        break;
    case UNIT_STATE::LYING_ATTACK_STATE:
        HandleLyingAttackState();
        break;
    case UNIT_STATE::HANGING_STATE:
        HandleHangingState();
        break;
    case UNIT_STATE::HANGING_MOVE_STATE:
        HandleHangingMoveState();
        break;
    case UNIT_STATE::SKILL_STATE:
        HandleSkillState();
        break;
    case UNIT_STATE::DEAD_STATE:
        HandleDeadState();
        break;
    default:
        break;
    }

    //동전 줍기는 상태에 상관없이 항상 가능하다.
    if (KeyManager::GetSingleton()->IsStayKeyDown('Z'))
    {
        //TODO 동전 줍기
    }
    //skill
    skillManager->Update();
    //맞았을때 HitTimer 실행
    CountHitTimer();
    CountRecovery();
    damageQueue->Update();
    recoveryQueue->Update();
    //가만히 있을때 RecoveryTimer 실행

}

void Character::HandleDefaultState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
    {
        state = UNIT_STATE::WALKING_STATE;
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        state = UNIT_STATE::WALKING_STATE;
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        //TODO 로프가 있는지 포탈이 있는 지 확인
        if(hangingState == HANGING_STATE::ALLOWED){
            state = UNIT_STATE::HANGING_STATE;
            center.y = center.y - 5;
            return;
        }
        else if (portalState == PORTAL_STATE::ALLOWED)
        {
            portalState = PORTAL_STATE::USING;
            return;
        }
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        state = UNIT_STATE::LYING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_SHIFT))
    {
        if (skillManager->FireSkill(SKILL_TYPE::LUCKY_SEVEN, this, moveDirection))
        {
            state = UNIT_STATE::ATTACK_STATE;
            return;
        }
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('X'))
    {
        state = UNIT_STATE::JUMPING_STATE;
        return;
    }
    

    ShowAnimation(UNIT_STATE::DEFAULT_STATE);//애니메이션 처리
}

void Character::HandleWalkingState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        state = UNIT_STATE::LYING_STATE;
        return;
    }
    
    if (KeyManager::GetSingleton()->IsKeyUp(VK_LEFT) && KeyManager::GetSingleton()->IsKeyUp(VK_RIGHT))
    {
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }
    if (KeyManager::GetSingleton()->IsStayKeyDown('X'))
    {
        state = UNIT_STATE::JUMPING_STATE;
        return;
    }
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_SHIFT))
    {
        if (skillManager->FireSkill(SKILL_TYPE::LUCKY_SEVEN, this, moveDirection))
        {
            state = UNIT_STATE::ATTACK_STATE;
            return;
        }
    }
    //캐릭터 이동
    Move();

    ShowAnimation(UNIT_STATE::WALKING_STATE);//애니메이션 처리
}

void Character::HandleAttackState(SKILL_TYPE skillType)
{
   //공격상태에서는 아무키도 먹지 않는다.
   if(skillType == SKILL_TYPE::END_OF_SKILL_TYPE)
    {
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }

   ShowAnimation(UNIT_STATE::ATTACK_STATE);//애니메이션 처리
}

void Character::HandleJumpingState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        if (hangingState == HANGING_STATE::ALLOWED) {
            state = UNIT_STATE::HANGING_STATE;
            return;
        }
    }


    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
        Move();
        priorPos.x = center.x;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        Move();
        priorPos.x = center.x;
    }
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_SHIFT))
    {
        if (skillManager->FireSkill(SKILL_TYPE::LUCKY_SEVEN, this, moveDirection))
        {
            state = UNIT_STATE::ATTACK_STATE;
            return;
        }
    }

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//처음 점프를 했을 때
    {
        //d = v * t; 포물선 운동
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPos.x = center.x;
        priorPos.y = center.y;
        jumpTimer = 0.0f;
    }
    if (KeyManager::GetSingleton()->IsStayKeyDown('C'))
    {
        state = UNIT_STATE::SKILL_STATE;
        return;
    }

    ShowAnimation(UNIT_STATE::JUMPING_STATE);//애니메이션 처리
}

//점프 중 공격
void Character::HandleJumpingAttackState(SKILL_TYPE skillType)
{
    //공격상태에서는 아무키도 먹지 않는다.
    if (skillType == SKILL_TYPE::END_OF_SKILL_TYPE)
    {
        state = UNIT_STATE::JUMPING_STATE;
        return;
    }
    ShowAnimation(UNIT_STATE::JUMPING_ATTACK_STATE);//애니메이션 처리
}

//엎드린 상태
void Character::HandleLyingState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        //state = UNIT_STATE::LYING_ATTACK_STATE; //현재 구현되지 않은 상태(리소스의 부재)
        //return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
    {
        isJumpingDown = false;
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
        state = UNIT_STATE::WALKING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        isJumpingDown = false;
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        state = UNIT_STATE::WALKING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsKeyUp(VK_DOWN))
    {
        if (isJumpingDown) {
            isJumpingDown = false;
            state = UNIT_STATE::JUMPING_STATE;
            return;
        }
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('X'))
    {
        isJumpingDown = true;
    }
    else
    {
        isJumpingDown = false;
    }

    ShowAnimation(UNIT_STATE::LYING_STATE);
}

//엎드려서 공격
void Character::HandleLyingAttackState()
{
    //현재 구현되지 않음
}

//밧줄에 매달린 상태
void Character::HandleHangingState()
{
    if (jumpingState != JUMPING_STATE::END_OF_JUMPING_STATE)
    {
        state = UNIT_STATE::JUMPING_STATE;
        return;
    }
    hangingState = HANGING_STATE::HANGED;
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        state = UNIT_STATE::HANGING_MOVE_STATE;
        return;
    }
    else if(KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        state = UNIT_STATE::HANGING_MOVE_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('X'))
    {
        state = UNIT_STATE::JUMPING_STATE;
        jumpingState = JUMPING_STATE::JUMPING_DOWN;
        return;
    }

    ShowAnimation(UNIT_STATE::HANGING_STATE);
}

void Character::HandleHangingMoveState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        moveDirection = MOVE_DIRECTION::MOVE_UP;
        Move();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        moveDirection = MOVE_DIRECTION::MOVE_DOWN;
        Move();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown('X'))
    {
        state = UNIT_STATE::JUMPING_STATE;
        jumpingState = JUMPING_STATE::JUMPING_DOWN;
        return;
    }
    else
    {
        state = UNIT_STATE::HANGING_STATE;
        return;
    }

    ShowAnimation(UNIT_STATE::HANGING_MOVE_STATE);
}

void Character::HandleSkillState()
{
    if(!isSkillOn){
        if (KeyManager::GetSingleton()->IsStayKeyDown('C'))
        {
            if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
            {
                jumpAngle = PI / 2.0f + DegToRad(45);
            }
            else if(moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            {
                jumpAngle = PI / 2.0f - DegToRad(45);
            }
            velocity = 100.0f;
            jumpTimer = 0.0f;
            jumpingState = JUMPING_STATE::JUMPING_UP;
            priorPos.x = center.x;
            priorPos.y = center.y;

            //skillManager->FireSkill("flashJump", shape);
        }
        isSkillOn = true;
    }
    //TODO SKILL_STATE로 구현 및 수정 요망
    ShowAnimation(UNIT_STATE::JUMPING_STATE);
}

void Character::HandleDeadState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RETURN))
    {
        state = UNIT_STATE::DEFAULT_STATE;
        //Init deathFrame
        deathFrame.x = 0;
        deathFrame.y = 0;
        hp = maxHp/4.0f;
        return;
    }
    ShowAnimation(UNIT_STATE::DEAD_STATE);
    DropTheTombstone();
}

//캐릭터 이동

//애니메이션 그리기
void Character::ShowAnimation(UNIT_STATE unitState)
{
    int nextFrameY = static_cast<int>(UnitStateToCharacterFrameY(unitState) );
    //캐릭터 상태가 고정된 상태에서 왼쪽 오른쪽이 나눠져 있을 때
    if(unitState == UNIT_STATE::DEFAULT_STATE || unitState == UNIT_STATE::JUMPING_STATE || unitState == UNIT_STATE::LYING_STATE)
    {
        if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
        {
            frame.x = static_cast<int>(MOVE_DIRECTION::MOVE_LEFT);
        }
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
        {
            frame.x = static_cast<int>(MOVE_DIRECTION::MOVE_RIGHT);
        }
        frame.y = nextFrameY;
        return;
    }
    else if (unitState == UNIT_STATE::HANGING_STATE)
    {
        frame.x = static_cast<int>(MOVE_DIRECTION::MOVE_RIGHT);
        frame.y = nextFrameY;
        return;
    }

    timer += TimerManager::GetSingleton()->GetElapsedTime();
    if (timer >= 0.2f)
    {
        if (frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_2) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_3)) 
        {
            if (nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1) && nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_2) && nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_3))
            {
                frame.x = 0;
                frame.y = nextFrameY;
            }
            else
            {
                frame.x++;
                if (frame.x >= image->GetVMaxFrameX(frame.y))
                {
                    frame.x--;
                    if (state == UNIT_STATE::ATTACK_STATE)
                        state = UNIT_STATE::DEFAULT_STATE;
                    else if (state == UNIT_STATE::JUMPING_ATTACK_STATE)
                        state = UNIT_STATE::JUMPING_STATE;
                }
            }
        }
        else if (frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_2) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_3))
        {
            if (nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1) && nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_2) && nextFrameY != static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_3))
            {
                frame.x = 0;
                frame.y = nextFrameY;
            }
            else
            {
                frame.x++;
                if (frame.x >= image->GetVMaxFrameX(frame.y))
                {
                    frame.x--;
                    if (state == UNIT_STATE::ATTACK_STATE)
                        state = UNIT_STATE::DEFAULT_STATE;
                    else if (state == UNIT_STATE::JUMPING_ATTACK_STATE)
                        state = UNIT_STATE::JUMPING_STATE;
                }
            }
        }
        else if (nextFrameY == frame.y)
        {
            frame.x++;
            if (frame.x >= image->GetVMaxFrameX(frame.y))
            {
                frame.x = 0;
            }
        }
        else
        {
            frame.x = 0;
            frame.y = nextFrameY;
        }

        timer = 0.0f;
    }
}

void Character::DropTheTombstone()
{
    if (deathFrame.y >= deathImage->GetMaxFrameY() - 1)
        return;
    if (deathFrame.x == 0 && deathFrame.y == 0)
    {
        tombstonePos.x = shape.left - width/4.0f;
        tombstonePos.y = shape.top - WINSIZE_Y/2.0f;
    }
    deathTimer += TimerManager::GetSingleton()->GetElapsedTime();
    if(deathTimer >= 0.1f)
    { 
        deathFrame.x++;
        tombstonePos.y += WINSIZE_Y / 12.0f * 2;
        if (tombstonePos.y >= shape.top + height/2.0f)
        {
            tombstonePos.y = shape.top + height / 2.0f;
        }
        if (deathFrame.x >= deathImage->GetVMaxFrameX(deathFrame.y))
        {
            deathFrame.x = 0;
            deathFrame.y++;
        }
        deathTimer = 0.0f;
    }
}

//유닛의 상태에 해당하는 애니메이션 시작 Y프레임을 반환
CHARACTER_FRAME_Y Character::UnitStateToCharacterFrameY(UNIT_STATE unitState)
{
    CHARACTER_FRAME_Y result = CHARACTER_FRAME_Y::DEFAULT;
    switch (unitState)
    {
    case UNIT_STATE::DEFAULT_STATE:
        //기본값이 DEFAULT
        break;
    case UNIT_STATE::WALKING_STATE:
        if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
            result = CHARACTER_FRAME_Y::LEFT_WALK;
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            result = CHARACTER_FRAME_Y::RIGHT_WALK;
        break;
    case UNIT_STATE::ATTACK_STATE:
        if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
            result = CHARACTER_FRAME_Y(static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1) + rand() % 3);
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            result = CHARACTER_FRAME_Y(static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1) + rand() % 3);
        break;
    case UNIT_STATE::JUMPING_STATE:
        result = CHARACTER_FRAME_Y::JUMP;
        break;
    case UNIT_STATE::JUMPING_ATTACK_STATE:
        if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
            result = CHARACTER_FRAME_Y(static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1) + rand() % 3);
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            result = CHARACTER_FRAME_Y(static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1) + rand() % 3);
        break;
    case UNIT_STATE::LYING_STATE:
        result = CHARACTER_FRAME_Y::LYING;
        break;
    case UNIT_STATE::LYING_ATTACK_STATE:
        //이미지가 존재하지 않음 TODO 나중에 이미지 수정 요망
        result = CHARACTER_FRAME_Y::LYING;
        break;
    case UNIT_STATE::HANGING_STATE:
        result = CHARACTER_FRAME_Y::HANGING_1;
        break;
    case UNIT_STATE::HANGING_MOVE_STATE:
        result = CHARACTER_FRAME_Y::HANGING_1;
        break;
    case UNIT_STATE::DEAD_STATE:
        result = CHARACTER_FRAME_Y::DEATH;
        break;
    default:
        break;
    }

    return result;
}

void Character::Render(HDC hdc)
{
    if (state == UNIT_STATE::DEAD_STATE) 
    {
        deathImage->FrameRender(hdc, tombstonePos.x, tombstonePos.y, deathFrame.x, deathFrame.y, false);
        image->AlphaFrameRender(hdc, shape.left, shape.top, frame.x, frame.y, false);
        return;
    }
    if (isHit && (int)(hitTimer * 10.0f) % 2 == 0)
    {
        //image->BlinkRender(hdc, shape.left, shape.top, frame.x, frame.y, false);
    }
    else
    {
        image->FrameRender(hdc, shape.left, shape.top, frame.x, frame.y, false);
    }
    if (skillManager)
        skillManager->Render(hdc);
}