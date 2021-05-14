#include "BattleFieldScene.h"
#include "ImageManager.h"
#include "Character.h"
#include "Image.h"

HRESULT BattleFieldScene::Init()
{
    character = new Character();
    character->Init();

    hBrush = CreateSolidBrush(RGB( 255, 0, 255 ));
    return S_OK;
}

void BattleFieldScene::Release()
{
    DeleteObject(hBrush);
}

void BattleFieldScene::Update()
{
    character->Update();
    CameraManager::GetSingleton()->Update(character);
}

void BattleFieldScene::Render(HDC hdc)
{
    CameraManager::GetSingleton()->FocusOnCharacter(hdc, character);

    if (character)
        character->Render(hdc);
}
