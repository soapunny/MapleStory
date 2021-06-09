#pragma once
#include "Character.h"
#include "Inventory.h"
#include "Managers.h"
#include "DamageQueue.h"
#include "CalcUtil.h"
#include "MonsterManager.h"
#include "Managers.h"
#include "NPCStorage.h"
#include "NPCEntity.h"
#include "InventoryStore.h"
#include "InventoryEntity.h"
#include "ConsumableItemEntity.h"
#include "Image.h"
#include "Alert.h"
#include "Confirm.h"


void CameraManager::Init(string sceneImage, string minimap, string minimapUI, Unit* character, MonsterManager* monsterManager, NPCManager* npcManager, vector<POINT*>* vPortalLoc)
{
	bg = ImageManager::GetSingleton()->FindImage(sceneImage);
	this->minimap = ImageManager::GetSingleton()->FindImage(minimap);
	this->minimapUI = ImageManager::GetSingleton()->FindImage(minimapUI);
	this->portal = Gdiplus::Image::FromFile(L"Image/Map/portal.png");
	this->levelUp = ImageManager::GetSingleton()->FindImage("LevelUp");
	this->hitDamageImg = ImageManager::GetSingleton()->FindImage("hitDamage");
	barUI = ImageManager::GetSingleton()->FindImage("BarUI");
	monsterBarUI = ImageManager::GetSingleton()->FindImage("MonsterBarUI");
	hpUI = ImageManager::GetSingleton()->FindImage("HPBar");
	mpUI = ImageManager::GetSingleton()->FindImage("MPBar");
	expUI = ImageManager::GetSingleton()->FindImage("ExpUI");
	expBar = ImageManager::GetSingleton()->FindImage("ExpBar"); 
	quickSlot = ImageManager::GetSingleton()->FindImage("QuickSlot");
	quickSlotAlpha = ImageManager::GetSingleton()->FindImage("QuickSlotAlpha");
	abilitySlot = ImageManager::GetSingleton()->FindImage("Ability");
	abilityOnSlot = ImageManager::GetSingleton()->FindImage("AbilityOn");
	inventorySlot = ImageManager::GetSingleton()->FindImage("Inventory");
	skillSlot = ImageManager::GetSingleton()->FindImage("Skill");
	shop = ImageManager::GetSingleton()->FindImage("Shop");
	noGoldAlert = ImageManager::GetSingleton()->FindImage("NoGoldAlert");
	buyConfirm = ImageManager::GetSingleton()->FindImage("BuyConfirm");
	showColor = false;
	
	lWindowBuffer.push_back(UI_WINDOW::MAP);

	vSlotPos.resize(static_cast<int>(UI_WINDOW::END_OF_UI_WINDOW));
	for(int i = 0;i< static_cast<int>(UI_WINDOW::END_OF_UI_WINDOW);i++)
	{
		vSlotPos[i] = POINT{};
		vSlotPos[i].x = 100 * i;
		vSlotPos[i].y = 100 * i;
	}
	vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x = WINSIZE_X - this->minimap->GetWidth();
	vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y = 0;
	isDragging = false;
	
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
	if(!hNPCBrush)
		hNPCBrush = CreateSolidBrush(RGB(240, 0, 240));
	if (!hPortalBrush)
		hPortalBrush = CreateSolidBrush(RGB(0, 0, 255));
	if(!hWhiteBrush)
		hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	if(!hBlackBrush)
		hBlackBrush = CreateSolidBrush(RGB(70, 70, 70));

	clickedNPC = nullptr;
	shopPos.x = 100;
	shopPos.y = 100;
	displayIdx = 0;
	maxCntInPage = 9;
	ConsumableItemsImg = ImageManager::GetSingleton()->FindImage("ConsumableItems");

}

