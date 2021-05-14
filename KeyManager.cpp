#include "config.h"
#include "KeyManager.h"

HRESULT KeyManager::Init()
{
    keyUp.set();    //1 초기화
    keyDown.reset();//0 초기화

    return S_OK;
}

void KeyManager::Release()
{
    ReleaseSingleton();
}

bool KeyManager::IsOnceKeyDown(int key)
{
    /*
        GetAsyncKeyState(VK_...)

        함수 호출 시점에 가상키(VK_...)가 어떤 상태인지 확인
        1. 0x0000 -> 이전 프레임에 누른적이 없고 호출시점에도 눌려있지 않음
        2. 0x0001 -> 이전 프레임에 누른적이 있고 호출시점에는 눌려있지 않음
        3. 0x8000 -> 이전 프레임에 누른적이 없고 호출시점에는 눌려있는 상태
        4. 0x8001 -> 이전 프레임에 누른적이 있고 호출시점에도 눌려있는 상태
    */

    if (GetAsyncKeyState(key) & 0x8000)
    {
        if (keyDown[key] == false)
        {
            keyDown[key] = true;
            return true;
        }
    }
    else
    {
        keyDown[key] = false;
    }

    return false;
}

bool KeyManager::IsOnceKeyUp(int key)
{
    // 실습. 직접 만들어보자.
    if (GetAsyncKeyState(key) & 0x8000)
    {
        keyUp[key] = false;
    }
    else
    {
        if (keyUp[key] == false)
        {
            keyUp[key] = true;
            return true;
        }
    }
    return false;
}

bool KeyManager::IsStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000)
    {
        return true;
    }

    return false;
}

bool KeyManager::IsKeyUp(int key)
{
    if (! (GetAsyncKeyState(key) & 0x8000) )
    {
        return true;
    }

    return false;
}
