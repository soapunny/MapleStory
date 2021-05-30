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

    //ĳ���͸� ó�� ���� ���� �� �����ϰ� ����
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

    font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("�����ý��丮"));
    damageQueue = new DamageQueue(
        RGB(127, 31, 245), RGB(77, 11, 155), 
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("Ÿ����_�����̸��� L")),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("Ÿ����_�����̸��� B"))
    );

    //Recovery
    recoveryInfo = new RecoveryInfo();
    InitHpRecovery(100, false, 0.0f, 5.0f, 2.0f);
    InitMpRecovery(100, false, 0.0f, 5.0f);
    recoveryQueue = new DamageQueue(
        RGB(0, 85, 255), RGB(0, 85, 255),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("�����ý��丮")),
        CreateFontA(60, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("�����ý��丮"))
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

//TODO ���� �Ų����� �� �ǰ� �����ϱ�
//TODO ��� ���࿡ elapsedTime�� ���ؼ� �����ӿ� ������� �� ����ǰ� ����
void Character::Update()
{
    //��������
    if (jumpingState == JUMPING_STATE::JUMPING_UP)
    {
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = priorPos.x + velocity * cos(jumpAngle) * jumpTimer;
        center.y = priorPos.y - (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);// ����Ÿ�̸�(-0.5*GA*����Ÿ�̸� + velocity * sin(PI/2.0f))
        
        if (jumpTimer >= velocity * sin(jumpAngle) / (0.5f * GA) / 2.0f)//���Ͻð���� ����
        {
            jumpingState = JUMPING_STATE::JUMPING_DOWN;
        }
    }
    else if (jumpingState == JUMPING_STATE::JUMPING_DOWN)
    {
        //������ �ƴ϶� ������ ���ڱ� �� ������ ���
        if (jumpTimer < velocity * sin(jumpAngle) / (0.5f * GA) / 2.0f) {//���Ͻð��� �ƴ϶� �̷��ð����̸�
            jumpTimer = velocity * sin(jumpAngle / (0.5f * GA) / 2.0f);//���� �ð������� �ٲ��ְ�
            //���� ��ġ���� �������� �ְ��� ��ġ ��ŭ ���ش�. -> �Ʒ��ļ� ������ġ�� �������� �ְ����̵ǰ�
            priorPos.x = center.x;
            priorPos.y = center.y + (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
        }
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = priorPos.x + velocity * cos(jumpAngle) * jumpTimer;
        center.y = priorPos.y - (velocity * sin(jumpAngle) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
    }
    else if(jumpingState == JUMPING_STATE::JUST_LANDED)//��� �������� ��
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
    //�̵� �����ֱ�
    if (center.x <= GetCenterToLeft())
    {
        center.x = GetCenterToLeft();
    }
    else if (center.x >= CameraManager::GetSingleton()->GetBG()->GetWidth() - GetCenterToRight())
    {
        center.x = CameraManager::GetSingleton()->GetBG()->GetWidth() - GetCenterToRight();
    }


    //���ѻ��� ��� �����
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

    //���� �ݱ�� ���¿� ������� �׻� �����ϴ�.
    if (KeyManager::GetSingleton()->IsStayKeyDown('Z'))
    {
        //TODO ���� �ݱ�
    }
    //skill
    skillManager->Update();
    //�¾����� HitTimer ����
    CountHitTimer();
    CountRecovery();
    damageQueue->Update();
    recoveryQueue->Update();
    //������ ������ RecoveryTimer ����

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
        //TODO ������ �ִ��� ��Ż�� �ִ� �� Ȯ��
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
    

    ShowAnimation(UNIT_STATE::DEFAULT_STATE);//�ִϸ��̼� ó��
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
    //ĳ���� �̵�
    Move();

    ShowAnimation(UNIT_STATE::WALKING_STATE);//�ִϸ��̼� ó��
}

void Character::HandleAttackState(SKILL_TYPE skillType)
{
   //���ݻ��¿����� �ƹ�Ű�� ���� �ʴ´�.
   if(skillType == SKILL_TYPE::END_OF_SKILL_TYPE)
    {
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }

   ShowAnimation(UNIT_STATE::ATTACK_STATE);//�ִϸ��̼� ó��
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

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//ó�� ������ ���� ��
    {
        //d = v * t; ������ �
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

    ShowAnimation(UNIT_STATE::JUMPING_STATE);//�ִϸ��̼� ó��
}

//���� �� ����
void Character::HandleJumpingAttackState(SKILL_TYPE skillType)
{
    //���ݻ��¿����� �ƹ�Ű�� ���� �ʴ´�.
    if (skillType == SKILL_TYPE::END_OF_SKILL_TYPE)
    {
        state = UNIT_STATE::JUMPING_STATE;
        return;
    }
    ShowAnimation(UNIT_STATE::JUMPING_ATTACK_STATE);//�ִϸ��̼� ó��
}

//���帰 ����
void Character::HandleLyingState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        //state = UNIT_STATE::LYING_ATTACK_STATE; //���� �������� ���� ����(���ҽ��� ����)
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

//������� ����
void Character::HandleLyingAttackState()
{
    //���� �������� ����
}

//���ٿ� �Ŵ޸� ����
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
    //TODO SKILL_STATE�� ���� �� ���� ���
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

//ĳ���� �̵�

//�ִϸ��̼� �׸���
void Character::ShowAnimation(UNIT_STATE unitState)
{
    int nextFrameY = static_cast<int>(UnitStateToCharacterFrameY(unitState) );
    //ĳ���� ���°� ������ ���¿��� ���� �������� ������ ���� ��
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

//������ ���¿� �ش��ϴ� �ִϸ��̼� ���� Y�������� ��ȯ
CHARACTER_FRAME_Y Character::UnitStateToCharacterFrameY(UNIT_STATE unitState)
{
    CHARACTER_FRAME_Y result = CHARACTER_FRAME_Y::DEFAULT;
    switch (unitState)
    {
    case UNIT_STATE::DEFAULT_STATE:
        //�⺻���� DEFAULT
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
        //�̹����� �������� ���� TODO ���߿� �̹��� ���� ���
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