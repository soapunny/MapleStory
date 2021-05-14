#include "Button.h"
#include "Image.h"

/*
	실습1. 버튼 이미지를 세이브, 로드에 맞게 수정
	실습2. 스테이지를 지정할 수 있는 버튼을 추가로 생성
	실습3. 추가된 버튼(스테이지 전환)이 정상 동작하도록 코드 구현
*/

HRESULT Button::Init(const char* fileName, int posX, int posY /* 버튼 기능 */)
{
	state = ButtonState::NONE;
	img = ImageManager::GetSingleton()->FindImage(fileName);
	if (img == nullptr)
	{
		string warningText = string(fileName) + "가 없다!!";
		MessageBox(g_hWnd, warningText.c_str(), "실패", MB_OK);
		return E_FAIL;
	}

	pos.x = posX;
	pos.y = posY;

	rc.left = pos.x - img->GetFrameWidth() / 2;
	rc.top = pos.y - img->GetFrameHeight() / 2;
	rc.right = rc.left + img->GetFrameWidth();
	rc.bottom = rc.top + img->GetFrameHeight();

	func = nullptr;// TilemapTool::Save;

	return S_OK;
}

HRESULT Button::Init()
{
	return E_NOTIMPL;
}

void Button::Release()
{
}

void Button::Update()
{
	if (PtInRect(&rc, g_ptMouse))
	{
		if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_LBUTTON))
		{
			state = ButtonState::DOWN;
		}
		else if (state == ButtonState::DOWN &&
			KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			state = ButtonState::UP;

			// 버튼 기능 수행 : 세이브, 로드
			func(func_arg);
		}
	}
	else
	{
		state = ButtonState::NONE;
	}
}

void Button::Render(HDC hdc)
{
	switch (state)
	{
	case ButtonState::NONE:
	case ButtonState::UP:
		img->FrameRender(hdc, pos.x, pos.y, 0, 0, true);
		break;
	case ButtonState::DOWN:
		img->FrameRender(hdc, pos.x, pos.y, 0, 1, true);
		break;
	}
}
