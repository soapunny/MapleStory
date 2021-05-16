#include "Character.h"
#include "ImageManager.h"
#include "Image.h"
#include "SkillManager.h"
#include <unordered_map>

#define VELOCITY    60.0f
#define ATTACK_TIME 1.0f

HRESULT Character::Init()
{
    image = ImageManager::GetSingleton()->FindImage("Character");

    timer = 0.0f;
    
    FileManager::GetSingleton()->ReadCharacterData(this);

    width = 100.0f;
    height = 107.0f;

    shape.left = center.x - width / 2.0f;
    shape.top = center.y - height / 2.0f;
    shape.right = shape.left + width;
    shape.bottom = shape.top + height;

    moveDirection = MOVE_DIRECTION::MOVE_RIGHT;

    frame.x = 0;
    frame.y = 9;

    //ĳ���͸� ó�� ���� ���� �� �����ϰ� ����
    state = UNIT_STATE::JUMPING_STATE;
    jumpingState = JUMPING_STATE::JUMPING_DOWN;
    jumpTimer = 0.0f;
    priorPosY = center.y;
    isJumpingDown = false;

    blockedState = BLOCKED_STATE::END_OF_BLOCKED_STATE;

    attackTimer = 0.0f;

    skillManager = new SkillManager();
    skillManager->Init();

    CollisionManager::GetSingleton()->RegisterPlayer(this);

    return S_OK;
}

void Character::Release()
{
    if (skillManager)
        SAFE_RELEASE(skillManager);
}

//TODO ���� �Ų����� �� �ǰ� �����ϱ�
//TODO ��� ���࿡ elapsedTime�� ���ؼ� �����ӿ� ������� �� ����ǰ� ����
void Character::Update()
{

    //��������
    if (jumpingState == JUMPING_STATE::JUMPING_UP)
    {
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = center.x + VELOCITY * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);// ����Ÿ�̸�(-0.5*GA*����Ÿ�̸� + VELOCITY * sin(PI/2.0f))
        
        if (jumpTimer >= VELOCITY * sin(PI / 2.0f) / (0.5f * GA) / 2.0f)//���Ͻð���� ����
        {
            jumpingState = JUMPING_STATE::JUMPING_DOWN;
        }
    }
    else if (jumpingState == JUMPING_STATE::JUMPING_DOWN)
    {
        //������ �ƴ϶� ������ ���ڱ� �� ������ ���
        if (jumpTimer < VELOCITY * sin(PI / 2.0f) / (0.5f * GA) / 2.0f) {//���Ͻð��� �ƴ϶� �̷��ð����̸�
            jumpTimer = VELOCITY * sin(PI / 2.0f / (0.5f * GA) / 2.0f);//���� �ð������� �ٲ��ְ�
            //���� ��ġ���� �������� �ְ��� ��ġ ��ŭ ���ش�. -> �Ʒ��ļ� ������ġ�� �������� �ְ����̵ǰ�
            priorPosY = center.y + (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
        }
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = center.x + VELOCITY * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
    }
    else if(jumpingState == JUMPING_STATE::JUST_LANDED)//��� �������� ��
    {
        state = UNIT_STATE::DEFAULT_STATE;
        jumpTimer = 0.0f;
        priorPosY = center.y;
        jumpingState = JUMPING_STATE::END_OF_JUMPING_STATE;
    }
    else
    {
        jumpTimer = 0.0f;
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
        HandleAttackState();
        break;
    case UNIT_STATE::JUMPING_STATE:
        HandleJumpingState();
        break;
    case UNIT_STATE::JUMPING_ATTACK_STATE:
        HandleJumpingAttackState();
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
    default:
        break;
    }

    //���� �ݱ�� ���¿� ������� �׻� �����ϴ�.
    if (KeyManager::GetSingleton()->IsStayKeyDown('Z'))
    {
        //TODO ���� �ݱ�
    }

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
        state = UNIT_STATE::HANGING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        state = UNIT_STATE::LYING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        state = UNIT_STATE::ATTACK_STATE;
        return;
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
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        state = UNIT_STATE::ATTACK_STATE;
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
    //ĳ���� �̵�
    MoveCharacter();

    ShowAnimation(UNIT_STATE::WALKING_STATE);//�ִϸ��̼� ó��
}

