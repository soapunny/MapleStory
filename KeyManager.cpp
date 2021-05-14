#include "config.h"
#include "KeyManager.h"

HRESULT KeyManager::Init()
{
    keyUp.set();    //1 �ʱ�ȭ
    keyDown.reset();//0 �ʱ�ȭ

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

        �Լ� ȣ�� ������ ����Ű(VK_...)�� � �������� Ȯ��
        1. 0x0000 -> ���� �����ӿ� �������� ���� ȣ��������� �������� ����
        2. 0x0001 -> ���� �����ӿ� �������� �ְ� ȣ��������� �������� ����
        3. 0x8000 -> ���� �����ӿ� �������� ���� ȣ��������� �����ִ� ����
        4. 0x8001 -> ���� �����ӿ� �������� �ְ� ȣ��������� �����ִ� ����
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
    // �ǽ�. ���� ������.
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
