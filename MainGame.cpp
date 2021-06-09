#pragma once
#include "MainGame.h"
#include "Image.h"
#include "LoadingScene.h"
#include "MushroomFieldScene.h"
#include "FirstTownScene.h"
#include "Managers.h"
#include "ItemStorage.h"
#include "CalcUtil.h"
#include "CSound.h"
#include <ctime>

HRESULT MainGame::Init()
{
	hdc = GetDC(g_hWnd);

	srand((unsigned int)time(NULL));

	KeyManager::GetSingleton()->Init();
	ImageManager::GetSingleton()->Init();
	SceneManager::GetSingleton()->Init();
	FileManager::GetSingleton()->Init();
	CollisionManager::GetSingleton()->Init();

	//FONT Resources 
	AddFontResourceA("MapleStory/Font/Maplestory Bold.ttf");
	AddFontResourceA("MapleStory/Font/Maplestory Light.ttf");
	AddFontResourceA("MapleStory/Font/Typo_BabyHeartB.ttf");
	AddFontResourceA("MapleStory/Font/Typo_BabyHeartL.ttf");
	AddFontResourceA("C:/Windows/Fonts/impact.ttf");

	//Sound
	CSound::Init();

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
	ImageManager::GetSingleton()->AddImage("NameBox", "Image/UI/NameBox.bmp", 161, 35, true, RGB(255, 255, 255));
	ImageManager::GetSingleton()->AddImage("Character", "Image/Character/player.bmp", 315, 1184, 4, vector<int>{2,4,4,3,3,3,3,3,3,2,2,2,2,4}, 14, true, RGB(255, 0, 255), 1.5f);
	ImageManager::GetSingleton()->AddImage("Luke", "Image/NPC/Luke.bmp", 300, 184, 4, vector<int>{4, 4}, 2, true, RGB(255, 0, 255), 1.5f);
	ImageManager::GetSingleton()->AddImage("Tory", "Image/NPC/Tory.bmp", 300, 182, 5, vector<int>{5, 5}, 2, true, RGB(255, 0, 255), 1.5f);
	ImageManager::GetSingleton()->AddImage("LevelUp", "Image/UI/levelUp.bmp", 9870, 250, 21, vector<int>{21}, 1, true, RGB(255, 255, 255), 1.0f);
	ImageManager::GetSingleton()->AddImage("Death", "Image/Character/death.bmp", 640, 200, 6, vector<int>{4, 4, 4, 1}, 4, true, RGB(255, 0, 255), 1.5f);
	ImageManager::GetSingleton()->AddImage("Mushmom", "Image/Monster/Mushmom.bmp", 1827, 1205, 10, vector<int>{2, 6, 10, 6, 2, 6, 10, 6}, 8, true, RGB(255, 0, 255), 2.0f);
	ImageManager::GetSingleton()->AddImage("Mushmom_Skill", "Image/Monster/MushmomSkill.bmp", 767, 125, 3, 1, true, RGB(255, 0, 255), 2.0f);
	ImageManager::GetSingleton()->AddImage("�ӽ����ʵ�", "Image/Map/�ӽ����ʵ�_2520_1080.bmp", "Image/Map/�ӽ����ʵ�_2520_1080_tile_collision.bmp", 2520, 1080);
	ImageManager::GetSingleton()->AddImage("miniMapUI", "Image/UI/miniMapUI_�ӽ����ʵ�.bmp", 300, 50);
	ImageManager::GetSingleton()->AddImage("DeathAlert", "Image/UI/DeathAlert.bmp", 277, 120);
	ImageManager::GetSingleton()->AddImage("MushroomHill_minimap", "Image/Map/�ӽ����ʵ�_minimap.bmp", 300, 128);
	ImageManager::GetSingleton()->AddImage("��׽ý�����", "Image/Map/��׽ý�����.bmp", "Image/Map/��׽ý�����_tile_collision.bmp", 6850, 1654);
	ImageManager::GetSingleton()->AddImage("miniMapUI_��׽ý�����", "Image/UI/miniMapUI_��׽ý�����.bmp", 300, 50);
	ImageManager::GetSingleton()->AddImage("��׽ý�����_minimap", "Image/Map/��׽ý�����_minimap.bmp", 300, 72);
	ImageManager::GetSingleton()->AddImage("BarUI", "Image/UI/BarUI.bmp", 172, 66, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("MonsterBarUI", "Image/UI/MonsterBarUI_2.bmp", 143, 15, true, RGB(255, 255, 255));
	ImageManager::GetSingleton()->AddImage("HPBar", "Image/UI/HPBar.bmp", 141, 13, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("MPBar", "Image/UI/MPBar.bmp", 141, 13, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("ExpBar", "Image/UI/ExpBar.bmp", 974, 9, false);
	ImageManager::GetSingleton()->AddImage("ExpUI", "Image/UI/ExpUI.bmp", 1000, 13, false);
	ImageManager::GetSingleton()->AddImage("QuickSlot", "Image/UI/QuickSlot.bmp", 151, 80, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("QuickSlotAlpha", "Image/UI/QuickSlotAlpha.bmp", 151, 80, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("Ability", "Image/UI/Ability.bmp", 214, 297, true, RGB(0, 128, 0)); 
	ImageManager::GetSingleton()->AddImage("AbilityOn", "Image/UI/AbilityOn.bmp", 214, 297, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("Shop", "Image/UI/Shop.bmp", 511, 507, true, RGB(0, 128, 0));
	ImageManager::GetSingleton()->AddImage("Inventory", "Image/UI/Inventory.bmp", 172, 294, true, RGB(0, 128, 0)); 
	ImageManager::GetSingleton()->AddImage("Skill", "Image/UI/Skill.bmp", 174, 300, true, RGB(0, 0, 0));
	ImageManager::GetSingleton()->AddImage("ConsumableItems", "Image/Item/comsumableItems.bmp", 1008, 796, 19, 
								vector<int>{19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 14}, 14, true, RGB(255, 0, 255), 1.0f);
	ImageManager::GetSingleton()->AddImage("Confirm", "Image/UI/Confirm.bmp", 187, 80, true, RGB(255, 0, 255));
	//ImageManager::GetSingleton()->AddImage("attackDamage");

	//��
	SceneManager::GetSingleton()->AddScene("�ӽ����ʵ�", new MushroomFieldScene());
	SceneManager::GetSingleton()->AddScene("��׽ý�����", new FirstTownScene());
	
	//�ε���
	SceneManager::GetSingleton()->AddLoadingScene("�ε���_1", new LoadingScene());
	
	//���� ��
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
	CameraManager::GetSingleton()->Release();
	CalcUtil::GetSingleton()->Release();

	CSound::Release();

	RemoveFontResource("MapleStory/Font/Maplestory Bold.ttf");
	RemoveFontResource("MapleStory/Font/Maplestory Light.ttf");
	RemoveFontResource("MapleStory/Font/Typo_BabyHeartB.ttf");
	RemoveFontResource("MapleStory/Font/Typo_BabyHeartL.ttf");

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