void CameraManager::Update()
{
	//Alert / Confirm 삭제
	for (auto it = lAlerts.begin(); it != lAlerts.end();)
	{
		if (*it && !(*it)->GetOn())
			it = lAlerts.erase(it);
		else
			it++;
	}
	for (auto it = lConfirm.begin(); it != lConfirm.end();)
	{
		if (*it && !((*it)->GetOn()))
			it = lConfirm.erase(it);
		else
			it++;
	}

	//미니맵 켜고 끄기
	if (KeyManager::GetSingleton()->IsOnceKeyDown('M'))
	{
		auto iter = find(lWindowBuffer.begin(), lWindowBuffer.end(), UI_WINDOW::MAP);
		if (iter != lWindowBuffer.end())//존재하면
		{
			lWindowBuffer.erase(iter);
		}
		else//존재하지 않으면
		{
			lWindowBuffer.push_back(UI_WINDOW::MAP);
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown('I'))
	{
		auto iter = find(lWindowBuffer.begin(), lWindowBuffer.end(), UI_WINDOW::INVENTORY);
		if (iter != lWindowBuffer.end())//존재하면
		{
			lWindowBuffer.erase(iter);
		}
		else//존재하지 않으면
		{
			lWindowBuffer.push_back(UI_WINDOW::INVENTORY);
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown('K'))
	{
		auto iter = find(lWindowBuffer.begin(), lWindowBuffer.end(), UI_WINDOW::SKILL);
		if (iter != lWindowBuffer.end())//존재하면
		{
			lWindowBuffer.erase(iter);
		}
		else//존재하지 않으면
		{
			lWindowBuffer.push_back(UI_WINDOW::SKILL);
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown('B'))
	{
		auto iter = find(lWindowBuffer.begin(), lWindowBuffer.end(), UI_WINDOW::ABILITY);
		if (iter != lWindowBuffer.end())//존재하면
		{
			lWindowBuffer.erase(iter);
		}
		else//존재하지 않으면
		{
			lWindowBuffer.push_back(UI_WINDOW::ABILITY);
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_ESCAPE))
	{
		if (!lWindowBuffer.empty())
		{
			lWindowBuffer.pop_back();
		}
	}
	else if (KeyManager::GetSingleton()->IsStayKeyDown(VK_LBUTTON))
	{
		if (!isDragging)
		{
			RECT slotRect;
			RECT exitRect;
			RECT gap;
			Image* image = nullptr;
			//UI창 드래그, 닫기
			if (lWindowBuffer.size() > 0)
			{
				currentWindowIt = lWindowBuffer.end();
				do
				{
					currentWindowIt--;
					switch (*currentWindowIt)
					{
					case UI_WINDOW::INVENTORY:
						image = inventorySlot;
						break;
					case UI_WINDOW::ABILITY:
						image = abilitySlot;
						break;
					case UI_WINDOW::SKILL:
						image = skillSlot;
						break;
					case UI_WINDOW::MAP:
						image = minimap;
						break;
					}
					SetRect(&slotRect,
						vSlotPos[static_cast<int>(*currentWindowIt)].x,
						vSlotPos[static_cast<int>(*currentWindowIt)].y,
						vSlotPos[static_cast<int>(*currentWindowIt)].x + image->GetWidth() - 40,
						vSlotPos[static_cast<int>(*currentWindowIt)].y + 30);
					SetRect(&exitRect,
						vSlotPos[static_cast<int>(*currentWindowIt)].x + image->GetWidth() - 40,
						vSlotPos[static_cast<int>(*currentWindowIt)].y,
						vSlotPos[static_cast<int>(*currentWindowIt)].x + image->GetWidth(),
						vSlotPos[static_cast<int>(*currentWindowIt)].y + 30);
					if (PtInRect(&slotRect, g_ptMouse))
					{
						lWindowBuffer.push_back(*currentWindowIt);
						lWindowBuffer.erase(currentWindowIt);
						currentWindowIt = --lWindowBuffer.end();
						vSlotPos[static_cast<int>(*currentWindowIt)]
							= POINT{ g_ptMouse.x - image->GetWidth() / 2,
									g_ptMouse.y - 20 };
						isDragging = true;
						return;
					}
					else if (PtInRect(&exitRect, g_ptMouse))
					{
						lWindowBuffer.erase(currentWindowIt);
						return;
					}
					//else if (PtInRect())
				} while (currentWindowIt != lWindowBuffer.begin());
			}
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_PRIOR))
	{
		if (clickedNPC)
		{
			if(displayIdx > 0)
				displayIdx--;
		}
	}
	else if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_NEXT))
	{
		if (clickedNPC)
		{
			if (displayIdx + maxCntInPage < clickedNPC->GetLStockConsumableItems().size())
				displayIdx++;
		}
	}

	if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
	{
		//NPC가 클릭되었을 때
		if (!clickedNPC)
			ClickAPSlot();
		ClickNPC();
		ShowStocks();
		isDragging = false;
	}
	// 숨겨진 컬러(hitbox, 타일충돌)를 보여줌
	if (KeyManager::GetSingleton()->IsOnceKeyDown(VK_TAB))
	{
		showColor = !showColor;
	}

	if (isDragging)
	{
		Image* image = nullptr;
		switch (*currentWindowIt)
		{
		case UI_WINDOW::INVENTORY:
			image = inventorySlot;
			break;
		case UI_WINDOW::ABILITY:
			image = abilitySlot;
			break;
		case UI_WINDOW::SKILL:
			image = skillSlot;
			break;
		case UI_WINDOW::MAP:
			image = minimap;
			break;
		}
		vSlotPos[static_cast<int>(*currentWindowIt)].x = g_ptMouse.x - image->GetWidth()/2;
		vSlotPos[static_cast<int>(*currentWindowIt)].y = g_ptMouse.y - 15;
		if (KeyManager::GetSingleton()->IsOnceKeyUp(VK_LBUTTON))
		{
			isDragging = false;
		}
	}

	//portalTimer += TimerManager::GetSingleton()->GetElapsedTime();
	//if (portalTimer >= 0.2)
	//{
	//	portalFrame.x++;
	//	if (portalFrame.x >= portal->GetMaxFrameX())
	//	{
	//		portalFrame.x = 0;
	//		portalFrame.y++;
	//		if (portalFrame.y >= portal->GetMaxFrameY())
	//		{
	//			portalFrame.y = 0;
	//		}
	//	}
	//	portalTimer = 0.0f;
	//}

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
				monster->SetLocalPos(FPOINT{ monster->GetWorldPos().x - pos.x,  monster->GetWorldPos().y - pos.y });
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
	DeleteObject(hWhiteBrush);
	DeleteObject(hBlackBrush);

	for (Alert* alert : lAlerts)
	{
		SAFE_RELEASE(alert);
	}
	lAlerts.clear();

	ReleaseSingleton();
}


void CameraManager::BoundCamera()
{
	//카메라 위치 조정
	pos.x = (LONG)(character->GetWorldPos().x - WINSIZE_X / 2);
	if (pos.x <= 0)
		pos.x = 0;
	else if (pos.x >= bg->GetWidth() - WINSIZE_X)
		pos.x = bg->GetWidth() - WINSIZE_X;

	pos.y = (LONG)(character->GetWorldPos().y - WINSIZE_Y / 2);
	if (pos.y <= 0)
		pos.y = 0;
	else if (pos.y >= bg->GetHeight() - WINSIZE_Y)
		pos.y = bg->GetHeight() - WINSIZE_Y;
}

void CameraManager::BoundCharacter()
{
	RECT shape = character->GetShape();
	if (WINSIZE_X / 2.0f <= character->GetWorldPos().x && character->GetWorldPos().x <= bg->GetWidth() - WINSIZE_X / 2.0f)
	{
		shape.left = (LONG)(WINSIZE_X / 2.0f - character->GetWidth() / 2.0f);
		shape.right = (LONG)(shape.left + character->GetWidth() / 2.0f);
	}
	else if (WINSIZE_X / 2.0f > character->GetWorldPos().x)
	{
		shape.left = (LONG)(character->GetWorldPos().x - character->GetWidth() / 2.0f);
		shape.right = (LONG)(shape.left + character->GetWidth());
	}
	else if (character->GetWorldPos().x > bg->GetWidth() - WINSIZE_X / 2.0f)
	{
		shape.left = (LONG)(character->GetWorldPos().x - (bg->GetWidth() - WINSIZE_X) - character->GetWidth() / 2.0f);
		shape.right = (LONG)(shape.left + character->GetWidth());
	}
	if (WINSIZE_Y / 2.0f <= character->GetWorldPos().y && character->GetWorldPos().y <= bg->GetHeight() - WINSIZE_Y / 2.0f)
	{
		shape.top = (LONG)(WINSIZE_Y / 2.0f - character->GetHeight() / 2.0f);
		shape.bottom = (LONG)(shape.top + character->GetHeight() / 2.0f);
	}
	else if (WINSIZE_Y / 2.0f > character->GetWorldPos().y)
	{
		shape.top = (LONG)(character->GetWorldPos().y - character->GetHeight() / 2.0f);
		shape.bottom = (LONG)(shape.top + character->GetHeight());
	}
	else if (character->GetWorldPos().y > bg->GetHeight() - WINSIZE_Y / 2.0f)
	{
		shape.top = (LONG)(character->GetWorldPos().y - (bg->GetHeight() - WINSIZE_Y) - character->GetHeight() / 2.0f);
		shape.bottom = (LONG)(shape.top + character->GetHeight());
	}

	character->SetShape(shape);
	character->SetLocalPos(FPOINT{ character->GetWorldPos().x - pos.x, character->GetWorldPos().y - pos.y });
}

void CameraManager::ClickAPSlot()
{
	if (lWindowBuffer.size() > 0 && lWindowBuffer.back() == UI_WINDOW::ABILITY)
	{
		if (((Character*)character)->GetAP() > 0)
		{
			int startPosX = vSlotPos[static_cast<int>(UI_WINDOW::ABILITY)].x;
			int startPosY = vSlotPos[static_cast<int>(UI_WINDOW::ABILITY)].y;
			RECT strRect = { startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 188, 
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 210,
							startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 200,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 220
							};
			RECT dexRect = { startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 188,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 229,
							startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 200,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 239
							};
			RECT intRect = { startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 188,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 246,
							startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 200,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 256
			};
			RECT lukRect = { startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 188,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 264,
							startPosX + abilityOnSlot->GetImageInfo()->renderRatio * 200,
							startPosY + abilityOnSlot->GetImageInfo()->renderRatio * 274
			};
			if (PtInRect(&strRect, g_ptMouse))
			{
				((Character*)character)->PlusStr();
				((Character*)character)->MinusAP();
			}
			else if (PtInRect(&dexRect, g_ptMouse))
			{
				((Character*)character)->PlusDex();
				((Character*)character)->MinusAP();
			}
			else if (PtInRect(&intRect, g_ptMouse))
			{
				((Character*)character)->PlusInt();
				((Character*)character)->MinusAP();
			}
			else if (PtInRect(&lukRect, g_ptMouse))
			{
				((Character*)character)->PlusLuk();
				((Character*)character)->MinusAP();
			}
		}
	}
}

void CameraManager::ClickNPC()
{
	if (!clickedNPC)
	{
		RECT slotRect;
		NPCEntity* npcEntity = nullptr;
		for (auto npcPair : *NPCStorage::GetSingleton()->GetMNPCStorage())
		{
			npcEntity = npcPair.second;
			if (npcEntity) {
				SetRect(&slotRect,
					npcEntity->GetWorldPos().x - pos.x - npcEntity->GetImage()->GetFrameWidth() / 2.0f,
					npcEntity->GetWorldPos().y - pos.y - pos.x - npcEntity->GetImage()->GetFrameHeight() / 2.0f,
					npcEntity->GetWorldPos().x - pos.x + npcEntity->GetImage()->GetFrameWidth() / 2.0f,
					npcEntity->GetWorldPos().y - pos.y + npcEntity->GetImage()->GetFrameHeight() / 2.0f);
				if (PtInRect(&slotRect, g_ptMouse))
				{
					npcEntity->SetNPCClicked(true);
					clickedNPC = npcEntity;
					displayIdx = 0;
				}
			}
		}
	}
}

void CameraManager::ShowStocks()
{
	if (clickedNPC)
	{
		RECT exitRect;
		RECT itemRect;
		SetRect(&exitRect,
			shopPos.x + 202,
			shopPos.y + 54,
			shopPos.x + 265,
			shopPos.y + 69);
		if (PtInRect(&exitRect, g_ptMouse))
		{
			clickedNPC->SetNPCClicked(false);
			clickedNPC = nullptr;
		}
		else
		{
			//상점에서 아이템을 살 때
			auto lconsumableItems = clickedNPC->GetLStockConsumableItems();
			if (lconsumableItems.size() > 0) {
				int itemType = 0;
				int cnt = 0;
				int itemIdx = 0;
				auto mInventoryStore = InventoryStore::GetSingleton()->GetMInventoryStore()->find(character->GetName());
				for (auto item : lconsumableItems)//상점 아이템 재고 출력
				{
					if (cnt < displayIdx)
					{
						cnt++;
						continue;
					}
					if (item)
					{
						SetRect(&itemRect,
							shopPos.x + 8,
							shopPos.y + 122 + 42 * itemIdx,
							shopPos.x + 255,
							shopPos.y + 162 + 42 * itemIdx);

						if (PtInRect(&itemRect, g_ptMouse))
						{
							//TODO 아이템 구매
							if (mInventoryStore->second->GetMoney() < item->GetPrice())
							{
								Alert* alert = new Alert();
								alert->Init("메소가 부족합니다.", true);
								lAlerts.push_back(alert);
								return;
							}
							mInventoryStore->second->SetMoney(mInventoryStore->second->GetMoney() - item->GetPrice());//돈 차감

							mInventoryStore->second->insert_consumable_item(item, 1);

							Alert* alert = new Alert();
							alert->Init("아이템이 구매 되었습니다.", true);
							lAlerts.push_back(alert);

							break;
						}
					}
					itemIdx++;
					if (itemIdx >= maxCntInPage)//한 페이지에 보여줄 아이템 수가 최대이면
						break;
				}
			}
		}
	}
}


void CameraManager::FocusOnCharacter(HDC hdc)
{
	HBRUSH hOldBrush;
	if (showColor)
		bg->RenderWalkingCamara(hdc, pos.x, pos.y, false, false);
	else
		bg->RenderWalkingCamara(hdc, pos.x, pos.y, false, true);

	//NPC
	map<string, NPCEntity*>* mNPCStorage = NPCStorage::GetSingleton()->GetMNPCStorage();
	if (mNPCStorage)
	{
		HFONT font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
		HFONT oldFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(255, 255, 255));

		NPCEntity* npcEntity = nullptr;
		int worldPosX = 0;
		int worldPosY = 0;
		RECT npcRECT;
		for (auto npcPair : *mNPCStorage)
		{
			npcEntity = npcPair.second;
			worldPosX = npcEntity->GetWorldPos().x;
			worldPosY = npcEntity->GetWorldPos().y;
			if (npcEntity && pos.x <= worldPosX + npcEntity->GetImage()->GetFrameWidth() / 2.0f
				&& worldPosX - npcEntity->GetImage()->GetFrameWidth() / 2.0f <= pos.x + WINSIZE_X
				&& pos.y <= worldPosY - npcEntity->GetImage()->GetFrameHeight() / 2.0f
				&& worldPosY + npcEntity->GetImage()->GetFrameHeight() / 2.0f <= pos.y + WINSIZE_Y)
			{
				Image* image = npcEntity->GetImage();
				image->RenderAutoFrame(hdc, worldPosX - pos.x, worldPosY - pos.y, 0, 0.3f, true);
				SetRect(&npcRECT, worldPosX - pos.x - image->GetFrameWidth()/2.0f, 
						worldPosY - pos.y + image->GetFrameHeight() * npcEntity->GetImage()->GetImageInfo()->renderRatio / 2.0f,
						worldPosX - pos.x + image->GetFrameWidth() / 2.0f + 10,
						worldPosY - pos.y + image->GetFrameHeight() * npcEntity->GetImage()->GetImageInfo()->renderRatio / 2.0f + 20);
				FillRect(hdc, &npcRECT, hBlackBrush);
				TextOut(hdc, 
					worldPosX - 4 * npcEntity->GetNPCName().size() - pos.x,
					worldPosY + npcEntity->GetImage()->GetFrameHeight() * npcEntity->GetImage()->GetImageInfo()->renderRatio /2.0f - pos.y,
					npcEntity->GetNPCName().c_str(),
					strlen(npcEntity->GetNPCName().c_str())
				);
			}
				
		}
		DeleteObject(font);
	}

	if (monsterManager)
	{
		hOldBrush = (HBRUSH)SelectObject(hdc, hEnemyBrush);
		for (Unit* monster : *monsterManager->GetVMonsters())
		{
			if (monster && monster->GetUnitState() != UNIT_STATE::END_OF_UNIT_STATE && (pos.x <= monster->GetShape().right && monster->GetShape().left <= pos.x + WINSIZE_X && pos.y <= monster->GetShape().bottom && monster->GetShape().top <= pos.y + WINSIZE_Y))
			{
				if (showColor)
				{
					//몬스터 히트박스
					Rectangle(hdc,
						monster->GetWorldPos().x - monster->GetCenterToLeft() - pos.x,
						monster->GetWorldPos().y - pos.y - monster->GetCenterToTop(),
						monster->GetWorldPos().x + monster->GetCenterToRight() - pos.x,
						monster->GetWorldPos().y - pos.y + monster->GetCenterToBottom());
					//몬스터 어택박스
					if (monster->GetAttack())
					{
						Rectangle(hdc, monster->GetAttackBox().left - pos.x, monster->GetAttackBox().top - pos.y, monster->GetAttackBox().right - pos.x, monster->GetAttackBox().bottom - pos.y);
					}
				}

				
				monster->Render(hdc, (int)(monster->GetWorldPos().x - pos.x), (int)(monster->GetWorldPos().y - pos.y));

				//Hp bar for monsters
				if (monster->GetUnitState() != UNIT_STATE::DEAD_STATE)
				{
					monsterBarUI->Render(hdc,
						(int)(monster->GetWorldPos().x - monster->GetCenterToLeft() - pos.x),
						(int)(monster->GetWorldPos().y - monster->GetCenterToTop() - pos.y - monsterBarUI->GetHeight() - 20.0f),
						(int)(monster->GetCenterToLeft() + monster->GetCenterToRight()),
						monsterBarUI->GetHeight()
					);
					hpUI->Render(hdc,
						(int)(monster->GetWorldPos().x - monster->GetCenterToLeft() - pos.x + 1),
						(int)(monster->GetWorldPos().y - monster->GetCenterToTop() - pos.y - monsterBarUI->GetHeight() - 20.0f + 1.0f),
						(int)((monster->GetCenterToLeft() + monster->GetCenterToRight() - 2.0f) * monster->GetHp() / monster->GetMaxHp()),
						hpUI->GetHeight()
					);
				}
				//Damage 표기
				monster->GetDamageQueue()->Render(hdc, FPOINT{ monster->GetWorldPos().x - pos.x, monster->GetWorldPos().y - pos.y - 50 - monster->GetCenterToTop() });
			}
		}
	}
		
	if (character)
	{
		hOldBrush = (HBRUSH)SelectObject(hdc, hPlayerBrush);

		if (showColor)
		{
			//Character HitBox
			Rectangle(hdc, character->GetWorldPos().x - character->GetCenterToLeft() - pos.x,
				character->GetWorldPos().y - pos.y - character->GetCenterToTop(),
				character->GetWorldPos().x + character->GetCenterToRight() - pos.x,
				character->GetWorldPos().y - pos.y + character->GetCenterToBottom()
			);
		}
		character->Render(hdc);
		//Gdiplus::Graphics g(hdc);
		//levelUp->RenderAutoFrame(hdc, 0, 0, 0, 0.2f, false);

		// HP 회복/ 감소 출력
		character->GetRecoveryQueue()->Render(hdc, FPOINT{ character->GetWorldPos().x - pos.x, character->GetWorldPos().y - pos.y - 50 - character->GetCenterToTop() });
		// Damage 표기
		character->GetDamageQueue()->Render(hdc, FPOINT{ character->GetWorldPos().x - pos.x, character->GetWorldPos().y - pos.y - 50 - character->GetCenterToTop() });
	}

	if (portal)
	{
		RenderPortals(hdc);
	}

	//Render Slots
	for (UI_WINDOW window : lWindowBuffer)
	{
		switch (window)
		{
		case UI_WINDOW::MAP:
			RenderMinimap(hdc);
			break;
		case UI_WINDOW::INVENTORY:
			RenderInventory(hdc);
			break;
		case UI_WINDOW::SKILL:
			RenderSkillWindow(hdc);
			break;
		case UI_WINDOW::ABILITY:
			RenderAbility(hdc);
			break;
		}
	}

	if (clickedNPC)
	{
		RenderNPCWindow(hdc);
	}
	for (Alert* alert : lAlerts)
		alert->Render(hdc);

	RenderCharacterUI(hdc);
}


