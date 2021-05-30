#pragma once
#include "SkillManager.h"
#include "Managers.h"
#include "Skill.h"
#include "Unit.h"

HRESULT SkillManager::Init()
{
    ImageManager::GetSingleton()->AddImage("luckySeven", "Image/Skill/luckySeven.bmp", 300, 88, 5, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("shuriken", "Image/Projectile/shuriken.bmp", 60, 30, 2, 1, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("flashJump", "Image/Skill/flashJump.bmp", 650, 170, 5, 2, true, RGB(255, 0, 255));

    Skill* skill = new Skill();
    skill->Init("luckySeven");
    mSkillData.insert(make_pair(SKILL_TYPE::LUCKY_SEVEN, skill));


    skill = new Skill();
    skill->Init("flashJump");
    mSkillData.insert(make_pair(SKILL_TYPE::FLASH_JUMP, skill));

    isOn = false;

    return S_OK;
}

void SkillManager::Release()
{
    for (map<SKILL_TYPE, Skill*>::iterator iter = mSkillData.begin(); iter != mSkillData.end(); iter++)
    {
        if (iter->second)
        {
            delete iter->second;
            iter->second = nullptr;
        }
    }
    mSkillData.clear();
}

void SkillManager::Update()
{
    if (currentSkill == nullptr)
        return;

    if (!currentSkill->GetFired())//스킬 사용시간이 지났으면
    {
        currentSkill = nullptr;
        currentSkillType = SKILL_TYPE::END_OF_SKILL_TYPE;//스킬 끝
        isOn = false;
        return;
    }
    currentSkill->Update();
}

void SkillManager::Render(HDC hdc)
{
    if (currentSkill && currentSkill->GetFired())//스킬 사용시간이 지났으면
    {
        currentSkill->Render(hdc);
    }
}

bool SkillManager::FireSkill(SKILL_TYPE skillType, Unit* owner, MOVE_DIRECTION direction)
{
    auto iter = mSkillData.find(skillType);
    if(iter != mSkillData.end() && !iter->second->GetFired())
    {
        //lack of mp
        if (owner->GetMp() < iter->second->GetMpConsumption())
            return false;
        iter->second->FireSkill(owner, direction);
        currentSkill = iter->second;
        currentSkillType = skillType;
        isOn = true;
        return true;
    }
    return false;
}