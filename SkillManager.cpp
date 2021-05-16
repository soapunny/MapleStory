#include "SkillManager.h"
#include "Skill.h"

HRESULT SkillManager::Init()
{
    ImageManager::GetSingleton()->AddImage("luckySeven", "Image/Skill/luckySeven.bmp", 300, 88, 5, 2, true, RGB(255, 0, 255));
    ImageManager::GetSingleton()->AddImage("shuriken", "Image/Projectile/shuriken.bmp", 60, 30, 2, 1, true, RGB(255, 0, 255));

    Skill* skill = new Skill();
    skill->Init();
    
    mSkillData.insert(make_pair("luckySeven", skill));


    return S_OK;
}

void SkillManager::Release()
{
    for (map<string, Skill*>::iterator iter = mSkillData.begin(); iter != mSkillData.end(); iter++)
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
    for (auto skillPair : mSkillData)
    {
        if (skillPair.second && skillPair.second->GetFired())
        {
            skillPair.second->Update();
        }
    }
}

void SkillManager::Render(HDC hdc)
{
    for (auto skillPair : mSkillData)
    {
        if (skillPair.second)
        {
            skillPair.second->Render(hdc);
        }
    }
}

void SkillManager::FireSkill(string skillName, RECT playerShape)
{
    auto iter = mSkillData.find(skillName);
    if(iter != mSkillData.end() && !iter->second->GetFired())
        iter->second->FireSkill(playerShape);
}

float SkillManager::GetSkillDelay(string skillName)
{
    auto iter = mSkillData.find(skillName);
    iter->second->GetSkillDelay();
    return 0.0f;
}