void CameraManager::RenderMinimap(HDC hdc)
{
	//Rectangle(hdc, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y, WINSIZE_X, minimap->GetHeight() + minimapUI->GetHeight());
	minimapUI->Render(hdc, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y);
	minimap->Render(hdc, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x, vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y + minimapUI->GetHeight());

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
			if (monster->GetUnitState() != UNIT_STATE::END_OF_UNIT_STATE)
			{
				minimapEnemyX = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x + monster->GetWorldPos().x * minimap->GetWidth() / bg->GetWidth();
				minimapEnemyY = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y + (monster->GetWorldPos().y + monster->GetCenterToBottom()) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();

				Ellipse(hdc, (int)(minimapEnemyX - 5), (int)(minimapEnemyY - 5), (int)(minimapEnemyX + 5), (int)(minimapEnemyY + 5));
			}
		}
	}
	//포탈 위치
	float minimapPortalX = 0;
	float minimapPortalY = 0;
	hOldBrush = (HBRUSH)SelectObject(hdc, hPortalBrush);
	for (POINT* portalPos : *vPortalLoc)
	{
			minimapPortalX = (float)(vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x + portalPos->x * minimap->GetWidth() / bg->GetWidth());
			minimapPortalY = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y + (portalPos->y + portal->GetHeight()/2.0f + 20.0f) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();
			//portal->FrameRender(hdc, (*portalPos).x - pos.x, (*portalPos).y - pos.y, portalFrame.x, portalFrame.y, false);
			Ellipse(hdc, (int)(minimapPortalX - 5), (int)(minimapPortalY - 5), (int)(minimapPortalX + 5), (int)(minimapPortalY + 5));
	}

	float minimapNPCX = 0;
	float minimapNPCY = 0;
	hOldBrush = (HBRUSH)SelectObject(hdc, hNPCBrush);
	auto npcMap = NPCStorage::GetSingleton()->GetMNPCStorage();
	for (auto npcPair : *npcMap)
	{
		if (npcPair.second)
		{
			minimapNPCX = (float)(vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x + npcPair.second->GetWorldPos().x * minimap->GetWidth() / bg->GetWidth());
			minimapNPCY = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y + (npcPair.second->GetWorldPos().y + npcPair.second->GetImage()->GetHeight() / 2.0f - 20.0f) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();
			Ellipse(hdc, (int)(minimapNPCX - 5), (int)(minimapNPCY - 5), (int)(minimapNPCX + 5), (int)(minimapNPCY + 5));
		}
	}

	//캐릭터 위치
	float minimapCharacterX = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].x + character->GetWorldPos().x * minimap->GetWidth() / bg->GetWidth();
	float minimapCharacterY = vSlotPos[static_cast<int>(UI_WINDOW::MAP)].y + (character->GetWorldPos().y + character->GetCenterToBottom()) * minimap->GetHeight() / bg->GetHeight() + minimapUI->GetHeight();

	hOldBrush = (HBRUSH)SelectObject(hdc, hPlayerBrush);
	Ellipse(hdc, (int)(minimapCharacterX - 5), (int)(minimapCharacterY - 5), (int)(minimapCharacterX + 5), (int)(minimapCharacterY + 5));
}

