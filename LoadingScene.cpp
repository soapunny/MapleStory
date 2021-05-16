#include "LoadingScene.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT LoadingScene::Init()
{
	bg = ImageManager::GetSingleton()->FindImage("로딩화면_핑크빈");
	frame.x = 0;
	frame.y = 0;

	timer = 0.0f;

	return S_OK;
}

void LoadingScene::Release()
{
}

void LoadingScene::Update()
{
	timer += TimerManager::GetSingleton()->GetElapsedTime();

	if(timer >= 0.2)
	{ 
		frame.x++;
		if (frame.x >= bg->GetMaxFrameX())
		{
			frame.x = 0;
		}
		timer = 0.0f;
	}
}

void LoadingScene::Render(HDC hdc)
{
	if (bg)
	{
		PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
		bg->FrameRender(hdc, (int)(WINSIZE_X/2.0f), (int)(WINSIZE_Y/2.0f), frame.x, frame.y, true);
	}
}
