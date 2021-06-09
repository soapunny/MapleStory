#pragma once
#include "GameNode.h"

class Image;
class Confirm : public GameNode
{
private:
	Image* image;
	string content;
	RECT confirmRect;
	RECT cancelRect;
	float width;
	float height;
	float renderRatio;
	POINT pos;
	HFONT font;
	bool isOn;
	bool isConfirm;

public:
	virtual HRESULT Init();
	HRESULT Init(string content, bool isOn = true);
	virtual void Release();
	virtual void Render(HDC hdc);

	void CheckClick();

	inline void SetOn(bool isOn) { this->isOn = isOn; }
	inline void SetConfirm(bool isConfirm) { this->isConfirm = isConfirm; }


	inline POINT GetPos() { return pos; }
	inline bool GetOn() { return isOn; }
	inline bool GetConfirm() { return isConfirm; }
	inline Image* GetImage() { return image; }
	inline RECT GetConfirmRect() { return confirmRect; }
	inline RECT GetCancelRect() { return cancelRect; }
};

