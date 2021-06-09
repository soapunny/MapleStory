#pragma once
#include "InGameScene.h"

class CharDataTellerMachine;
class CharacterDTO;
class FirstTownScene : public InGameScene
{
private:
	CharDataTellerMachine* charDataTellerMachine;
	map<string, CharacterDTO*>* mCharacterData;


	virtual void MoveSceneUsingPortal();
public:
	virtual HRESULT Init();			// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();			// �޸� ����
	virtual void Update();			// ������ ������ ���� ���� ���� (������ ����)
	virtual void Render(HDC hdc);	// ������ ������ ��� (�̹���, �ؽ�Ʈ ��)

	virtual ~FirstTownScene() {}
};

