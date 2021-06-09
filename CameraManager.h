#pragma once
#include "Singleton.h"
#include "config.h"

enum class UI_WINDOW {MAP, INVENTORY, ABILITY, SKILL, END_OF_UI_WINDOW};
enum class SHOP_STATE{SHOPPING, NO_GOLD, READY_TO_BUY, };

typedef struct _uiWindowInfo
{
	UI_WINDOW uiWindow;
	bool isOn;
}UIWindowInfo;

class Image;
class Unit;
class MonsterManager;
class NPCManager;
class Alert;
class Confirm;
class CameraManager : public Singleton<CameraManager>
{
private:
	Image* bg;
	Image* minimap;
	Image* minimapUI;
	Gdiplus::Image* portal;
	Image* levelUp;
	Image* hitDamageImg;
	Image* barUI;
	Image* monsterBarUI;
	Image* hpUI;
	Image* mpUI;
	Image* expBar;
	Image* expUI;
	Image* quickSlot;
	Image* quickSlotAlpha;
	Image* abilitySlot;
	Image* abilityOnSlot;
	Image* inventorySlot;
	Image* skillSlot;
	Image* shop;
	Image* ConsumableItemsImg;
	Image* noGoldAlert;
	Image* buyConfirm;

	bool showColor;
	float npcRenderTimer;

	list<Alert*> lAlerts;
	list<Confirm*> lConfirm;
	vector<POINT> vSlotPos;

	list<UI_WINDOW> lWindowBuffer;
	list<UI_WINDOW>::iterator currentWindowIt;
	bool isDragging;

	char szText[128] = "";
	HFONT font;

	float hitDamageTimer;

	Unit* character;
	MonsterManager* monsterManager;
	NPCManager* npcManager;

	POINT pos;

	POINT portalFrame;
	vector<POINT*>* vPortalLoc;
	float portalTimer;

	HBRUSH hPlayerBrush;
	HBRUSH hEnemyBrush;
	HBRUSH hNPCBrush;
	HBRUSH hPortalBrush;
	HBRUSH hWhiteBrush;
	HBRUSH hBlackBrush;

	NPCEntity* clickedNPC;
	POINT shopPos;
	int displayIdx;
	int maxCntInPage;
	
	void BoundCamera();
	void BoundCharacter();
	void ClickAPSlot();
	void ClickNPC();
	void ShowStocks();
	void RenderMinimap(HDC hdc);
	void RenderPortals(HDC hdc);
	void RenderCharacterUI(HDC hdc);
	void RenderInventory(HDC hdc);
	void RenderSkillWindow(HDC hdc);
	void RenderAbility(HDC hdc);
	void RenderNPCWindow(HDC hdc);

	bool CompareWindow(int& num1, int& num2);

public:
	void Init(string sceneImage, string minimap, string minimapUI, Unit* character, MonsterManager* monsterManager, NPCManager* npcManager, vector<POINT*>* vPortalLoc);
	void Update();
	void Release();

	void FocusOnCharacter(HDC hdc);

	Image* GetBG() { return bg; }
};

