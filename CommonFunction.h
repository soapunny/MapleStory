// CommonFunction.h

#pragma once
#include "config.h"

// 두 점 사이의 각도를 구하는 함수
inline float GetAngle(FPOINT pos, FPOINT targetPos)
{
	float angle = 0.0f;
	float x = targetPos.x - pos.x;
	float y = targetPos.y - pos.y;

	angle = atan2(-y, x);

	return angle;
}

// 네모를 그리는 함수
inline void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

inline void RenderRectToCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc,
		x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2));
}

inline void RenderEllipseToCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc,
		x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2));
}

inline bool PointInRect(POINT ptMouse, RECT rc)
{
	if (ptMouse.x < rc.left || ptMouse.x > rc.right
		|| ptMouse.y < rc.top || ptMouse.y > rc.bottom)
	{
		return false;
	}

	return true;
}

inline bool PointInRect(POINT ptMouse, POINT rcPos, int rcSize)
{
	if (ptMouse.x < rcPos.x - (rcSize / 2)
		|| ptMouse.x > rcPos.x + (rcSize / 2)
		|| ptMouse.y < rcPos.y - (rcSize / 2)
		|| ptMouse.y > rcPos.y + (rcSize / 2))
	{
		return false;
	}

	return true;
}

inline bool RectInRect(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left || rc1.left > rc2.right
		|| rc1.top > rc2.bottom || rc1.bottom < rc2.top)
		return false;

	return true;
}

inline float GetHeightFromParabolaMoving(float x/*, POINT currentPos*/)
{
	// y = a(x-h)^2 + k
	float y = 0.01f * ((x - 200.0f) * (x - 200.0f)) - 400.0f;
	if (y > 0)
		y = 0.0f;
	return /*currentPos.y + */y;
}

inline RECT GetRect(int left, int top, int width, int height)
{
	RECT rc{ left, top, left + width, top + height };
	return rc;
}

inline RECT GetRectToCenter(int posX, int posY, int width, int height)
{
	RECT rc{ posX - (width / 2), posY - (height / 2), posX + (width / 2), posY + (height / 2) };
	return rc;
}

inline void SetClientRect(HWND hWnd, int width, int height)
{
	RECT crt;
	DWORD style, exStyle;

	SetRect(&crt, 0, 0, width, height);
	style = GetWindowLong(hWnd, GWL_STYLE);
	exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	AdjustWindowRectEx(&crt, style, GetMenu(hWnd) != NULL, exStyle);
	if (style & WS_VSCROLL) crt.right += GetSystemMetrics(SM_CXVSCROLL);
	if (style & WS_HSCROLL) crt.bottom += GetSystemMetrics(SM_CYHSCROLL);
	SetWindowPos(hWnd, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,
		SWP_NOMOVE | SWP_NOZORDER);
}