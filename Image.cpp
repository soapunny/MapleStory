#include "Image.h"

HRESULT Image::Init(int width, int height)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::EMPTY;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = FALSE;
    this->transColor = FALSE;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height,
    bool isTransparent/* = FALSE*/, COLORREF transColor/* = FALSE*/)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    // ���� ����
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc,
        imageInfo->width, imageInfo->height);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC,
        imageInfo->hBlendBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    this->blendFunc.AlphaFormat = 0;
    this->blendFunc.BlendFlags = 0;
    this->blendFunc.BlendOp = AC_SRC_OVER;
    this->blendFunc.SourceConstantAlpha = 155;

    return S_OK;
}

HRESULT Image::BgInit(const char* fileName, const char* subFileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hSubDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, subFileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hSubDC, imageInfo->hBitmap);

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    // ���� ����
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc,
        imageInfo->width, imageInfo->height);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC,
        imageInfo->hBlendBit);

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    this->blendFunc.AlphaFormat = 0;
    this->blendFunc.BlendFlags = 0;
    this->blendFunc.BlendOp = AC_SRC_OVER;
    this->blendFunc.SourceConstantAlpha = 155;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor, bool isDualDC )
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);
    if (isDualDC)
    {
        imageInfo->hSubDC = CreateCompatibleDC(hdc);
        imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
        imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hSubDC, imageInfo->hBitmap);
        isRenderReady = false;
    }
    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = 0;
    imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;
}

HRESULT Image::Init(const char* fileName, int width, int height, int maxFrameX, vector<int> vMaxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor, float renderRatio)
{
    HDC hdc = GetDC(g_hWnd);

    imageInfo = new IMAGE_INFO();
    imageInfo->resID = 0;
    imageInfo->hMemDC = CreateCompatibleDC(hdc);
    imageInfo->hBitmap =
        (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    imageInfo->hOldBit =
        (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

    // ���� ����
    imageInfo->hBlendDC = CreateCompatibleDC(hdc);
    imageInfo->hBlendBit = CreateCompatibleBitmap(hdc, width, height);
    imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBit);
    this->blendFunc.AlphaFormat = 0;
    this->blendFunc.BlendFlags = 0;
    this->blendFunc.BlendOp = AC_SRC_OVER;
    this->blendFunc.SourceConstantAlpha = 155;

    imageInfo->width = width;
    imageInfo->height = height;
    imageInfo->renderRatio = renderRatio;
    imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

    imageInfo->maxFrameX = maxFrameX;
    imageInfo->vMaxFrameX = vMaxFrameX;
    imageInfo->maxFrameY = maxFrameY;
    imageInfo->frameWidth = width / maxFrameX;
    imageInfo->frameHeight = height / maxFrameY;
    imageInfo->currFrameX = 0;
    imageInfo->currFrameY = 0;

    ReleaseDC(g_hWnd, hdc);

    if (imageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    this->isTransparent = isTransparent;
    this->transColor = transColor;

    return S_OK;
}

void Image::Render(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,
            x, y,
            imageInfo->width, imageInfo->height,

            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor
        );
    }
    else
    {
        // bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
        BitBlt(
            hdc,                // ���� ������ DC
            x, y,               // ���� ���� ��ġ
            imageInfo->width,   // �������� ����� ����ũ��
            imageInfo->height,  // �������� ����� ����ũ��
            imageInfo->hMemDC,  // ���� DC
            0, 0,               // �������� ���� ���� ��ġ
            SRCCOPY             // ���� �ɼ�
        );
    }

}

void Image::Render(HDC hdc, int destX, int destY, int width, int height, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,
            x, y,
            width, height,

            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor
        );
    }
    else
    {
        // bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
        StretchBlt(
            hdc,                // ���� ������ DC
            x, y,               // ���� ���� ��ġ
            width, height,

            imageInfo->hMemDC,
            0,0,
            imageInfo->width,   // �������� ����� ����ũ��
            imageInfo->height,  // �������� ����� ����ũ��
            SRCCOPY             // ���� �ɼ�
        );
    }

}

