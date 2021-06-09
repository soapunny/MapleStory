#pragma once
#include "FirstTownScene.h"
#include "Character.h"
#include "Managers.h"
#include "NPCManager.h"
#include "NPCStorage.h"
#include "NPCEntity.h"
#include "CharDataTellerMachine.h"
#include "CharacterDTO.h"
#include "Map.h"
#include "CSound.h"
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
            tmpDistance = sqrt(pow((double)(*vPortalLoc)[i]->x - character->GetWorldPos().x, 2.0) + pow((double)(*vPortalLoc)[i]->y - character->GetWorldPos().y, 2.0));
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
    m_bgm = new CSound("Sound/Henesis/FloralLife.mp3", false);
    m_bgm->play();

    charDataTellerMachine = new CharDataTellerMachine();
    mCharacterData = charDataTellerMachine->LoadCharacter("¶Ñºñ");
    CharacterDTO* myCharacterData = (mCharacterData->find("¶Ñºñ"))->second;
    character = new Character();
    ((Character*)character)->SetCharacterDTO(myCharacterData);
    character->Init();
    
    if(myCharacterData->GetCurrentMap() == MAP_NAME::MUSHROOM_HILL){
        character->SetWorldPos(FPOINT{ 245, 820 });
    }
    else if (myCharacterData->GetCurrentMap() == MAP_NAME::HENESIS_SQUARE)
    {
        //Nothing
    }

    myCharacterData->SetCurrentMap(MAP_NAME::HENESIS_SQUARE);


    npcManager = new NPCManager();
    npcManager->Init();

    //Portal À§Ä¡
   vPortalLoc = new vector<POINT*>;
    vPortalLoc->resize(2);
    (*vPortalLoc)[0] = new POINT{ 210, 765 };
    (*vPortalLoc)[1] = new POINT{ 1485, 725 };

    NPCStorage::GetSingleton()->LoadLocalNPC(MAP_NAME::HENESIS_SQUARE);
    auto mNPCStorage = NPCStorage::GetSingleton()->GetMNPCStorage();
    for (auto npcEntity : *mNPCStorage)
    {
        if (npcEntity.second)
        {
            npcEntity.second->SetImage(ImageManager::GetSingleton()->FindImage(npcEntity.second->GetNPCName()));
        }
    }
    CameraManager::GetSingleton()->Init("Çì³×½Ã½º±¤Àå", "Çì³×½Ã½º±¤Àå_minimap", "miniMapUI_Çì³×½Ã½º±¤Àå", character, nullptr, npcManager, vPortalLoc);
    Sleep(2000);

    return S_OK;
}

void FirstTownScene::Release()
{
    if (charDataTellerMachine)
    {
        charDataTellerMachine->SaveCharacter(((Character*)character)->GetCharacterDTO());
        SAFE_RELEASE(charDataTellerMachine);
    }
    SAFE_RELEASE(character);
    SAFE_RELEASE(npcManager);
    NPCStorage::GetSingleton()->Release();
    if (!vPortalLoc)
        return;
    for (POINT* portalPos : *vPortalLoc)
    {
        SAFE_DELETE(portalPos);
    }
    vPortalLoc->clear();
    SAFE_DELETE(vPortalLoc);
    
    SAFE_DELETE(m_bgm);
}

void FirstTownScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("¸Ó½¬¸¾ÇÊµå", "·Îµù¾À_1");
        return;
    }
    m_bgm->Update();
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