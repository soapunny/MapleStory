#include "FirstTownScene.h"
#include "ImageManager.h"
#include "Character.h"
#include "Image.h"


HRESULT FirstTownScene::Init()
{
    character = new Character();
    character->Init();

    character->SetCenter(FPOINT{5000.0f, 500.0f});
   
    map = ImageManager::GetSingleton()->FindImage("Çì³×½Ã½º±¤Àå");
    minimapUI = ImageManager::GetSingleton()->FindImage("miniMapUI_Çì³×½Ã½º±¤Àå");
    minimap = ImageManager::GetSingleton()->FindImage("Çì³×½Ã½º±¤Àå_minimap");

    CameraManager::GetSingleton()->Init("Çì³×½Ã½º±¤Àå");
    Sleep(2000);

    hBrush = CreateSolidBrush(RGB(0, 255, 0));
    return S_OK;
}

void FirstTownScene::Release()
{
    DeleteObject(hBrush);
}

void FirstTownScene::Update()
{
    if (KeyManager::GetSingleton()->IsOnceKeyDown('S'))
    {
        SceneManager::GetSingleton()->ChangeScene("¸Ó½¬¸¾ÇÊµå", "·Îµù¾À_1");
    }
    character->Update();
    CollisionManager::GetSingleton()->Update();
    CameraManager::GetSingleton()->Update(character);
}

void FirstTownScene::Render(HDC hdc)
{
    CameraManager::GetSingleton()->FocusOnCharacter(hdc, character);
    RenderMiniMap(hdc);

    if (character)
        character->Render(hdc);
}

void FirstTownScene::RenderMiniMap(HDC hdc)
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