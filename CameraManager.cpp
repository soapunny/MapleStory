#pragma once
#include "Character.h"
#include "Managers.h"
#include "DamageQueue.h"
#include "CalcUtil.h"
#include "MonsterManager.h"
#include "Managers.h"
#include "Image.h"


void CameraManager::Init(string sceneImage, string minimap, string minimapUI, Unit* character, MonsterManager* monsterManager, NPCManager* npcManager, vector<POINT*>* vPortalLoc)
{
	bg = ImageManager::GetSingleton()->FindImage(sceneImage);
	this->minimap = ImageManager::GetSingleton()->FindImage(minimap);
	this->minimapUI = ImageManager::GetSingleton()->FindImage(minimapUI);
	this->portal = ImageManager::GetSingleton()->FindImage("portal");
	this->hitDamageImg = ImageManager::GetSingleton()->FindImage("hitDamage");
	barUI = ImageManager::GetSingleton()->FindImage("BarUI");
	monsterBarUI = ImageManager::GetSingleton()->FindImage("MonsterBarUI");
	hpUI = ImageManager::GetSingleton()->FindImage("HPBar");
	mpUI = ImageManager::GetSingleton()->FindImage("MPBar");

	font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
	hitDamageTimer = 0.0f;
	
	//초기화
	this->character = nullptr;
	this->monsterManager = nullptr;
	this->npcManager = nullptr;

	this->character = character;
	this->monsterManager = monsterManager;
	this->npcManager = npcManager;
	this->vPortalLoc = vPortalLoc;

	pos.x = 0;
	pos.y = bg->GetHeight() - WINSIZE_Y;

	if(!hPlayerBrush)
		hPlayerBrush = CreateSolidBrush(RGB(0, 255, 0));
	if (!hEnemyBrush)
		hEnemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	if (!hPortalBrush)
		hPortalBrush = CreateSolidBrush(RGB(0, 0, 255));

	isMinimapOn = true;
}

void CameraManager::Update()
{
	//미니맵 켜고 끄기
	if (KeyManager::GetSingleton()->IsOnceKeyDown('M'))
	{
		isMinimapOn = !isMinimapOn;
	}

	portalTimer += TimerManager::GetSingleton()->GetElapsedTime();
	if (portalTimer >= 0.2)
	{
		portalFrame.x++;
		if (portalFrame.x >= portal->GetMaxFrameX())
		{
			portalFrame.x = 0;
			portalFrame.y++;
			if (portalFrame.y >= portal->GetMaxFrameY())
			{
				portalFrame.y = 0;
			}
		}
		portalTimer = 0.0f;
	}

	//카메라 이동 제한
	BoundCamera();

	//캐릭터 위치 조정
	BoundCharacter();
	//몬스터 로컬 좌표 갱신
	if (monsterManager)
	{
		for (Unit* monster : *monsterManager->GetVMonsters())
		{
			if (monster->GetUnitState() != UNIT_STATE::DEAD_STATE)
			{
				monster->SetLocalPos(FPOINT{ monster->GetCenter().x - pos.x,  monster->GetCenter().y - pos.y });
			}
		}
	}
}

void CameraManager::Release()
{
	DeleteObject(font);
	DeleteObject(hPlayerBrush);
	DeleteObject(hEnemyBrush);
	DeleteObject(hPlayerBrush);

	ReleaseSingleton();
}


void CameraManager::BoundCamera()
{
	//카메라 위치 조정
	pos.x = character->GetCenter().x - WINSIZE_X / 2;
	if (pos.x <= 0)
		pos.x = 0;
	else if (pos.x >= bg->GetWidth() - WINSIZE_X)
		pos.x = bg->GetWidth() - WINSIZE_X;

	pos.y = character->GetCenter().y - WINSIZE_Y / 2;
	if (pos.y <= 0)
		pos.y = 0;
	else if (pos.y >= bg->GetHeight() - WINSIZE_Y)
		pos.y = bg->GetHeight() - WINSIZE_Y;
}

