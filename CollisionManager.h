#pragma once
#include "Singleton.h"
#include "config.h"

enum class JUMPING_STATE;
class Unit;
class CollisionManager : public Singleton<CollisionManager>
{
private:
	Unit* player;
	vector<Unit*>* vEnemyUnits;//TODO 컨테이너 뭘로 할 지 정하기

public:
	void RegisterPlayer(Unit* player);
	void RegisterVEnemyUnits(vector<Unit*>* vEnemyUnits);

	HRESULT Init();
	void Update();
	void Render();
	void Release();

	void CheckPlayerBottomPixelCollision();
	void CheckPlayerSidePixelCollision();
	void CheckPlayerRopePixelCollision();

	void CheckEnemiesBottomPixelCollision();
	void CheckEnemiesSidePixelCollision();
};