void CameraManager::RenderPortals(HDC hdc)
{
	for (POINT* portalPos : *vPortalLoc)
	{
		if (pos.x <= (*portalPos).x + portal->GetWidth() / 2.0f && (*portalPos).x - portal->GetHeight() / 2.0f <= pos.x + WINSIZE_X)
		{
			Gdiplus::Graphics g(hdc);
			g.DrawImage(portal, (INT)((*portalPos).x - pos.x), (INT)((*portalPos).y - pos.y) );
		}
	}
}

void CameraManager::RenderCharacterUI(HDC hdc)
{
	int posX = (int)(WINSIZE_X / 2.0f + barUI->GetWidth() / 2.0f);
	int posY = (int)(WINSIZE_Y - barUI->GetHeight() - expUI->GetHeight());
	barUI->Render(hdc, posX, posY);
	SetTextColor(hdc, RGB(255, 255, 255)); //폰트 색상 지정
	HFONT oldFont;
	oldFont = (HFONT)SelectObject(hdc, font);
	string level = "Lv." + CalcUtil::GetSingleton()->TurnIntegerIntoStr(character->GetLevel());
	TextOut(hdc, posX + 20, posY + 2, level.c_str(), level.size());
	TextOut(hdc, posX + 100, posY + 2, character->GetName().c_str(), (character->GetName().size()));
	hpUI->Render(hdc, posX + 23, posY + 27, (int)(hpUI->GetWidth() * character->GetHp() / (float)character->GetMaxHp()), hpUI->GetHeight());
	mpUI->Render(hdc, posX + 23, posY + 45, (int)(mpUI->GetWidth() * character->GetMp() / (float)character->GetMaxMp()), hpUI->GetHeight());
	expUI->Render(hdc, 0, posY + barUI->GetHeight(), WINSIZE_X, expUI->GetHeight(), false);
	float expRatio = ((Character*)character)->GetExp()%10000 / 10000.0f;
	expBar->Render(hdc, 25, posY + barUI->GetHeight() + 2, expRatio * (WINSIZE_X - 30), expBar->GetHeight(), false);
	quickSlot->Render(hdc, WINSIZE_X - quickSlot->GetWidth(), posY + barUI->GetHeight() - quickSlot->GetHeight(), false);
	quickSlotAlpha->Render(hdc, WINSIZE_X - quickSlotAlpha->GetWidth(), posY + barUI->GetHeight() - quickSlotAlpha->GetHeight());
}

