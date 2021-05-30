#pragma once
#include "InGameScene.h"
#include "Unit.h"
#include "Managers.h"

void InGameScene::MoveSceneUsingPortal()
{
    if (character->GetPortalState() == PORTAL_STATE::USING)
    {
        int minPortalIdx = 0;
        float minPortalDistance = 9999.9f;
        double tmpDistance = 0.0;
        for (int i = 0; i < vPortalLoc->size(); i++)
        {
            tmpDistance = sqrt(pow((double)(*vPortalLoc)[i]->x - character->GetCenter().x, 2.0) + pow((double)(*vPortalLoc)[i]->y - character->GetCenter().y, 2.0));
            if (minPortalDistance > tmpDistance)
            {
                minPortalDistance = tmpDistance;
                minPortalIdx = i;
            }
        }

        switch (minPortalIdx)
        {
        case 0:
            SceneManager::GetSingleton()->ChangeScene("¸Ó½¬¸¾ÇÊµå", "·Îµù¾À_1");
            break;
        case 1:
            SceneManager::GetSingleton()->ChangeScene("Çì³×½Ã½º±¤Àå", "·Îµù¾À_1");
            break;
        default:
            break;
        }
        return;
    }
}

HRESULT InGameScene::Init()
{
    return E_NOTIMPL;
}

void InGameScene::Release()
{
}

void InGameScene::Update()
{
}

void InGameScene::Render(HDC hdc)
{
}
