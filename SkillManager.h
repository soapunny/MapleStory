#pragma once
#include "GameNode.h"

class Skill;
class SkillManager:public GameNode
{
private:
	map<string, Skill*> mSkillData;

public:
	virtual HRESULT Init();			// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();			// 메모리 해제
	virtual void Update();			// 프레임 단위로 게임 로직 실행 (데이터 변동)
	virtual void Render(HDC hdc);	// 프레임 단위로 출력 (이미지, 텍스트 등)

	void FireSkill(string skillName, RECT playerShape);
	
	float GetSkillDelay(string skillName);

	virtual ~SkillManager() {}
};

