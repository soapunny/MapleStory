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
    // �ȼ� �浹 Ȯ��
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
                COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, i);//�� �Ʒ��κ� ��ǥ�� ����� �ȼ��浹

                R = GetRValue(color);
                G = GetGValue(color);
                B = GetBValue(color);

                if ((R == 255 && G == 0 && B == 255) || (R == 0 && G == 0 && B == 0 && !player->GetJumpingDown()))//�������� ��
                {
                    //��ġ ����
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

        //�ٴ��� �ƴѰ��
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
            COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, player->GetCenter().y);//�� �Ʒ��κ� ��ǥ�� ����� �ȼ��浹

            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if ((R == 50 && G == 50 && B == 50))
            {
                //��ġ ����
                player->SetBlockedState(BLOCKED_STATE::LEFT);
                return;
            }
        }

        for (int j = (int)(player->GetCenter().x - 30.0f); j <= (int)(player->GetCenter().x - 25.0f); j++)
        {
            COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, player->GetCenter().y);//�� �Ʒ��κ� ��ǥ�� ����� �ȼ��浹

            R = GetRValue(color);
            G = GetGValue(color);
            B = GetBValue(color);

            if ((R == 50 && G == 50 && B == 50))
            {
                //��ġ ����
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
                COLORREF color = GetPixel(CameraManager::GetSingleton()->GetBG()->GetSubDC(), j, i);//�� �Ʒ��κ� ��ǥ�� ����� �ȼ��浹

                R = GetRValue(color);
                G = GetGValue(color);
                B = GetBValue(color);

                if (R == 0 && G == 255 && B == 255)//������ ����� ��
                {
                    //��ġ ����
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
