#pragma once
#include "GameNode.h"

class Image;
class MainGame : public GameNode
{
private:
	bool isInited;
	HDC hdc;
	char szText[128] = "";

	Image* backBuffer;

public:
	HRESULT Init();		// �������̵� : ������
	void Release();
	void Update();
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();
};

