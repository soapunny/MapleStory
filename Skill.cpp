#include "Skill.h"
#include "Image.h"

HRESULT Skill::Init()
{
    name = "luckySeven";
    damage = 10;
    damageRatio = 1.5f;
    hpRecovery = 0.0f;
    mpRecovery = 0.0f;
    hpConsumption = 0.0f;
    mpConsumption = 15.0f;

    image1 = ImageManager::GetSingleton()->FindImage(name);
    if(needShuriken)
        image2 = ImageManager::GetSingleton()->FindImage("shuriken");
    
    frame.x = 0;
    frame.y = 0;
    weaponFrame.x = 0;
    weaponFrame.y = 0;

    return S_OK;
}

void Skill::Release()
{
}

void Skill::Update()
{
    timer += TimerManager::GetSingleton()->GetElapsedTime();
    if (isFired && timer >= 0.1)
    {
        frame.x++;
        weaponFrame.x++;
        shape.left -= 50;
        shape.right = shape.left + image1->GetFrameWidth();
        if (frame.x >= 5)
        { 
            frame.x = 0;
            isFired = false;
        }
        timer = 0.0f;
    }
}

void Skill::Render(HDC hdc)
{
    if (isFired)
    {
        image1->FrameRender(hdc, ownerPos.x, shape.top, frame.x, frame.y, false);
        image2->FrameRender(hdc, shape.left, shape.top, weaponFrame.x % 2, weaponFrame.y, false);
    }
}

void Skill::FireSkill(RECT ownerShape)
{
    this->ownerPos.x = ownerShape.left - 50;

    this->shape.left = ownerShape.left-50;
    this->shape.right = shape.left + image1->GetFrameWidth();
    this->shape.top = ownerShape.top;
    this->shape.bottom = this->shape.top + image1->GetFrameHeight();

    isFired = true;
}
