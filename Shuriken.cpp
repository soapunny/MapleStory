#include "Shuriken.h"
#include "Managers.h"
#include "Image.h"
#include "Unit.h"

HRESULT Shuriken::Init()
{
    image = ImageManager::GetSingleton()->FindImage("shuriken");
    width = image->GetFrameWidth();
    height = image->GetFrameHeight();
    pos.x = 0.0f;
    pos.y = 0.0f;
    frame.x = 0;
    frame.y = 0;
    moveSpeed = 300.0f;
    angle = 0.0f;
    range = 400.0f;
    isFired = false;

    target = nullptr;

    return S_OK;
}

void Shuriken::Release()
{
}

void Shuriken::Update()
{
    if (!isFired)
        return;

    float elapsedTime = TimerManager::GetSingleton()->GetElapsedTime();
    if (target && target->GetUnitState() != UNIT_STATE::DEAD_STATE && target->GetUnitState() != UNIT_STATE::END_OF_UNIT_STATE)
    {
        if (pos.x > target->GetLocalPos().x)//수리검이 몬스터보다 오른쪽에 있을 때
        {
            if (pos.y >= target->GetLocalPos().y - target->GetCenterToTop())//수리검이 몬스터보다 위에 있을 때
            {
                angle -= elapsedTime * 3.0f;
            }
            else if(pos.y <= target->GetLocalPos().y + target->GetCenterToBottom())
            {
                angle += elapsedTime * 3.0f;
            }
        }
        else
        {
            if (pos.y >= target->GetLocalPos().y - target->GetCenterToTop())//수리검이 몬스터보다 위에 있을 때
            {
                angle += elapsedTime * 3.0f;
            }
            else if (pos.y <= target->GetLocalPos().y + target->GetCenterToBottom())
            {
                angle -= elapsedTime * 3.0f;
            }
        }
        pos.x += cos(angle) * moveSpeed * elapsedTime;
        pos.y -= sin(angle) * moveSpeed * elapsedTime;
        //TODO range에서 이동한 거리 빼주기
    }
    else
    {
        pos.x += cos(angle) * moveSpeed * elapsedTime;
        pos.y -= sin(angle) * moveSpeed * elapsedTime;
    }
    range -= moveSpeed * elapsedTime;
    if (range <= 0.0f)
    {
        isFired = false;
        range = 300.0f;//init
        target = nullptr;
        return;
    }

    //UpdateAnimation
    timer += TimerManager::GetSingleton()->GetElapsedTime();
    if (timer >= 0.1f)
    {
        frame.x++;
        if (frame.x >= image->GetMaxFrameX())
            frame.x = 0;
        timer = 0.0f;
    }

}

void Shuriken::Render(HDC hdc)
{
    if (isFired)
    {
        //Rectangle(hdc, pos.x - width / 2.0f, pos.y - height / 2.0f, pos.x + width / 2.0f, pos.y + height / 2.0f);
        image->FrameRender(hdc, pos.x, pos.y, frame.x, frame.y, true);
    }
}
