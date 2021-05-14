#include "LoadingScene.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	bg = ImageManager::GetSingleton()->FindImage("로딩화면1");
	return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
}

void LoadingScene::Render(HDC hdc)
{
	if (bg)
	{
		bg->Render(hdc);
	}
}
