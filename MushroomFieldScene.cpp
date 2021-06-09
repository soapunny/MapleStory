#pragma once
#include "MushroomFieldScene.h"
#include "Character.h"
#include "Managers.h"
#include "MonsterManager.h"
#include "NPCStorage.h"
#include "NPCEntity.h"
#include "CharDataTellerMachine.h"
#include "CharacterDTO.h"
#include "Map.h"
#include "CSound.h"
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
    //Sound ³Ö±â
    m_bgm = new CSound("Sound/Henesis/RestNPeace.mp3", false);
    m_bgm->play();
    Sleep(2000);

    charDataTellerMachine = new CharDataTellerMachine();
    mCharacterData = charDataTellerMachine->LoadCharacter("¶Ñºñ");
    CharacterDTO* myCharacterData = (mCharacterData->find("¶Ñºñ"))->second;
    character = new Character();
    ((Character*)character)->SetCharacterDTO(myCharacterData);
    character->Init();

    if (myCharacterData->GetCurrentMap() == MAP_NAME::HENESIS_SQUARE)
    {
        character->SetWorldPos(FPOINT{ 2360, 760 });
    }
    else if(myCharacterData->GetCurrentMap() == MAP_NAME::MUSHROOM_HILL)
    {
        //Nothing
    }
    myCharacterData->SetCurrentMap(MAP_NAME::MUSHROOM_HILL);

    monsterManager = new MonsterManager;
    monsterManager->Init();

    //Æ÷Å» À§Ä¡ ¼³Á¤
    vPortalLoc = new vector<POINT*>;
    vPortalLoc->resize(2);
    (*vPortalLoc)[0] = new POINT{ 215, 860 };
    (*vPortalLoc)[1] = new POINT{ 2320, 750 };

    NPCStorage::GetSingleton()->LoadLocalNPC(MAP_NAME::MUSHROOM_HILL);
    auto mNPCStorage = NPCStorage::GetSingleton()->GetMNPCStorage();
    for (auto npcEntity : *mNPCStorage)
    {
        if (npcEntity.second)
        {
            npcEntity.second->SetImage(ImageManager::GetSingleton()->FindImage(npcEntity.second->GetNPCName()));
        }
    }
    CameraManager::GetSingleton()->Init("¸Ó½¬¸¾ÇÊµå", "MushroomHill_minimap", "miniMapUI", character, monsterManager, nullptr, vPortalLoc);

    return S_OK;
}

void MushroomFieldScene::Release()
{
    if (charDataTellerMachine)
    {
        charDataTellerMachine->SaveCharacter(((Character*)character)->GetCharacterDTO());
        SAFE_RELEASE(charDataTellerMachine);
    }
    SAFE_RELEASE(character);
    SAFE_RELEASE(monsterManager);
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

void MushroomFieldScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("Çì³×½Ã½º±¤Àå", "·Îµù¾À_1");
        return;
    }
    m_bgm->Update();
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