void Image::RenderMiniMap(HDC hdc, int destX, int destY, int width, int height, FPOINT characterCenterPos)
{
    int x = destX;
    int y = destY;

    Rectangle(hdc, destX, destY, destX + width, destY + height);

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,
            x, y,
            width, height,

            imageInfo->hMemDC,
            0, 0,
            imageInfo->width, imageInfo->height,
            transColor
        );
    }
    else
    {
        // bitmap �� �ִ� �̹��� ������ �ٸ� ��Ʈ�ʿ� ����
        StretchBlt(
            hdc,                // ���� ������ DC
            x, y,               // ���� ���� ��ġ
            width, height,
            imageInfo->hMemDC,
            0, 0,
            imageInfo->width,   // �������� ����� ����ũ��
            imageInfo->height,  // �������� ����� ����ũ��
            SRCCOPY             // ���� �ɼ�
        );
    }

    Ellipse(hdc, characterCenterPos.x - 5, characterCenterPos.y - 5, characterCenterPos.x + 5, characterCenterPos.y + 5);
}

void Image::RenderWalkingCamara(HDC hdc, int copyX, int copyY, bool isCenterRenderring)
{
    int x = 0;
    int y = 0;

    if (isCenterRenderring)
    {
        x = x - (WINSIZE_X / 2);
        y = y - (WINSIZE_Y / 2);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,                // ������ DC
            0, 0,               // ���� ��ġ
            WINSIZE_X,
            WINSIZE_Y,

            imageInfo->hMemDC,  // ���� DC
            copyX,  // ���� X ��ġ
            copyY, // ���� Y ��ġ
            WINSIZE_X,  // ���� ũ��
            WINSIZE_Y,
            transColor  // ������ ����
        );
    }
    else
    {
        StretchBlt(
            hdc,                // ������ DC
            0, 0,               // ���� ��ġ
            WINSIZE_X,
            WINSIZE_Y,

            imageInfo->hMemDC,  // ���� DC
            copyX,  // ���� X ��ġ
            copyY, // ���� Y ��ġ
            WINSIZE_X,  // ���� ũ��
            WINSIZE_Y,
            SRCCOPY);
    }
}

void Image::FrameRender(HDC hdc, int destX, int destY, int currFrameX, int currFrameY, bool isCenterRenderring)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth * imageInfo->renderRatio / 2.0f);
        y = destY - (imageInfo->frameHeight * imageInfo->renderRatio / 2.0f);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,                // ������ DC
            x, y,               // ���� ��ġ
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,  // ���� ũ��

            imageInfo->hMemDC,  // ���� DC
            imageInfo->frameWidth * imageInfo->currFrameX,  // ���� X ��ġ
            imageInfo->frameHeight * imageInfo->currFrameY, // ���� Y ��ġ
            imageInfo->frameWidth, imageInfo->frameHeight,  // ���� ũ��
            transColor  // ������ ����
        );
    }
    else
    {
        StretchBlt(hdc,
            x, y,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            SRCCOPY);
    }
}

void Image::FrameRender(HDC hdc, int destX, int destY, int width, int height, int currFrameX, int currFrameY, bool isCenterRenderring)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth * imageInfo->renderRatio / 2);
        y = destY - (imageInfo->frameHeight * imageInfo->renderRatio / 2);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,                // ������ DC
            x, y,               // ���� ��ġ
            width * imageInfo->renderRatio,
            height * imageInfo->renderRatio,  // ���� ũ��

            imageInfo->hMemDC,  // ���� DC
            imageInfo->frameWidth * imageInfo->currFrameX,  // ���� X ��ġ
            imageInfo->frameHeight * imageInfo->currFrameY, // ���� Y ��ġ
            imageInfo->frameWidth, imageInfo->frameHeight,  // ���� ũ��
            transColor  // ������ ����
        );
    }
    else
    {
        StretchBlt(hdc,
            x, y,
            width,
            height,
            imageInfo->hMemDC,
            imageInfo->frameWidth * imageInfo->currFrameX,
            imageInfo->frameHeight * imageInfo->currFrameY,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            SRCCOPY);
    }
}

void Image::AlphaRender(HDC hdc, int destX, int destY, bool isCenterRenderring)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->width / 2);
        y = destY - (imageInfo->height / 2);
    }

    // 1. ������ DC(hdc)�� �׷��� �ִ� ������ BlendDC�� ����
    BitBlt(imageInfo->hBlendDC, 0, 0, imageInfo->width, imageInfo->height,
        hdc, x, y, SRCCOPY);

    //GdiTransparentBlt(imageInfo->)

    // 2. ����� �̹��� DC�� ������ BlendDC�� ������ ������ �����ϸ鼭 ����


    // 3.
    AlphaBlend(hdc, x, y, imageInfo->width, imageInfo->height,
        imageInfo->hBlendDC, 0, 0, imageInfo->width, imageInfo->height, blendFunc);
}

