#pragma once
#include "GameNode.h"

/*
	함수 포인터 : 함수의 ...

	(변수) 포인터 : 변수의 메모리주소를 담을 수 있는 (값으로 가지는) 변수

	int a = 10;
	int* pA = &a;

	void sum(int a, int b);

	void (*funcPointer)(int, int);	// 함수포인터(변수) 선언

	funcPointer = sum;
	sum(10, 20);
	funcPointer(10, 20);

	void Print(void);

	void (*funcPrint)(void);
	funcPrint = Print;
	//funcPrint = sum;		함수포인터 (변수) = 함수의 주소 대입
	funcPrint();

*/

enum class ButtonState
{
	NONE, DOWN, UP
};

typedef void (*func_t)(int);

class Image;
class Button : public GameNode
{
private:
	ButtonState state;
	Image* img;
	RECT rc;
	POINT pos;

	func_t func;
	int func_arg;

	//void (*func_t)(int); // void Save(int stageNum);

public:
	HRESULT Init(const char* fileName, int posX, int posY);
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetFunc(func_t func, int funcArg)
	{
		this->func = func; this->func_arg = funcArg;
	}
	//void SetFunc(void (*func_t)(int)) { this->func_t = func_t; }

	virtual ~Button() {};
};

