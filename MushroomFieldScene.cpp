#pragma once
#include "MushroomFieldScene.h"
#include "Character.h"
#include "Managers.h"
#include "MonsterManager.h"
#include "Image.h"


void MushroomFieldScene::MoveSceneUsingPortal()
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
            SceneManager::GetSingleton()->ChangeScene("Çì³×½Ã½º±¤Àå", "·Îµù¾À_1");
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

HRESULT MushroomFieldScene::Init()
{
    character = new Character();
    character->Init();

    monsterManager = new MonsterManager;
    monsterManager->Init();

    Sleep(2000);

    //Æ÷Å» À§Ä¡ ¼³Á¤
    vPortalLoc = new vector<POINT*>;
    vPortalLoc->resize(2);
    (*vPortalLoc)[0] = new POINT{ 180, 880 };
    (*vPortalLoc)[1] = new POINT{ 2300, 780 };

    CameraManager::GetSingleton()->Init("¸Ó½¬¸¾ÇÊµå", "MushroomHill_minimap", "miniMapUI", character, monsterManager, nullptr, vPortalLoc);

    return S_OK;
}

void MushroomFieldScene::Release()
{
    SAFE_RELEASE(character);
    SAFE_RELEASE(monsterManager);

    if (!vPortalLoc)
        return;
    for (POINT* portalPos : *vPortalLoc)
    {
        SAFE_DELETE(portalPos);
    }
    vPortalLoc->clear();
    SAFE_DELETE(vPortalLoc);
}

void MushroomFieldScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("Çì³×½Ã½º±¤Àå", "·Îµù¾À_1");
        return;
    }

    monsterManager->Update();
    character->Update();
    CollisionManager::GetSingleton()->Update();
    CameraManager::GetSingleton()->Update();

    //portal ¾À ÀÌµ¿
    MoveSceneUsingPortal();
}

void MushroomFieldScene::Render(HDC hdc)
{
    CameraManager::GetSingleton()->FocusOnCharacter(hdc);
    CollisionManager::GetSingleton()->Render(hdc);
}