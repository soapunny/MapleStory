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

    //캐릭터를 처음 시작 했을 때 낙하하게 세팅
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

//TODO 점프 매끄럽게 잘 되게 수정하기
//TODO 모든 실행에 elapsedTime을 곱해서 프레임에 관계없이 잘 실행되게 수정
void Character::Update()
{

    //점프상태
    if (jumpingState == JUMPING_STATE::JUMPING_UP)
    {
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = center.x + VELOCITY * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);// 점프타이머(-0.5*GA*점프타이머 + VELOCITY * sin(PI/2.0f))
        
        if (jumpTimer >= VELOCITY * sin(PI / 2.0f) / (0.5f * GA) / 2.0f)//낙하시간대로 들어서면
        {
            jumpingState = JUMPING_STATE::JUMPING_DOWN;
        }
    }
    else if (jumpingState == JUMPING_STATE::JUMPING_DOWN)
    {
        //점프가 아니라 위에서 갑자기 뚝 떨어질 경우
        if (jumpTimer < VELOCITY * sin(PI / 2.0f) / (0.5f * GA) / 2.0f) {//낙하시간이 아니라 이륙시간대이면
            jumpTimer = VELOCITY * sin(PI / 2.0f / (0.5f * GA) / 2.0f);//낙하 시간대으로 바꿔주고
            //현재 위치에서 포물선의 최고점 위치 만큼 빼준다. -> 아래식서 현재위치가 포물선의 최고점이되게
            priorPosY = center.y + (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
        }
        jumpTimer += TimerManager::GetSingleton()->GetElapsedTime()*10;
        center.x = center.x + VELOCITY * cos(PI / 2.0f) * jumpTimer;
        center.y = priorPosY - (VELOCITY * sin(PI / 2.0f) * jumpTimer - 0.5f * GA * jumpTimer * jumpTimer);
    }
    else if(jumpingState == JUMPING_STATE::JUST_LANDED)//방금 착지했을 때
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

    //동전 줍기는 상태에 상관없이 항상 가능하다.
    if (KeyManager::GetSingleton()->IsStayKeyDown('Z'))
    {
        //TODO 동전 줍기
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
    //캐릭터 이동
    MoveCharacter();

    ShowAnimation(UNIT_STATE::WALKING_STATE);//애니메이션 처리
}

void Character::HandleAttackState()
{
    
    //공격상태에서는 아무키도 먹지 않는다.
   if(KeyManager::GetSingleton()->IsKeyUp(VK_CONTROL) && attackTimer == 0.0f)
    {
        state = UNIT_STATE::DEFAULT_STATE;
        return;
    }
    //TODO 공격 상태일 때 표창 제대로 나가게 구현, 스킬을 FILE에서 불러오게 처리, 스킬 딜레이 받아와서 딜레이주기
    ShowAnimation(UNIT_STATE::ATTACK_STATE);//애니메이션 처리
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

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//처음 점프를 했을 때
    {
        //d = v * t; 포물선 운동
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
    }

    ShowAnimation(UNIT_STATE::JUMPING_STATE);//애니메이션 처리

    //CalcParabolicMotion();
}

//점프 중 공격
void Character::HandleJumpingAttackState()
{
    if (KeyManager::GetSingleton()->IsKeyUp(VK_CONTROL))
    {
        state == UNIT_STATE::JUMPING_STATE;
        return;
    }

    if (jumpingState == JUMPING_STATE::END_OF_JUMPING_STATE)//처음 점프를 했을 때
    {
        //d = v * t; 포물선 운동
        jumpingState = JUMPING_STATE::JUMPING_UP;
        priorPosY = center.y;
        jumpTimer = 0.0f;
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

//캐릭터 이동
void Character::MoveCharacter()
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
    else if (moveDirection == MOVE_DIRECTION::MOVE_UP)
    {
        center.y -= moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
    else if (moveDirection == MOVE_DIRECTION::MOVE_DOWN)
    {
        center.y += moveSpeed * TimerManager::GetSingleton()->GetElapsedTime();
    }
}

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


    //TODO 상태 변경 나누는 기준 제대로 바꾸기
    //상태 변경이 없었을 때
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
        //이미지가 존재하지 않음 TODO 나중에 이미지 수정 요망
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
