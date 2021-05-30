#pragma once
#include "FirstTownScene.h"
#include "Character.h"
#include "Managers.h"
#include "NPCManager.h"
#include "Image.h"


void FirstTownScene::MoveSceneUsingPortal()
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
            SceneManager::GetSingleton()->ChangeScene("¸Ó½¬¸¾ÇÊµå", "·Îµù¾À_1");
            break;
        default:
            break;
        }
        return;
    }
}

HRESULT FirstTownScene::Init()
{
    character = new Character();
    character->Init();

    character->SetCenter(FPOINT{5000.0f, 500.0f});

    npcManager = new NPCManager();
    npcManager->Init();

    //Portal À§Ä¡
   vPortalLoc = new vector<POINT*>;
    vPortalLoc->resize(2);
    (*vPortalLoc)[0] = new POINT{ 180, 800 };
    (*vPortalLoc)[1] = new POINT{ 1450, 750 };

    CameraManager::GetSingleton()->Init("Çì³×½Ã½º±¤Àå", "Çì³×½Ã½º±¤Àå_minimap", "miniMapUI_Çì³×½Ã½º±¤Àå", character, nullptr, npcManager, vPortalLoc);
    Sleep(2000);

    return S_OK;
}

void FirstTownScene::Release()
{
    SAFE_RELEASE(character);
    SAFE_RELEASE(npcManager);

    if (!vPortalLoc)
        return;
    for (POINT* portalPos : *vPortalLoc)
    {
        SAFE_DELETE(portalPos);
    }
    vPortalLoc->clear();
    SAFE_DELETE(vPortalLoc);
}

void FirstTownScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("¸Ó½¬¸¾ÇÊµå", "·Îµù¾À_1");
        return;
    }
    character->Update();
    CollisionManager::GetSingleton()->Update();
    CameraManager::GetSingleton()->Update();
    //portal ¾À ÀÌµ¿
    MoveSceneUsingPortal();
}

void FirstTownScene::Render(HDC hdc)
{
    CameraManager::GetSingleton()->FocusOnCharacter(hdc);
    CollisionManager::GetSingleton()->Render(hdc);
}