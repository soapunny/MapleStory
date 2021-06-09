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
	font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("�����ý��丮"));

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
	hThread = CreateThread(NULL, 0, CheckClickThread, (LPVOID)this, 0, &checkClickThreadId);//���ȿɼ�, ũ��, ���� ��ų �Լ�������, �Լ��� �Ű�����, ��������̵�
	CloseHandle(hThread);//�ڵ����� �����尡 �� ���� �Ϸ��� �� ���Ḧ �Ѵ�.
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
