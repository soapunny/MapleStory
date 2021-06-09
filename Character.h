#pragma once
#include "Unit.h"
#include "CharacterDTO.h"
enum class CHARACTER_FRAME_Y {
	DEFAULT, LEFT_WALK, RIGHT_WALK,
	LEFT_ATTACK_1, LEFT_ATTACK_2, LEFT_ATTACK_3,
	RIGHT_ATTACK_1, RIGHT_ATTACK_2, RIGHT_ATTACK_3,
	JUMP, LYING, HANGING_1, HANGING_2, DEATH
};

enum class Occupation {
	WARRIOR, ARCHER, WIZARD, THIEF, END_OF_OCCUPATION
};

class Image;
class Inventory;
enum class SKILL_TYPE;
class Character:public Unit
{
private:
	Image* nameBox;
	Gdiplus::Image* levelMaple;
	
	CharacterDTO* characterDTO;

	Inventory* inventory;

	float jumpTimer;
	FPOINT priorPos;

	Image* deathImage;
	Image* deathAlert;
	POINT deathFrame;
	FPOINT tombstonePos;
	float deathTimer;

	HFONT font;
	bool isLevelUp;
	float levelUpTimer;

	void HandleDefaultState();
	void HandleWalkingState();
	void HandleAttackState(SKILL_TYPE skillType);
	void HandleJumpingState();
	void HandleJumpingAttackState(SKILL_TYPE skillType);
	void HandleLyingState();
	void HandleLyingAttackState();
	void HandleHangingState();
	void HandleHangingMoveState();
	void HandleSkillState();
	void HandleDeadState();

	void ShowAnimation(UNIT_STATE unitState);
	void DropTheTombstone();
	CHARACTER_FRAME_Y UnitStateToCharacterFrameY(UNIT_STATE unitState);
	
	void RenderUI(HDC hdc);
	void RenderLevelUp(HDC hdc);

public:
	virtual ~Character() {}

	virtual HRESULT Init();
	virtual void Update();
	virtual void Release(); 
	virtual void Render(HDC hdc);

	virtual inline void SetWorldPos(FPOINT worldPos) { this->worldPos = worldPos; characterDTO->SetWorldPos(worldPos.x, worldPos.y); }
	virtual inline void SetWorldPosX(float x) { SetWorldPos(FPOINT{ x, worldPos.y }); }
	virtual inline void SetWorldPosY(float y) { SetWorldPos(FPOINT{ worldPos.x, y }); }
	virtual inline void SetLevel(int level) { this->level = level; characterDTO->SetLevel(level); }
	virtual void SetCharacterDTO(CharacterDTO* characterDTO);
	virtual inline void SetExp(int exp) { 
		int levelUpCnt = 0; 
		if ((levelUpCnt = exp / 10000 - characterDTO->GetExp() / 10000) > 0)
			while (levelUpCnt-- > 0)
				SetLevelUp();
		characterDTO->SetExp(exp);
	}
	virtual void PlusStr();
	virtual void PlusDex();
	virtual void PlusInt();
	virtual void PlusLuk();
	virtual inline void MinusAP() { if (characterDTO->GetCurrAP() <= 0) return; characterDTO->SetCurrAP(characterDTO->GetCurrAP()-1); }
	virtual inline void SetHP(int hp) { this->hp = hp; characterDTO->SetCurrHP(hp); }
	virtual inline void SetMP(int mp) { this->mp = mp; characterDTO->SetCurrMP(mp); }
	virtual void SetLevelUp();
	
	inline virtual float GetCenterToLeft() { return width / 2.0f - 15.0f; };
	inline virtual float GetCenterToRight() { return width / 2.0f - 15.0f; };
	inline virtual float GetCenterToTop() { return height / 2.0f - 10.0f; };
	inline virtual float GetCenterToBottom() { return height / 2.0f; };
	inline virtual Inventory* GetInventory() { return inventory; }
	inline bool GetLeveUp() { return isLevelUp; }
	virtual inline int GetExp() { return characterDTO->GetExp(); }
	inline virtual int GetAP() { return characterDTO->GetCurrAP(); }
	inline virtual int GetStr() { return  characterDTO->GetStr(); }
	inline virtual int GetDex() { return  characterDTO->GetDex(); }
	inline virtual int GetInt() { return  characterDTO->GetInt(); }
	inline virtual int GetLuk() { return  characterDTO->GetLuk(); }
	inline virtual string GetOccupation() 
	{ 
		switch (characterDTO->GetOccupation())
		{
		case Occupation::WARRIOR:
			return "전사";
		case Occupation::ARCHER:
			return "궁수";
		case Occupation::WIZARD:
			return "마법사";
		case Occupation::THIEF:
			return "도적";
		default:
			return "초보자";
		}
	}

	inline virtual string InstanceOf() { return string{ typeid(this).name() }; }

	inline virtual CharacterDTO* GetCharacterDTO() { return characterDTO; }

};

