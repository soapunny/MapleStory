#pragma once
#include "Managers.h"
#include "Image.h"

HRESULT ImageManager::Init()
{
    return S_OK;
}

void ImageManager::Release()
{
    // 모든 이미지에 대한 메모리 해제가 이루어 진다.
    map<string, Image*>::iterator it;
    for (it = mImageDatas.begin(); it != mImageDatas.end(); it++)
    {
        if ((it->second))
        {
            (it->second)->Release();
            delete (it->second);
            (it->second) = nullptr;
        }
    }
    mImageDatas.clear();

    ReleaseSingleton();
}

Image* ImageManager::AddImage(string key, const char* fileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->Init(fileName, width, height, isTransparent, transColor)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

Image* ImageManager::AddImage(string key, const char* fileName, const char* subFileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->BgInit(fileName, subFileName, width, height, isTransparent, transColor)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

Image* ImageManager::AddImage(string key, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor, bool isDualDC)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->Init(fileName, width, height, maxFrameX, maxFrameY, isTransparent, transColor, isDualDC)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

Image* ImageManager::AddImage(string key, const char* fileName, int width, int height, int maxFrameX, vector<int> vMaxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor, float renderRatio)
{
    Image* image = nullptr;

    // 맵에 키에 해당하는 데이터가 있으면 추가생성없이 리턴
    image = FindImage(key);
    if (image)
    {
        return image;
    }

    // 없으면 생성 후 맵에 추가
    image = new Image();
    if (FAILED(image->Init(fileName, width, height, maxFrameX, vMaxFrameX, maxFrameY, isTransparent, transColor, renderRatio)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mImageDatas.insert(make_pair(key, image));
    return image;
}

void ImageManager::DeleteImage(string key)
{
    map<string, Image*>::iterator it;
    it = mImageDatas.find(key);

    if (it == mImageDatas.end())
    {
        return;
    }

    // 맵에 key, value pair가 있는 상태
    // value에 해당하는 Image* 데이터도 메모리 해제해야 한다.
    (it->second)->Release();
    delete (it->second);
    (it->second) = nullptr;

    mImageDatas.erase(it);
}

Image* ImageManager::FindImage(string key)
{
    map<string, Image*>::iterator it;
    it = mImageDatas.find(key);

    if (it == mImageDatas.end())
    {
        return nullptr;
    }

    return it->second;
}
