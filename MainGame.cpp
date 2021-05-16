#include "MainGame.h"
#include "Image.h"
#include "LoadingScene.h"
#include "MushroomFieldScene.h"
#include "FirstTownScene.h"

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	FileManager::GetSingleton()->Init();
	CollisionManager::GetSingleton()->Init();


	// ���ΰ����� �ʱ�ȭ �Լ�
	//hTimer = (HANDLE)SetTimer(g_hWnd, 0, 1, NULL);

	// ����� �̹���
	int maxWidth, maxHeight;
	maxWidth = max(WINSIZE_X, TILEMAPTOOLSIZE_X);
	maxHeight = max(WINSIZE_Y, TILEMAPTOOLSIZE_Y);

	backBuffer = new Image();
	backBuffer->Init(maxWidth, maxHeight);
	//�̹���
	ImageManager::GetSingleton()->AddImage("�ε�ȭ��_��ũ��", "Image/LoadingScene/��ũ��.bmp", 9597, 331, 21, 1);
	ImageManager::GetSingleton()->AddImage("Character", "Image/Character/player.bmp", 315, 1099, 4, vector<int>{2,4,4,3,3,3,3,3,3,2,2,2,2}, 13, true, RGB(255, 0, 255));
	ImageManager::GetSingleton()->AddImage("�ӽ����ʵ�", "Image/Map/�ӽ����ʵ�_2520_1080.bmp", "Image/Map/�ӽ����ʵ�_2520_1080_tile_collision.bmp", 2520, 1080);
	ImageManager::GetSingleton()->AddImage("miniMapUI", "Image/UI/miniMapUI_�ӽ����ʵ�.bmp", 300, 50);
	ImageManager::GetSingleton()->AddImage("MushroomHill_minimap", "Image/Map/�ӽ����ʵ�_minimap.bmp", 300, 128);
	ImageManager::GetSingleton()->AddImage("��׽ý�����", "Image/Map/��׽ý�����.bmp", "Image/Map/��׽ý�����_tile_collision.bmp", 6850, 1654);
	ImageManager::GetSingleton()->AddImage("miniMapUI_��׽ý�����", "Image/UI/miniMapUI_��׽ý�����.bmp", 300, 50);
	ImageManager::GetSingleton()->AddImage("��׽ý�����_minimap", "Image/Map/��׽ý�����_minimap.bmp", 300, 72);

	//��
	SceneManager::GetSingleton()->AddScene("�ӽ����ʵ�", new MushroomFieldScene());
	SceneManager::GetSingleton()->AddScene("��׽ý�����", new FirstTownScene());
	SceneManager::GetSingleton()->AddLoadingScene("�ε���_1", new LoadingScene());

	SceneManager::GetSingleton()->ChangeScene("�ӽ����ʵ�");


	isInited = true;

	return S_OK;
}

void MainGame::Release()
{
	KeyManager::GetSingleton()->Release();
	ImageManager::GetSingleton()->Release();
	SceneManager::GetSingleton()->Release();
	CollisionManager::GetSingleton()->Release();

	SAFE_RELEASE(backBuffer);

	ReleaseDC(g_hWnd, hdc);
}

void MainGame::Update()
{
	SceneManager::GetSingleton()->Update();
}

void MainGame::Render()
{
	HDC hBackDC = backBuffer->GetMemDC();

	SceneManager::GetSingleton()->Render(hBackDC);

	// �λ�
	//TextOut(hBackDC, 20, 20, "MainGame ���� ��", strlen("MainGame ���� ��"));
	// ���콺 ��ǥ
	//wsprintf(szText, "X : %d, Y : %d", g_ptMouse.x, g_ptMouse.y);
	//TextOut(hBackDC, 200, 20, szText, strlen(szText));
	// FPS
	TimerManager::GetSingleton()->Render(hBackDC);

	backBuffer->Render(hdc);
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
	isInited = false;
}

MainGame::~MainGame()
{
}
