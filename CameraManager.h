#pragma once
#include "Singleton.h"
#include "config.h"

class Image;
class Unit;
class CameraManager : public Singleton<CameraManager>
{
private:
	Image* bg;
	POINT pos;
public:
	void Init();
	void Update(Unit* character);
	void FocusOnCharacter(HDC hdc, Unit* character);

	Image* GetBG() { return bg; }
};

