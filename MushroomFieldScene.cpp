#include "MushroomFieldScene.h"
#include "ImageManager.h"
#include "Character.h"
#include "Image.h"


HRESULT MushroomFieldScene::Init()
{
    character = new Character();
    character->Init();

    map = ImageManager::GetSingleton()->FindImage("¸Ó½¬¸¾ÇÊµå");
    minimapUI = ImageManager::GetSingleton()->FindImage("miniMapUI");
    minimap = ImageManager::GetSingleton()->FindImage("MushroomHill_minimap");

    CameraManager::GetSingleton()->Init("¸Ó½¬¸¾ÇÊµå");
    Sleep(2000);

    hBrush = CreateSolidBrush(RGB( 0, 255, 0));
    return S_OK;
}

void MushroomFieldScene::Release()
{
    DeleteObject(hBrush);
}

void MushroomFieldScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("Çì³×½Ã½º±¤Àå", "·Îµù¾À_1");
    }
    character->Update();
    CollisionManager::GetSingleton()->Update();
    CameraManager::GetSingleton()->Update(character);
}

void MushroomFieldScene::Render(HDC hdc)
{
    CameraManager::GetSingleton()->FocusOnCharacter(hdc, character);
    RenderMiniMap(hdc);

    if (character)
        character->Render(hdc);
}

void MushroomFieldScene::RenderMiniMap(HDC hdc)
{
    int startX = WINSIZE_X - minimap->GetWidth();
    
    Rectangle(hdc, startX, 0, WINSIZE_X, minimap->GetHeight() + 50);
    minimapUI->Render(hdc, startX, 0);
    minimap->Render(hdc, startX, 50);
    float minimapCharacterX = startX + character->GetCenter().x * minimap->GetWidth() / map->GetWidth();
    float minimapCharacterY = character->GetCenter().y * minimap->GetHeight() / map->GetHeight() + 50;
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, minimapCharacterX - 5, minimapCharacterY - 5, minimapCharacterX + 5, minimapCharacterY + 5);
}