void Character::HandleAttackState()
{
    
    //���ݻ��¿����� �ƹ�Ű�� ���� �ʴ´�.
   if(KeyManager::GetSingleton()->IsKeyUp(VK_CONTROL) && attackTimer == 0.0f)
    {
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }
    //TODO ���� ������ �� ǥâ ����� ������ ����, ��ų�� FILE���� �ҷ����� ó��, ��ų ������ �޾ƿͼ� �������ֱ�
    ShowAnimation(UNIT_STATE::ATTACK_STATE);//�ִϸ��̼� ó��
}

void Character::HandleJumpingState()
{
    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        state = UNIT_STATE::HANGING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LEFT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
        MoveCharacter();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        MoveCharacter();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_CONTROL))
    {
        state = UNIT_STATE::JUMPING_ATTACK_STATE;
        return;
    }

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//ó�� ������ ���� ��
    {
        //d = v * t; ������ �
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
    }

    ShowAnimation(UNIT_STATE::JUMPING_STATE);//�ִϸ��̼� ó��

    //CalcParabolicMotion();
}

//���� �� ����
void Character::HandleJumpingAttackState()
{
    if (KeyManager::GetSingleton()->IsKeyUp(VK_CONTROL))
    {
        state == UNIT_STATE::JUMPING_STATE;
        return;
    }

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//ó�� ������ ���� ��
    {
        //d = v * t; ������ �
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
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
        moveDirection = MOVE_DIRECTION::MOVE_LEFT;
        state = UNIT_STATE::WALKING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_RIGHT))
    {
        moveDirection = MOVE_DIRECTION::MOVE_RIGHT;
        state = UNIT_STATE::WALKING_STATE;
        return;
    }
    else if (KeyManager::GetSingleton()->IsKeyUp(VK_DOWN))
    {
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

    if (KeyManager::GetSingleton()->IsStayKeyDown(VK_UP))
    {
        moveDirection = MOVE_DIRECTION::MOVE_UP;
        state = UNIT_STATE::HANGING_MOVE_STATE;
        return;
    }
    else if(KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        moveDirection = MOVE_DIRECTION::MOVE_DOWN;
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
        MoveCharacter();
    }
    else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_DOWN))
    {
        moveDirection = MOVE_DIRECTION::MOVE_DOWN;
        MoveCharacter();
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

//ĳ���� �̵�
void Character::MoveCharacter()
{
    //�÷��̾� �ȱ�
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
    else if (moveDirection == MOVE_DIRECTION::MOVE_UP)
    {
        center.y -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_DOWN)
    {
        center.y += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
}

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


    //TODO ���� ���� ������ ���� ����� �ٲٱ�
    //���� ������ ������ ��
    timer += TimerManager::GetSingleton()->GetElapsedTime();
    if (timer >= 0.2f)
    {
        if (nextFrameY == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1)
            && (frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_2) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_3)) )
        {
            frame.x++;
            if (frame.x >= image->GetVMaxFrameX(frame.y))
            {
                frame.x = 0;
                frame.y++;
                if (frame.y > static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_3))
                {
                    frame.y = static_cast<int>(CHARACTER_FRAME_Y::LEFT_ATTACK_1);
                }
            }
        }
        else if (nextFrameY == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1)
            && (frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_2) || frame.y == static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_3)))
        {
            frame.x++;
            if (frame.x >= image->GetVMaxFrameX(frame.y))
            {
                frame.x = 0;
                frame.y++;
                if (frame.y > static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_3))
                {
                    frame.y = static_cast<int>(CHARACTER_FRAME_Y::RIGHT_ATTACK_1);
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
            result = CHARACTER_FRAME_Y::LEFT_ATTACK_1;
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            result = CHARACTER_FRAME_Y::RIGHT_ATTACK_1;
        break;
    case UNIT_STATE::JUMPING_STATE:
        result = CHARACTER_FRAME_Y::JUMP;
        break;
    case UNIT_STATE::JUMPING_ATTACK_STATE:
        if (moveDirection == MOVE_DIRECTION::MOVE_LEFT)
            result = CHARACTER_FRAME_Y::LEFT_ATTACK_1;
        else if (moveDirection == MOVE_DIRECTION::MOVE_RIGHT)
            result = CHARACTER_FRAME_Y::RIGHT_ATTACK_1;
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
    default:
        break;
    }

    return result;
}