void CameraManager::BoundCharacter()
{
	RECT shape = character->GetShape();
	if (WINSIZE_X / 2.0f <= character->GetCenter().x && character->GetCenter().x <= bg->GetWidth() - WINSIZE_X / 2.0f)
	{
		shape.left = WINSIZE_X / 2.0f - character->GetWidth() / 2.0f;
		shape.right = shape.left + character->GetWidth() / 2.0f;
	}
	else if (WINSIZE_X / 2.0f > character->GetCenter().x)
	{
		shape.left = character->GetCenter().x - character->GetWidth() / 2.0f;
		shape.right = shape.left + character->GetWidth();
	}
	else if (character->GetCenter().x > bg->GetWidth() - WINSIZE_X / 2.0f)
	{
		shape.left = character->GetCenter().x - (bg->GetWidth() - WINSIZE_X) - character->GetWidth() / 2.0f;
		shape.right = shape.left + character->GetWidth();
	}
	if (WINSIZE_Y / 2.0f <= character->GetCenter().y && character->GetCenter().y <= bg->GetHeight() - WINSIZE_Y / 2.0f)
	{
		shape.top = WINSIZE_Y / 2.0f - character->GetHeight() / 2.0f;
		shape.bottom = shape.top + character->GetHeight() / 2.0f;
	}
	else if (WINSIZE_Y / 2.0f > character->GetCenter().y)
	{
		shape.top = character->GetCenter().y - character->GetHeight() / 2.0f;
		shape.bottom = shape.top + character->GetHeight();
	}
	else if (character->GetCenter().y > bg->GetHeight() - WINSIZE_Y / 2.0f)
	{
		shape.top = character->GetCenter().y - (bg->GetHeight() - WINSIZE_Y) - character->GetHeight() / 2.0f;
		shape.bottom = shape.top + character->GetHeight();
	}

	character->SetShape(shape);
}


void CameraManager::FocusOnCharacter(HDC hdc)
{
	HBRUSH hOldBrush;
	bg->RenderWalkingCamara(hdc, pos.x, pos.y);

	if (isMinimapOn)
	{
		RenderMinimap(hdc);
	}

	if (npcManager)
	{
		//TODO npc 그려주기
	}

	if (monsterManager)
	{
		hOldBrush = (HBRUSH)SelectObject(hdc, hEnemyBrush);
		for (Unit* monster : *monsterManager->GetVMonsters())
		{
			if (monster && monster->GetUnitState() != UNIT_STATE::END_OF_UNIT_STATE && (pos.x <= monster->GetShape().right && monster->GetShape().left <= pos.x + WINSIZE_X && pos.y <= monster->GetShape().bottom && monster->GetShape().top <= pos.y + WINSIZE_Y))
			{
				////몬스터 히트박스
				//Rectangle(hdc, 
				//	monster->GetCenter().x - monster->GetCenterToLeft() - pos.x,
				//	monster->GetCenter().y - pos.y - monster->GetCenterToTop(),
				//	monster->GetCenter().x + monster->GetCenterToRight() - pos.x,
				//	monster->GetCenter().y - pos.y + monster->GetCenterToBottom());

				////몬스터 어택박스
				//if (monster->GetAttack())
				//{
				//	Rectangle(hdc, monster->GetAttackBox().left - pos.x, monster->GetAttackBox().top - pos.y, monster->GetAttackBox().right - pos.x, monster->GetAttackBox().bottom - pos.y);
				//}
				
				monster->Render(hdc, monster->GetCenter().x - pos.x, monster->GetCenter().y - pos.y);

				//Hp bar for monsters
				monsterBarUI->Render(hdc,
					monster->GetCenter().x - monster->GetCenterToLeft() - pos.x,
					monster->GetCenter().y - monster->GetCenterToTop() - pos.y - monsterBarUI->GetHeight() - 20.0f,
					monster->GetCenterToLeft() + monster->GetCenterToRight(),
					monsterBarUI->GetHeight()
					);
				hpUI->Render(hdc,
					monster->GetCenter().x - monster->GetCenterToLeft() - pos.x + 1,
					monster->GetCenter().y - monster->GetCenterToTop() - pos.y - monsterBarUI->GetHeight() - 20.0f + 1.0f,
					(monster->GetCenterToLeft() + monster->GetCenterToRight() - 2.0f ) * monster->GetHp() / monster->GetMaxHp(),
					hpUI->GetHeight()
					);
				//Damage 표기
				monster->GetDamageQueue()->Render(hdc, FPOINT{ monster->GetCenter().x - pos.x, monster->GetCenter().y - pos.y - 50 - monster->GetCenterToTop() });
			}
		}
	}
		
	if (character)
	{
		hOldBrush = (HBRUSH)SelectObject(hdc, hPlayerBrush);
		//Character HitBox
		Rectangle(hdc, character->GetCenter().x - character->GetCenterToLeft() - pos.x,
			character->GetCenter().y - pos.y - character->GetCenterToTop(),
			character->GetCenter().x + character->GetCenterToRight() - pos.x,
			character->GetCenter().y - pos.y + character->GetCenterToBottom()
		);
		character->Render(hdc);
		//Character CenterX line
		/*MoveToEx(hdc, character->GetCenter().x - character->GetCenterToLeft() - pos.x, character->GetCenter().y - pos.y, NULL);
		LineTo(hdc, character->GetCenter().x + character->GetCenterToRight() - pos.x, character->GetCenter().y - pos.y);*/

		// HP 회복/ 감소 출력
		character->GetRecoveryQueue()->Render(hdc, FPOINT{ character->GetCenter().x - pos.x, character->GetCenter().y - pos.y - 50 - character->GetCenterToTop() });
		// Damage 표기
		character->GetDamageQueue()->Render(hdc, FPOINT{ character->GetCenter().x - pos.x, character->GetCenter().y - pos.y - 50 - character->GetCenterToTop() });
	}

	if (portal)
	{
		RenderPortals(hdc);
	}


	RenderCharacterUI(hdc);
}


