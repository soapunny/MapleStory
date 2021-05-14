#include "SkillManager.h"
#include "Skill.h"

HRESULT SkillManager::Init()
{
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