void CameraManager::RenderInventory(HDC hdc)
{
	int itemIdx = 0;
	int itemType = 0;
	int maxColumnCnt = 4;
	int maxColumnRow = 6;
	int posX = vSlotPos[static_cast<int>(UI_WINDOW::INVENTORY)].x;
	int posY = vSlotPos[static_cast<int>(UI_WINDOW::INVENTORY)].y;

	inventorySlot->Render(hdc, posX, posY, false);
	//((Character*)character)->GetInventory()->Render(hdc, posX, posY);

	HFONT font = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hdc, font);
	SetTextColor(hdc, RGB(0, 0, 0));
	map<string, InventoryEntity*>* mInventory = InventoryStore::GetSingleton()->GetMInventoryStore();
	for (auto inventoryPair : *mInventory)//캐릭터 인벤토리 아이템 출력
	{
		if (inventoryPair.first == character->GetName())//해당 캐릭터의 인벤토리이면
		{
			InventoryEntity* inventory = inventoryPair.second;
			if (inventory)
			{
				string money = CalcUtil::GetSingleton()->AddComma(inventory->GetMoney());
				TextOut(hdc, posX + 132 - 8*money.size(), posY + 270, money.c_str(), strlen(money.c_str()));//print charater's money
				auto characterItem = inventory->GetConsumableItem();
				itemIdx = 0;
				for (auto itemPair : characterItem)
				{
					itemType = static_cast<int>(itemPair.first->GetItemType());
					ConsumableItemsImg->FrameRender(hdc, posX+2 + 36 * (itemIdx% maxColumnCnt), posY + 40 + 35 * (itemIdx/ maxColumnCnt), itemType % ConsumableItemsImg->GetMaxFrameX(), itemType / ConsumableItemsImg->GetMaxFrameX());
					//갯수
					TextOut(hdc, posX + 10 + 36 * (itemIdx % maxColumnCnt), posY + 70 + 35 * (itemIdx / maxColumnCnt), to_string(itemPair.second).c_str(), strlen(to_string(itemPair.second).c_str()));
					itemIdx++;
					if (itemIdx >= maxColumnCnt * maxColumnRow)//한 페이지에 보여줄 아이템 수가 최대이면
						break;
				}
			}
		}
	}
	DeleteObject(font);
}

