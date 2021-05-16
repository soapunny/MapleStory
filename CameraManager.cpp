#pragma once
#include "CameraManager.h"
#include "Character.h"
#include "Image.h"

void CameraManager::Init(string sceneImage)
{
	bg = ImageManager::GetSingleton()->FindImage(sceneImage);
	
	pos.x = 0;
	pos.y = bg->GetHeight() - WINSIZE_Y;
}

void CameraManager::Update(Unit* character)
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


	//캐릭터 위치 조정
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


void CameraManager::FocusOnCharacter(HDC hdc, Unit* character)
{
	bg->RenderWalkingCamara(hdc, pos.x, pos.y);
}
