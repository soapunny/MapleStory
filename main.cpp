// main.cpp
#include "config.h"
#include "Managers.h"
#include "CommonFunction.h"
#include "MainGame.h"

// ��������
// �ν��Ͻ� : ��ü �޸𸮻� �����ϴ� ��ü
HINSTANCE g_hInstance;		// ���α׷� �ν��Ͻ� �ڵ�
HWND g_hWnd;				// ������ �ڵ�
LPSTR g_lpszClass = (LPSTR)TEXT("MapleStory");
POINT g_ptMouse;

MainGame g_mainGame;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//void SetClientRect(HWND hWnd, int width, int height);

int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpszCmdParam, int nCmdShow)
{
	g_hInstance = _hInstance;
	float timer = 0.0f;

	// �����츦 �����ϱ� ���� �⺻����
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);

	wndClass.cbClsExtra = 512;
	wndClass.cbWndExtra = 512;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = g_hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = g_lpszClass;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.hIconSm = LoadIcon(wndClass.hInstance, MAKEINTRESOURCE(IDI_SHIELD));


	//WNDCLASS wndClass;
	//wndClass.cbClsExtra = 2068;
	//wndClass.cbWndExtra = 2068;
	//wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wndClass.hInstance = g_hInstance;
	//wndClass.lpfnWndProc = WndProc;
	//wndClass.lpszClassName = g_lpszClass;
	//wndClass.lpszMenuName = NULL;
	//wndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ���
	RegisterClassEx(&wndClass);
	//RegisterClass(&wndClass);

	// ������ ����
	g_hWnd = CreateWindowEx(WS_EX_TOOLWINDOW, g_lpszClass, g_lpszClass,
		WS_OVERLAPPEDWINDOW,
		50, 50, WINSIZE_X, WINSIZE_Y,
		NULL, NULL, g_hInstance, NULL);

	//g_hWnd = CreateWindow(g_lpszClass, g_lpszClass, 
	//	WS_OVERLAPPEDWINDOW,
	//	50, 50, 1080, 720, 
	//	NULL, NULL, g_hInstance, NULL);

	// ������ ���
	ShowWindow(g_hWnd, nCmdShow);

	// ������ ũ�� ����
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	if (FAILED(g_mainGame.Init()))
	{
		MessageBox(g_hWnd, "���� ���� �ʱ�ȭ ����!!", "���", MB_OK);
		//return 0;
	}

	TimerManager::GetSingleton()->Init();

	// �޽��� ť�� �ִ� �޽��� ó��
	MSG message;
	while (true)
	{
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)	break;

			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TimerManager::GetSingleton()->Update();
			g_mainGame.Update();
			g_mainGame.Render();
			timer = 0.0f;
		}

	}

	//while (GetMessage(&message, 0, 0, 0))
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	g_mainGame.Release();

	return message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return g_mainGame.MainProc(hWnd, iMessage, wParam, lParam);
}