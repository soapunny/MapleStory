// config.h
#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include <Windows.h>
#include <string>
#include <ctime>
#include <bitset>
#include <map>
#include <vector>
using namespace std;

#define WINSIZE_X	1080
#define WINSIZE_Y	720
#define TILEMAPTOOLSIZE_X	1620
#define TILEMAPTOOLSIZE_Y	900
#define ASTARSIZE_X	1620
#define ASTARSIZE_Y	900

#define PI			3.141592f
#define GA			9.8f

#define DegToRad(x)	((x) * PI / 180.0f)
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#define SAFE_RELEASE(p) { if (p) { p->Release(); delete p; p = nullptr; } }

typedef struct tagFPoint
{
	float x;
	float y;
} FPOINT, * PFPOINT;

//typedef tagFPoint FPOINT2;
//typedef tagFPoint* PFPOINT2;

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;