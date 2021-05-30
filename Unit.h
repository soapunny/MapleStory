#pragma once
#include "GameNode.h"

enum class MOVE_DIRECTION { MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN, END_OF_MOVE_DIRECTION };
enum class UNIT_STATE{DEFAULT_STATE, WALKING_STATE, ATTACK_STATE, JUMPING_STATE, JUMPING_ATTACK_STATE, LYING_STATE, LYING_ATTACK_STATE, HANGING_STATE, HANGING_MOVE_STATE, SKILL_STATE, HIT_STATE, DEAD_STATE, END_OF_UNIT_STATE};
enum class JUMPING_STATE{JUMPING_UP, JUMPING_DOWN, JUST_LANDED, END_OF_JUMPING_STATE};
enum class BLOCKED_STATE{LEFT, RIGHT, TOP, BOTTOM, END_OF_BLOCKED_STATE};
enum class HANGING_STATE{BOTTOM_ALLOWED, ALLOWED, NOT_ALLOWED, HANGED, END_OF_HANGING_STATE};
enum class PORTAL_STATE { ALLOWED, NOT_ALLOWED, USING, END_OF_PORTAL_STATE };

typedef struct _recoveryInfo
{
	//HP
	int hpRecovery;
	bool isHpRecovered;
	float hpRecoveryTimer;
	float hpRecoveryTime;
	float hpRecoveryDuration;
	COLORREF hpRecoveryColor; 

	//MP
	int mpRecovery;
	bool isMpRecovered;
	float mpRecoveryTimer;
	float mpRecoveryTime;
}RecoveryInfo;

class Image;
class SkillManager;
class DamageQueue;
class Unit : public GameNode
{
protected:
	Image* image;
	Image* attackImage;

	string name;
	float moveSpeed;
	int level;
	MOVE_DIRECTION moveDirection;
	UNIT_STATE state;
	JUMPING_STATE jumpingState;
	FPOINT center;
	FPOINT localPos;
	float width;
	float height;
	POINT frame;
	RECT shape;
	float velocity;
	float jumpAngle;
	bool isSkillOn;

	SkillManager* skillManager;
	
	//Damage
	int damage;
	float defense;
	float protection;
	float criticalPercentage;
	float criticalDamage;
	DamageQueue* damageQueue;

	//HP,MP
	int maxHp, maxMp;
	int hp;
	int mp;
	RecoveryInfo* recoveryInfo;
	DamageQueue* recoveryQueue;
	char szText[128] = "";

	bool isJumpingDown;
	bool isHangingPossible;

	BLOCKED_STATE blockedState;
	HANGING_STATE hangingState;
	PORTAL_STATE portalState;

	//Hit
	bool isHit;
	float hitTimer;
	float maxHitTime;

	//Attack
	bool isAttack;
	RECT attackBox;
	float attackRange;
	POINT attackFrame;

	virtual void InitHitTimer(bool isHit, float hitTimer, float maxHitTime);
	virtual void CountHitTimer();

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Render(HDC hdc, int pasteX, int pasteY);

	virtual void Move();
	virtual inline void CheckAlive() { if (hp <= 0) { hp = 0; state = UNIT_STATE::DEAD_STATE; } }

