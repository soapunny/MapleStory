#pragma once
#include "config.h"

class Image
{
public:
	enum IMAGE_LOAD_KIND
	{
		RESOURCE,		// ������Ʈ ��ü�� ���� ��ų �̹���
		FILE,			// �ܺο��� �ε��� �̹���
		EMPTY,			// ��ü ���� �̹���
		END
	};

	typedef struct tagImageInfo
	{
		DWORD resID;		// ���ҽ��� ������ ID
		HDC hMemDC;			// �׸��⸦ �ְ��ϴ� �ڵ�
		HDC hSubDC;			// ������ ���� DC
		HBITMAP hBitmap;	// �̹��� ����
		HBITMAP hOldBit;	// ���� �̹��� ����
		int width;			// �̹��� ���� ũ��
		int height;			// �̹��� ���� ũ��
		BYTE loadType;		// �ε� Ÿ��

		// ���ĺ���
		HDC hBlendDC;
		HBITMAP hBlendBit;
		HBITMAP hOldBlendBit;

		// �ִϸ��̼� ���� (�����ӵ�����)
		vector<int> vMaxFrameX;//�������� maxFrameX
		int maxFrameX;//���� maxFrameX
		int maxFrameY;
		int frameWidth;
		int frameHeight;
		int currFrameX;
		int currFrameY;

		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBitmap = NULL;
			hOldBit = NULL;
			width = 0;
			height = 0;
			loadType = IMAGE_LOAD_KIND::EMPTY;

			hBlendDC = NULL;
			hBlendBit = NULL;
			hOldBlendBit = NULL;

			//maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			currFrameX = 0;
			currFrameY = 0;
		};
	} IMAGE_INFO, * LPIMAGE_INFO;

private:
	IMAGE_INFO* imageInfo;		// �̹��� ���� ����ü ������
	//LPIMAGE_INFO imageInfo;
	bool isTransparent;
	COLORREF transColor;

	BLENDFUNCTION blendFunc;

public:
	// �� ��Ʈ�� �̹����� ����� �Լ�
	HRESULT Init(int width, int height);

	// ���Ϸκ��� �̹����� �ε��ϴ� �Լ�
	HRESULT Init(const char* fileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	HRESULT BgInit(const char* fileName, const char* subFileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	// ���Ϸκ��� �̹����� �ε��ϴ� �Լ�
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);
	// ���� ũ�� maxFrameX�� ��� ��
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, vector<int> vMaxFrameX, int maxFrameY,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);

	// ȭ�鿡 ���
	void Render(HDC hdc, int destX = 0, int destY = 0,
		bool isCenterRenderring = false);
	void RenderMiniMap(HDC hdc, int destX, int destY, int width, int height, FPOINT characterCenterPos);
	void RenderWalkingCamara(HDC hdc, int copyX, int copyY, bool isCenterRenderring = false);
	void FrameRender(HDC hdc, int destX, int destY,
		int currFrameX, int currFrameY, bool isCenterRenderring = false, int size = 1);
	void FrameRender(HDC hdc, int destX, int destY, int width, int height, int currFrameX, int currFrameY, bool isCenterRenderring = false);
	void AlphaRender(HDC hdc, int destX, int destY,
		bool isCenterRenderring = false);


	void Release();

	// get, set
	HDC GetMemDC()
	{
		if (this->imageInfo)
			return this->imageInfo->hMemDC;
		return NULL;
	}

	HDC GetSubDC()
	{
		if (this->imageInfo)
			return this->imageInfo->hSubDC;
		return NULL;
	}

	BLENDFUNCTION* GetBlendFunc() { return &(this->blendFunc); }

	int GetWidth() { return this->imageInfo->width; }
	int GetHeight() { return this->imageInfo->height; }
	int GetFrameWidth() { return this->imageInfo->frameWidth; }
	int GetFrameHeight() { return this->imageInfo->frameHeight; }
	int GetMaxFrameX() { return this->imageInfo->maxFrameX; }
	int GetMaxFrameY() { return this->imageInfo->maxFrameY; }
	int GetVMaxFrameX(int frameY) { return this->imageInfo->vMaxFrameX[frameY]; }

	IMAGE_INFO* const  GetImageInfo() { return this->imageInfo; }
};

