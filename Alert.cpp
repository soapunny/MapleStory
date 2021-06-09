#include "Alert.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Image.h"

DWORD CALLBACK CheckClickThread(LPVOID pvParam)
{
	Alert* alert = ((Alert*)pvParam);
	RECT rect = RECT{ alert->GetConfirmRect().left, alert->GetConfirmRect().top, alert->GetConfirmRect().right, alert->GetConfirmRect().bottom };
	while (true)
	{
		if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&rect, g_ptMouse) )
			{
				alert->SetOn(false);
				break;
			}
		}
	}

	return 0;
}

HRESULT Alert::Init()
{
	Init("", true);

	return S_OK;
}

HRESULT Alert::Init(string content, bool isOn)
{
	image = ImageManager::GetSingleton()->AddImage("Alert", "Image/UI/Alert.bmp", 187, 80, true, RGB(255, 0, 255));
	this->content = content;
	this->pos.x = WINSIZE_X/2.0f;
	this->pos.y = WINSIZE_Y/2.0f;
	this->isOn = isOn;
	renderRatio = 1.5f;
	width = image->GetWidth() * renderRatio;
	height = image->GetHeight() * renderRatio;

	SetRect(&confirmRect, 
			pos.x + (153.0f - image->GetWidth()/2.0f) * renderRatio,
			pos.y + (61.0f - image->GetHeight()/2.0f) * renderRatio,
			pos.x + (179.0f - image->GetWidth()/2.0f) * renderRatio,
			pos.y + (72.0f - image->GetHeight() / 2.0f) * renderRatio
			);
	font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));

	if (isOn)
		CheckClick();

	return S_OK;
}

void Alert::Release()
{
	if(font)
		DeleteObject(font);
}

void Alert::CheckClick()
{
	if (!isOn)
		return;

	DWORD checkClickThreadId = 0;
	HANDLE hThread = NULL;
	hThread = CreateThread(NULL, 0, CheckClickThread, (LPVOID)this, 0, &checkClickThreadId);//보안옵션, 크기, 실행 시킬 함수포인터, 함수의 매개변수, 쓰레드아이디
	CloseHandle(hThread);//자동으로 쓰레드가 할 일을 완료한 후 종료를 한다.
}

void Alert::Render(HDC hdc)
{
	if (!isOn)
		return;

	image->Render(hdc, pos.x, pos.y, true, renderRatio);
	HFONT oldFont = (HFONT)SelectObject(hdc, font);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, pos.x - 7 * content.size()/2, pos.y - 30, content.c_str(), strlen(content.c_str()));
}