void CameraManager::RenderMinimap(HDC hdc)
{
	int startX = WINSIZE_X - minimap->GetWidth();

	Rectangle(hdc, startX, 0, WINSIZE_X, minimap->GetHeight() + minimapUI->GetHeight());
	minimapUI->Render(hdc, startX, 0);
	minimap->Render(hdc, startX, minimapUI->GetHeight());

	//npc 위치
	if (npcManager != nullptr)
	{
		//TODO 미니맵에서 npc 표시하기
	}

	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hEnemyBrush);
	//몬스터 위치
	if (monsterManager != nullptr)
	{ 
		float minimapEnemyX = 0;
		float minimapEnemyY = 0;
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hEnemyBrush);
		for (Unit* monster : *monsterManager->GetVMonsters())
		{
			if (monster->GetUnitState() != UNIT_STATE::DEAD_STATE)
			{
				minimapEnemyX = startX + monster->GetCenter().x * minimap->GetWidth() / bg->GetWidth();
				minimapEnemyY = (monster->GetCenter().y + monster->GetCenterToBottom()) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();

				Ellipse(hdc, minimapEnemyX - 5, minimapEnemyY - 5, minimapEnemyX + 5, minimapEnemyY + 5);
			}
		}
	}
	//포탈 위치
	float minimapPortalX = 0;
	float minimapPortalY = 0;
	hOldBrush = (HBRUSH)SelectObject(hdc, hPortalBrush);
	for (POINT* portalPos : *vPortalLoc)
	{
			minimapPortalX = startX + portalPos->x * minimap->GetWidth() / bg->GetWidth();
			minimapPortalY = (portalPos->y + portal->GetFrameHeight()/2.0f) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();
			portal->FrameRender(hdc, (*portalPos).x - pos.x, (*portalPos).y - pos.y, portalFrame.x, portalFrame.y, false);
			Ellipse(hdc, minimapPortalX - 5, minimapPortalY - 5, minimapPortalX + 5, minimapPortalY + 5);
	}

	//캐릭터 위치
	float minimapCharacterX = startX + character->GetCenter().x * minimap->GetWidth() / bg->GetWidth();
	float minimapCharacterY = (character->GetCenter().y + character->GetCenterToBottom()) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();

	hOldBrush = (HBRUSH)SelectObject(hdc, hPlayerBrush);
	Ellipse(hdc, minimapCharacterX - 5, minimapCharacterY - 5, minimapCharacterX + 5, minimapCharacterY + 5);
}

void CameraManager::RenderPortals(HDC hdc)
{
	for (POINT* portalPos : *vPortalLoc)
	{
		if(pos.x <= (*portalPos).x + portal->GetFrameWidth()/2.0f && (*portalPos).x - portal->GetFrameWidth() / 2.0f <= pos.x + WINSIZE_X)
			portal->FrameRender(hdc, (*portalPos).x - pos.x, (*portalPos).y - pos.y, portalFrame.x, portalFrame.y, false);
	}
}

void CameraManager::RenderCharacterUI(HDC hdc)
{
	int posX = WINSIZE_X / 2.0f + barUI->GetWidth() / 2.0f;
	int posY = WINSIZE_Y - barUI->GetHeight();
	barUI->Render(hdc, posX, posY);
	SetTextColor(hdc, RGB(255, 255, 255)); //폰트 색상 지정
	HFONT oldFont;
	oldFont = (HFONT)SelectObject(hdc, font);
	string level = "Lv." + CalcUtil::GetSingleton()->TurnIntegerIntoStr(character->GetLevel());
	TextOut(hdc, posX + 20, posY + 2, level.c_str(), level.size());
	TextOut(hdc, posX + 100, posY + 2, character->GetName().c_str(), (character->GetName().size()));
	hpUI->Render(hdc, posX + 23, posY + 27, hpUI->GetWidth() * character->GetHp() / (float)character->GetMaxHp(), hpUI->GetHeight());
	mpUI->Render(hdc, posX + 23, posY + 45, mpUI->GetWidth() * character->GetMp() / (float)character->GetMaxMp(), hpUI->GetHeight());

}