void CameraManager::RenderSkillWindow(HDC hdc)
{
	skillSlot->Render(hdc, vSlotPos[static_cast<int>(UI_WINDOW::SKILL)].x, vSlotPos[static_cast<int>(UI_WINDOW::SKILL)].y, false);
}

void CameraManager::RenderAbility(HDC hdc)
{
	int apSlotPosX = vSlotPos[static_cast<int>(UI_WINDOW::ABILITY)].x;
	int apSlotPosY = vSlotPos[static_cast<int>(UI_WINDOW::ABILITY)].y;
	if(((Character*)character)->GetAP() > 0)
		abilityOnSlot->Render(hdc, apSlotPosX, apSlotPosY, false);
	else
		abilitySlot->Render(hdc, apSlotPosX, apSlotPosY, false);

	HFONT font = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
	HFONT oldFont = (HFONT)SelectObject(hdc, font);
	SetTextColor(hdc, RGB(0, 0, 0));

	int plusX = 32;
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, character->GetName().c_str(), strlen(character->GetName().c_str()) );
	plusX += 18;
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, ((Character*)character)->GetOccupation().c_str(), strlen(((Character*)character)->GetOccupation().c_str()));
	plusX += 18;
	string criticalPer = (to_string((int)(character->GetCriticalPercentage()*100))).append("%");
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, criticalPer.c_str(), strlen(criticalPer.c_str()) );
	plusX += 18;
	string Damage = to_string((int)character->GetDamage());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, Damage.c_str(), strlen(Damage.c_str()));
	plusX += 18;
	string armor = to_string((int)character->GetDefense());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, armor.c_str(), strlen(armor.c_str()));
	plusX += 18;
	string hp = to_string((int)character->GetHp());
	string maxHP = to_string((int)character->GetMaxHp());
	hp.append(" / "); hp.append(maxHP);
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, hp.c_str(), strlen(hp.c_str()));
	plusX += 18;

	string mp = to_string((int)character->GetMp());
	string maxMP = to_string((int)character->GetMaxMp());
	mp.append(" / "); mp.append(maxMP);
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, mp.c_str(), strlen(mp.c_str()));

	plusX += 43;
	string ap = to_string(((Character*)character)->GetAP());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, ap.c_str(), strlen(ap.c_str()));

	plusX += 26;
	string state = to_string(((Character*)character)->GetStr());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, state.c_str(), strlen(state.c_str()));

	plusX += 18;
	state = to_string(((Character*)character)->GetDex());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, state.c_str(), strlen(state.c_str()));

	plusX += 18;
	state = to_string(((Character*)character)->GetInt());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, state.c_str(), strlen(state.c_str()));

	plusX += 18;
	state = to_string(((Character*)character)->GetLuk());
	TextOut(hdc, apSlotPosX + 75, apSlotPosY + plusX, state.c_str(), strlen(state.c_str()));

	DeleteObject(font);
}

