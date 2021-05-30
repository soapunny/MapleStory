#pragma once
#include "Singleton.h"
#include "config.h"

class Image;
class Unit;
class MonsterManager;
class NPCManager;
class CameraManager : public Singleton<CameraManager>
{
private:
	Image* bg;
	Image* minimap;
	Image* minimapUI;
	Image* portal;
	Image* hitDamageImg;
	Image* barUI;
	Image* monsterBarUI;
	Image* hpUI;
	Image* mpUI;

	char szText[128] = "";
	HFONT font;

	float hitDamageTimer;

	Unit* character;
	MonsterManager* monsterManager;
	NPCManager* npcManager;

	POINT pos;
	bool isMinimapOn;

	POINT portalFrame;
	vector<POINT*>* vPortalLoc;
	float portalTimer;

	HBRUSH hPlayerBrush;
	HBRUSH hEnemyBrush;
	HBRUSH hPortalBrush;

	void BoundCamera();
	void BoundCharacter();
	void RenderMinimap(HDC hdc);
	void RenderPortals(HDC hdc);
	void RenderCharacterUI(HDC hdc);

public:
	void Init(string sceneImage, string minimap, string minimapUI, Unit* character, MonsterManager* monsterManager, NPCManager* npcManager, vector<POINT*>* vPortalLoc);
	void Update();
	void Release();

	void FocusOnCharacter(HDC hdc);

	Image* GetBG() { return bg; }
};

