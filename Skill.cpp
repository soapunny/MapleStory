#pragma once
#include "Skill.h"
#include "Managers.h"
#include "Image.h"
#include "Unit.h"
#include "Shuriken.h"

HRESULT Skill::Init(string skillName)
{
    name = skillName;
    damage = 100;
    damageRatio = 1.5f;
    hpRecovery = 0.0f;
    mpRecovery = 0.0f;
    hpConsumption = 0.0f;
    mpConsumption = 22.0f;

    skillDelay = 1.2f;
    skillRange = 200.0f;
    attackCount = 2;
    skillTimer = 0.0f;

    needShuriken = true;
    shurikenCnt = 2;

    image = ImageManager::GetSingleton()->FindImage(name);
    if (needShuriken)
    {
        vShurikens.resize(shurikenCnt);
        for (int i = 0; i < vShurikens.size(); i++)
        {
            vShurikens[i] = new Shuriken();
            vShurikens[i]->Init();
        }
    }
    
    frame.x = 0;
    frame.y = 0;


    return S_OK;
}

void Skill::Release()
{
    for (Shuriken* shuriken : vShurikens)
    {
        if(shuriken)
            SAFE_RELEASE(shuriken);
    }
    vShurikens.clear();
}

void Skill::Update()
{
    if (!isFired)
        return;
    
    float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
    timer += elapsedTime;
    if (timer >= 0.2f && skillTimer >= 0.2f )
    {
        timer = 0.0f;
        frame.x++;
        if (frame.x >= image->GetMaxFrameX())
            frame.x = 0;
    }
    //수리검 업데이트
    if (needShuriken && skillTimer>=0.3f)
    {
        for (Shuriken* shuriken : vShurikens)
        {
            shuriken->Update();
        }
    }

    skillTimer += elapsedTime;
    if (skillTimer >= skillDelay)
    {
        isFired = false;
        frame.x = 0;
        frame.y = 0;
        ownerPos.x = 0.0f;
        ownerPos.y = 0.0f;
        skillTimer = 0.0f;
        for (Shuriken* shuriken : vShurikens)
        {
            shuriken->SetRange(300.0f);
            shuriken->SetFired(false);
        }
    }
}

void Skill::Render(HDC hdc)
{
    if (!isFired)
        return;

    if (needShuriken && skillTimer >= 0.3f)
    {
        image->FrameRender(hdc, ownerPos.x, ownerPos.y, frame.x, frame.y, false);
        for (Shuriken* shuriken : vShurikens)
        {
            shuriken->Render(hdc);
        }
    }
}

void Skill::FireSkill(Unit* owner, MOVE_DIRECTION direction)
{
    if (direction == MOVE_DIRECTION::MOVE_LEFT)
    {
        this->ownerPos.x = owner->GetShape().left - 50;
        frame.y = 0;
    }
    else
    {
        this->ownerPos.x = owner->GetShape().right + 50;
        frame.y = 1;
    }
    this->ownerPos.y = owner->GetShape().top + 50.0f;
    this->direction = direction;
    owner->SetMP(owner->GetMp() - mpConsumption);


    if (needShuriken)
    {
        for (int i = 0; i < vShurikens.size(); i++)
        {
            if (vShurikens[i]->GetFired())
                continue;
            if (direction == MOVE_DIRECTION::MOVE_LEFT)
            {
                vShurikens[i]->SetPos(FPOINT{ (float)owner->GetShape().left - 30 * i, (float)owner->GetShape().top + image->GetFrameHeight() + 10 * i });
                vShurikens[i]->SetAngle(PI);
            }
            else
            {
                vShurikens[i]->SetPos(FPOINT{ (float)owner->GetShape().right + 30 + 30 * i, (float)owner->GetShape().top + image->GetFrameHeight() + 10 * i });
                vShurikens[i]->SetAngle(0);
            }
            vShurikens[i]->SetFired(true);
            vShurikens[i]->SetTartget(nullptr);
        }
    }

    isFired = true;
}
