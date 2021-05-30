#pragma once
#include "Singleton.h"
#include "config.h"

enum class CHECK_DIRECTION{LEFT, RIGHT, TOP, BOTTOM, ROPE, PORTAL, END_OF_CHECK_DIRECTION};

enum class JUMPING_STATE;
class Unit;
class Image;
class CollisionManager : public Singleton<CollisionManager>
{
private:
	Unit* player;
	vector<Unit*>* vEnemyUnits;//TODO 컨테이너 뭘로 할 지 정하기
	RECT commonRect;

	Image* hitDamageImg;
	Image* attackDamageImg;
	POINT hitFrame;
	POINT attackFrame;

	bool isHitDamageReady;
	float hitDamageTimer;

	bool hasLeftCollided;
	bool hasRightCollided;
	bool hasTopCollided;
	bool hasBottomCollided;
	bool hasRopeCollided;
	bool hasPortalCollided;

	void CheckPixelCollision(Unit* unit, CHECK_DIRECTION checkDirection, float r);

	bool CheckLeftPixelCollision(Unit* unit, float r);
	bool CheckRightPixelCollision(Unit* unit, float r);
	bool CheckTopPixelCollision(Unit* unit, float r);
	bool CheckBottomPixelCollision(Unit* unit, float r);
	bool CheckRopePixelCollision(Unit* unit);
	bool CheckPortalPixelCollision(Unit* unit);

	void CheckPlayerEnemiesCollision();
	void CheckPlayerProtileEnemiesCollision();

	inline bool CheckBlock(int r, int g, int b) { return r == 255 && g == 0 && b == 255; }
	inline bool CheckIsland(int r, int g, int b) { return r == 0 && g == 0 && b == 0; }
	inline bool CheckLadder(int r, int g, int b) { return r == 0 && g == 255 && b == 255; }
	inline bool CheckPortal(int r, int g, int b) { return r == 255 && g == 255 && b == 0; }

	//Pixel충돌
	void CheckPixelCollisionFor(Unit* unit, bool checkRope, bool checkPortal);


	void ProcessDamage(Unit* attackUnit, Unit* defenseUnit);

public:
	void RegisterPlayer(Unit* player);
	void RegisterVEnemyUnits(vector<Unit*>* vEnemyUnits);

	HRESULT Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