	virtual inline void SetLevel(int level) { this->level = level; }
	virtual inline void SetDefense(float defense) { this->defense = defense; }
	virtual inline void SetProtection(float protection) { this->protection = protection; }
	virtual inline void SetCriticalPercentage(float criticalPercentage) { this->criticalPercentage = criticalPercentage; }
	virtual inline void SetCriticalDamage(float criticalDamage) { this->criticalDamage = criticalDamage; }
	virtual inline void SetName(string name) { this->name = name; }
	virtual inline void SetMoveSpeed(float moveSpeed) { this->moveSpeed = moveSpeed; }
	virtual inline void SetMoveDirection(MOVE_DIRECTION moveDirection) { this->moveDirection = moveDirection; }
	virtual inline void SetHp(int hp) { this->hp = hp; }
	virtual inline void SetMaxHp(int maxHp) { this->maxHp = maxHp; }
	virtual inline void SetMp(int mp) { this->mp = mp; }
	virtual inline void SetCenter(FPOINT center) { this->center = center; }
	virtual inline void SetLocalPos(FPOINT localPos) { this->localPos = localPos; }
	virtual inline void SetCenterX(float x) { this->center.x = x; }
	virtual inline void SetCenterY(float y) { this->center.y = y; }
	virtual inline void SetDamage(int damage) { this->damage = damage; }
	virtual inline void SetShape(RECT shape) { this->shape = shape; }
	virtual inline void SetJumpingDown(bool isJumpingDown) { this->isJumpingDown = isJumpingDown; }
	virtual inline void SetJumpingState(JUMPING_STATE jumpingState) { this->jumpingState = jumpingState; }
	virtual inline void SetBlockedState(BLOCKED_STATE blockedState) { this->blockedState = blockedState; }
	virtual inline void SetState(UNIT_STATE state) { this->state = state; }
	virtual inline void SetHangingState(HANGING_STATE hangingState) { this->hangingState = hangingState; }
	virtual inline void SetJumpAngle(float angle) { this->jumpAngle = angle; }
	virtual inline void SetPortalState(PORTAL_STATE portalState) { this->portalState = portalState; }
	virtual inline void SetHit(bool isHit) { this->isHit = isHit; }

	virtual inline string GetName() { return name; }
	virtual inline int GetLevel() { return level; }
	virtual inline RECT GetShape() { return shape; }
	virtual inline FPOINT GetCenter() { return center; }
	virtual inline FPOINT GetLocalPos() { return localPos; }
	virtual inline float GetWidth() { return width; }
	virtual inline float GetHeight() { return height; }
	virtual inline bool GetJumpingDown() { return isJumpingDown; }
	virtual inline JUMPING_STATE GetJumpingState() { return jumpingState; }
	virtual inline BLOCKED_STATE GetBlockedState() { return blockedState; }
	virtual inline UNIT_STATE GetUnitState() { return state; }
	virtual inline HANGING_STATE GetHangingState() { return hangingState; }
	virtual inline bool GetMoveSpeed() { return moveSpeed; }
	virtual inline float GetJumpAngle() { return jumpAngle; }
	virtual inline PORTAL_STATE GetPortalState() { return portalState; }
	virtual inline int GetHp() { return hp; }
	virtual inline int GetMaxHp() { return maxHp; }
	virtual inline int GetMp() { return mp; }
	virtual inline int GetMaxMp() { return maxMp; }
	virtual inline int GetDamage() { return damage; }
	virtual inline float GetDefense() { return defense; }
	virtual inline float GetProtection() { return protection; }
	virtual inline float GetCriticalPercentage() { return criticalPercentage; }
	virtual inline float GetCriticalDamage() { return criticalDamage; }
	virtual inline bool GetHit() { return this->isHit; }
	virtual inline float GetHitTimer() { return hitTimer; }
	virtual inline DamageQueue* GetDamageQueue() { return damageQueue; }
	virtual inline DamageQueue* GetRecoveryQueue() { return recoveryQueue; }
	virtual inline SkillManager* GetSkillManager() { return skillManager; }
	virtual RECT GetAttackBox() { return attackBox; }
	virtual bool GetAttack() { return isAttack; }
	virtual inline POINT GetAttackFrame() { return frame; }

	inline virtual float GetCenterToLeft() { return width / 2.0f; };
	inline virtual float GetCenterToRight() { return width / 2.0f; };
	inline virtual float GetCenterToTop() { return height / 2.0f; };
	inline virtual float GetCenterToBottom() { return height / 2.0f; };

	//Recovery
	virtual void InitHpRecovery(int hpRecovery, bool isHpRecovered = false, float hpRecoveryTimer = 0.0f, float hpRecoveryTime = 0.0f, float hpRecoveryDuration = 0.0f, COLORREF hpRecoveryColor = RGB(17, 204, 255));
	virtual void InitMpRecovery(int mpRecovery, bool isMpRecovered = false, float mpRecoveryTimer = 0.0f, float mpRecoveryTime = 0.0f);

	virtual void CountRecovery();

	virtual ~Unit() {};
};

