#pragma once
#include "GameNode.h"

class Image;
class TownScene : public GameNode
{
private:
	Image* image;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	virtual ~TownScene() {}
};