void CameraManager::RenderNPCWindow(HDC hdc)
{
	//상점 및 아이템 뿌려주기
	auto lconsumableItems = clickedNPC->GetLStockConsumableItems();
	int size = clickedNPC->GetLStockConsumableItems().size();
	if (size > 0)//상점이 존재하는 NPC이면
	{
		HFONT font = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("메이플스토리"));
		HFONT oldFont = (HFONT)SelectObject(hdc, font);
		SetTextColor(hdc, RGB(100, 100, 100));
		shop->Render(hdc, shopPos.x, shopPos.y, false);
		clickedNPC->GetImage()->FrameRender(hdc, shopPos.x, shopPos.y - 50, 0, 1, false);
		character->GetImage()->FrameRender(hdc, shopPos.x + 270, shopPos.y - 40, 0, 0, false);
		int itemType = 0;
		int cnt = 0;
		int itemIdx = 0;
		for (auto item : lconsumableItems)//상점 아이템 재고 출력
		{
			if (cnt < displayIdx) 
			{
				cnt++;
				continue;
			}
			if (item)
			{
				itemType = static_cast<int>(item->GetItemType());
				ConsumableItemsImg->FrameRender(hdc, shopPos.x + 3, shopPos.y + 113 + 42 * itemIdx, itemType % ConsumableItemsImg->GetMaxFrameX(), itemType / ConsumableItemsImg->GetMaxFrameX());
				
				TextOut(hdc, shopPos.x + 49, shopPos.y + 127 + 42 * itemIdx, item->GetItemName().c_str(), strlen(item->GetItemName().c_str()));
				string price = CalcUtil::GetSingleton()->AddComma(item->GetPrice());
				price.append(" 메소");
				TextOut(hdc, shopPos.x + 65, shopPos.y + 144 + 42 * itemIdx, price.c_str(), strlen(price.c_str()));

			}
			itemIdx++;
			if (itemIdx >= maxCntInPage)//한 페이지에 보여줄 아이템 수가 최대이면
				break;
		}
		map<string, InventoryEntity*>* mInventory = InventoryStore::GetSingleton()->GetMInventoryStore();
		for (auto inventoryPair : *mInventory)//캐릭터 인벤토리 아이템 출력
		{
			if (inventoryPair.first == character->GetName())//해당 캐릭터의 인벤토리이면
			{
				InventoryEntity* inventory = inventoryPair.second;
				if (inventory)
				{
					string money = CalcUtil::GetSingleton()->AddComma(inventory->GetMoney());
					TextOut(hdc, shopPos.x + 420, shopPos.y + 57, money.c_str(), strlen(money.c_str()));//print charater's money
					auto characterItem = inventory->GetConsumableItem();
					itemIdx = 0;
					for (auto itemPair : characterItem)
					{
						itemType = static_cast<int>(itemPair.first->GetItemType());
						ConsumableItemsImg->FrameRender(hdc, shopPos.x + 280, shopPos.y + 116 + 42 * itemIdx, itemType % ConsumableItemsImg->GetMaxFrameX(), itemType / ConsumableItemsImg->GetMaxFrameX());
						//갯수
						TextOut(hdc, shopPos.x + 285, shopPos.y + 146 + 42 * itemIdx, to_string(itemPair.second).c_str(), strlen(to_string(itemPair.second).c_str()));

						TextOut(hdc, shopPos.x + 325, shopPos.y + 127 + 42 * itemIdx, itemPair.first->GetItemName().c_str(), strlen(itemPair.first->GetItemName().c_str()));
						string price = CalcUtil::GetSingleton()->AddComma(itemPair.first->GetPrice()*0.7f);
						price.append(" 메소");
						TextOut(hdc, shopPos.x + 325, shopPos.y + 144 + 42 * itemIdx, price.c_str(), strlen(price.c_str()));
						
						itemIdx++;
						if (itemIdx >= maxCntInPage)//한 페이지에 보여줄 아이템 수가 최대이면
							break;
					}
				}
			}
		}

		DeleteObject(font);
	}

	//TODO Quest관련 출력
}

bool CameraManager::CompareWindow(int& num1, int& num2)//오름차순 정렬을 위한
{
	return num1 < num2;
}
