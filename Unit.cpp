#include "Unit.h"
#include "ImageManager.h"
#include "SkillManager.h"
#include "Image.h"

HRESULT Unit::Init()
{
    return S_OK;
}

void Unit::Release()
{
}

void Unit::Update()
{
}

void Unit::Render(HDC hdc)
{
    image->FrameRender(hdc, shape.left, shape.top, width, height, frame.x, frame.y, true);
    if(skillManager)
        skillManager->Render(hdc);
}
