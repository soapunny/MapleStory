#include "CollisionManager.h"
#include "Character.h"
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
	return S_OK;
}

void CollisionManager::Update()
{
    //Player Pixel Collision
    CheckPlayerBottomPixelCollision();
    CheckPlayerSidePixelCollision(); 
    CheckPlayerRopePixelCollision();

    //Enemy Pixel Collision
    CheckEnemiesBottomPixelCollision();
}

void CollisionManager::Render()
{
}

void CollisionManager::Release()
{
}


void CollisionManager::CheckPlayerBottomPixelCollision()
{
    if (player->GetUnitState() == UNIT_STATE::HANGING_STATE)
        return;
    // 픽셀 충돌 확인
    if (player->GetJumpingState() != JUMPING_STATE::JUMPING_UP)
    {
        int R = 0;
        int G = 0;
        int B = 0;
        float currPosY = player->GetCenter().y;
        for (int i = (int)(currPosY - 1.0f); i <= (int)(currPosY + 1.0f); i++)
        {
            for (int j = (int)(player->GetCenter().x - 35.0f); j <= (int)(player->GetCenter().x - 30.0f); j++)
            {
                COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, i);//발 아래부분 좌표와 배경의 픽셀충돌

                R = GetRValue(color);
                G = GetGValue(color);
                B = GetBValue(color);

                if ((R == 255 && G == 0 && B == 255) || (R == 0 && G == 0 && B == 0 && !player->GetJumpingDown()))//착륙했을 때
                {
                    //위치 고정
                    player->SetCenterY(i);
                    if (player->GetJumpingState() == JUMPING_STATE::JUMPING_DOWN) {
                        player->SetJumpingState(JUMPING_STATE::JUST_LANDED);
                        return;
                    }   
                    else
                    {
                        player->SetJumpingState(JUMPING_STATE::END_OF_JUMPING_STATE);
                        return;
                    }
                }
            }
        }

        //바닥이 아닌경우
        player->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
        return;
    }
    player->SetJumpingState(JUMPING_STATE::JUMPING_UP);
    return;
}

void CollisionManager::CheckPlayerSidePixelCollision()
{
    if (player->GetJumpingState() == JUMPING_STATE::END_OF_JUMPING_STATE)
    {
        int R = 0;
        int G = 0;
        int B = 0;

        for (int j = (int)(player->GetCenter().x - 50.0f); j <= (int)(player->GetCenter().x - 45.0f); j++)
        {
            COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, player->GetCenter().y);//발 아래부분 좌표와 배경의 픽셀충돌

            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if ((R == 50 && G == 50 && B == 50))
            {
                //위치 고정
                player->SetBlockedState(BLOCKED_STATE::LEFT);
                return;
            }
        }

        for (int j = (int)(player->GetCenter().x - 30.0f); j <= (int)(player->GetCenter().x - 25.0f); j++)
        {
            COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, player->GetCenter().y);//발 아래부분 좌표와 배경의 픽셀충돌

            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if ((R == 50 && G == 50 && B == 50))
            {
                //위치 고정
                player->SetBlockedState(BLOCKED_STATE::RIGHT);
                return;
            }
        }
    }
    player->SetBlockedState(BLOCKED_STATE::END_OF_BLOCKED_STATE);
}

void CollisionManager::CheckPlayerRopePixelCollision()
{
    if(player->GetUnitState() == UNIT_STATE::HANGING_STATE || player->GetUnitState() == UNIT_STATE::HANGING_MOVE_STATE)
    { 
        int R = 0;
        int G = 0;
        int B = 0;
        float currPosY = player->GetCenter().y;
        for (int i = (int)(currPosY - 1.0f); i <= (int)(currPosY + 1.0f); i++)
        {
            for (int j = (int)(player->GetCenter().x - 35.0f); j <= (int)(player->GetCenter().x - 30.0f); j++)
            {
                COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, i);//발 아래부분 좌표와 배경의 픽셀충돌

                R = GetRValue(color);
                G = GetGValue(color);
                B = GetBValue(color);

                if (R == 0 && G == 255 && B == 255)//로프에 닿았을 때
                {
                    //위치 고정
                    //player->SetCenterX(j);
                    player->SetJumpingState(JUMPING_STATE::END_OF_JUMPING_STATE);
                    return;
                }
                else if (R == 0 && G == 0 && B == 0)
                {
                    player->SetJumpingState(JUMPING_STATE::JUST_LANDED);
                    return;
                }
            }
        }
        if (player->GetJumpingState() == JUMPING_STATE::END_OF_JUMPING_STATE){
            player->SetJumpingState(JUMPING_STATE::JUMPING_DOWN);
            player->SetState(UNIT_STATE::JUMPING_STATE);
            player->SetCenterY(player->GetCenter().y - 30);
        }
    }
}


void CollisionManager::CheckEnemiesBottomPixelCollision()
{

}
void CollisionManager::CheckEnemiesSidePixelCollision()
{
}
