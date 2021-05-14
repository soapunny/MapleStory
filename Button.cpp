#include "Button.h"
#include "Image.h"

/*
	�ǽ�1. ��ư �̹����� ���̺�, �ε忡 �°� ����
	�ǽ�2. ���������� ������ �� �ִ� ��ư�� �߰��� ����
	�ǽ�3. �߰��� ��ư(�������� ��ȯ)�� ���� �����ϵ��� �ڵ� ����
*/

HRESULT Button::Init(const char* fileName, int posX, int posY /* ��ư ��� */)
{
	state = ButtonState::NONE;
	img = ImageManager::GetSingleton()->FindImage(fileName);
	if (img == nullptr)
	{
		string warningText = string(fileName) + "�� ����!!";
		MessageBox(g_hWnd, warningText.c_str(), "����", MB_OK);
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

			// ��ư ��� ���� : ���̺�, �ε�
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
