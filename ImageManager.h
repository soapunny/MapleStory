#pragma once
#include "config.h"
#include "Singleton.h"

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mImageDatas;

public:
	HRESULT Init();
	void Release();

	Image* AddImage(string key, const char* fileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	Image* AddImage(string key, const char* fileName, const char* subFileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	Image* AddImage(string key, const char* fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent = FALSE, COLORREF transColor = FALSE, bool isDualDC = FALSE);

	Image* AddImage(string key, const char* fileName, int width, int height, int maxFrameX, vector<int> vMaxFrameX, int maxFrameY, bool isTransparent = FALSE, COLORREF transColor = FALSE, float renderRatio = 1.0f);

	void DeleteImage(string key);
	Image* FindImage(string key);

};