void Image::AlphaFrameRender(HDC hdc, int destX, int destY, int currFrameX, int currFrameY, bool isCenterRenderring)
{
    imageInfo->currFrameX = currFrameX;
    imageInfo->currFrameY = currFrameY;

    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth * imageInfo->renderRatio / 2.0f);
        y = destY - (imageInfo->frameHeight * imageInfo->renderRatio / 2.0f);
    }
    PatBlt(imageInfo->hBlendDC, 0, 0, imageInfo->width, imageInfo->height, WHITENESS);
    if (isTransparent)
    {//TODO ���� ���ĺ��� �ϼ���Ű��
        // ��� ����
        GdiTransparentBlt(
            imageInfo->hBlendDC,// ������ DC
            0, 0,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,

            hdc,
            x, y,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,
            transColor
        );

        // ĳ���� ����
        GdiTransparentBlt(
            imageInfo->hBlendDC,// ������ DC
            0, 0,               
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,  // ���� ũ��

            imageInfo->hMemDC,
            currFrameX * imageInfo->frameWidth,
            currFrameY * imageInfo->frameHeight,
            imageInfo->frameWidth, imageInfo->frameHeight,
            transColor  // ������ ����
        );
        //���ĺ���
        AlphaBlend(
            hdc,// ������ DC
            x, y,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,  // ���� ũ��

            imageInfo->hBlendDC,// ������ DC
            0, 0,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight* imageInfo->renderRatio,
            blendFunc);
    }
    else
    {
        AlphaBlend(
            hdc,// ������ DC
            x, y,
            imageInfo->frameWidth * imageInfo->renderRatio,
            imageInfo->frameHeight * imageInfo->renderRatio,  // ���� ũ��

            imageInfo->hBlendDC,// ������ DC
            0, 0,
            imageInfo->frameWidth,
            imageInfo->frameHeight,
            blendFunc);
    }

}

void Image::DamageRender(HDC hdc, int destX, int destY, bool isCenterRenderring, bool isTransparent)
{
    int x = destX;
    int y = destY;
    if (isCenterRenderring)
    {
        x = destX - (imageInfo->frameWidth / 2.0f);
        y = destY - (imageInfo->frameHeight / 2.0f);
    }

    if (isTransparent)
    {
        // Ư�� ������ ���� �����ϴ� �Լ�
        GdiTransparentBlt(
            hdc,                // ������ DC
            x, y,               // ���� ��ġ
            imageInfo->subDCWidth,
            imageInfo->subDCHeight,  // ���� ũ��

            imageInfo->hSubDC,  // ���� DC
            0, 0,
            imageInfo->subDCWidth,
            imageInfo->subDCHeight,  // ���� ũ��
            transColor  // ������ ����
        );
    }
    else
    {
        StretchBlt(hdc,
            x, y,
            imageInfo->subDCWidth,
            imageInfo->subDCHeight,  // ���� ũ��

            imageInfo->hSubDC,  // ���� DC
            0, 0,
            imageInfo->subDCWidth,
            imageInfo->subDCHeight,  // ���� ũ��
            SRCCOPY);
    }
}

void Image::SetNumberRender(int* damageArray, int damageArraySize)
{
    if (damageArraySize == 0){
        isRenderReady = false;
        return;
    }
    PatBlt(imageInfo->hSubDC, 0, 0, WINSIZE_X, imageInfo->frameHeight, WHITENESS);

    imageInfo->subDCWidth = damageArraySize * imageInfo->frameWidth;
    imageInfo->subDCHeight = imageInfo->frameHeight;

    for(int i=0; i<damageArraySize; i++)
    {
        StretchBlt(
            imageInfo->hSubDC,
            i* imageInfo->frameWidth,
            0,
            imageInfo->frameWidth,
            imageInfo->frameHeight,  // ���� ũ��
            imageInfo->hMemDC,  // ���� DC
            damageArray[i]* imageInfo->frameWidth,
            0,
            imageInfo->frameWidth,
            imageInfo->frameHeight,  // ���� ũ��
            SRCCOPY);
    }
    isRenderReady = true;
}

void Image::Release()
{
    if (imageInfo)
    {
        SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);
        DeleteObject(imageInfo->hBitmap);
        DeleteDC(imageInfo->hMemDC);

        if (imageInfo->hBlendDC != NULL)
        {
            SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit);
            DeleteObject(imageInfo->hBlendBit);
            DeleteDC(imageInfo->hBlendDC);
        }
        if (imageInfo->hSubDC != NULL)
        {
            DeleteDC(imageInfo->hSubDC);
        }

        delete imageInfo;
        imageInfo = nullptr;
    }
}
