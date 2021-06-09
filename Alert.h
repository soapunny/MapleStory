#pragma once
#include "GameNode.h"

class Image;
class Alert : public GameNode
{
private:
	Image* image;
	string content;
	RECT confirmRect;
	float width;
	float height;
	float renderRatio;
	POINT pos;
	HFONT font;
	bool isOn;

public:
	virtual HRESULT Init();
	HRESULT Init(string content, bool isOn = true);
	virtual void Release();
	virtual void Render(HDC hdc);

	void CheckClick();

	inline void SetOn(bool isOn) { this->isOn = isOn; }

	inline bool GetOn() { return isOn; }
	inline POINT GetPos() { return pos; }
	inline Image* GetImage() { return image; }
	inline RECT GetConfirmRect() { return confirmRect; }
};

