#pragma once
#include "config.h"

class Image
{
public:
	enum IMAGE_LOAD_KIND
	{
		RESOURCE,		// 프로젝트 자체에 포함 시킬 이미지
		FILE,			// 외부에서 로드할 이미지
		EMPTY,			// 자체 생산 이미지
		END
	};

	typedef struct tagImageInfo
	{
		DWORD resID;		// 리소스의 고유한 ID
		HDC hMemDC;			// 그리기를 주관하는 핸들
		HDC hSubDC;			// 점프에 사용될 DC
		HBITMAP hBitmap;	// 이미지 정보
		HBITMAP hOldBit;	// 기존 이미지 정보
		int width;			// 이미지 가로 크기
		int height;			// 이미지 세로 크기
		BYTE loadType;		// 로드 타입

		// 알파블랜드
		HDC hBlendDC;
		HBITMAP hBlendBit;
		HBITMAP hOldBlendBit;

		// 애니메이션 관련 (프레임데이터)
		vector<int> vMaxFrameX;//가변적인 maxFrameX
		int maxFrameX;//고정 maxFrameX
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
	IMAGE_INFO* imageInfo;		// 이미지 정보 구조체 포인터
	//LPIMAGE_INFO imageInfo;
	bool isTransparent;
	COLORREF transColor;

	BLENDFUNCTION blendFunc;

public:
	// 빈 비트맵 이미지를 만드는 함수
	HRESULT Init(int width, int height);

	// 파일로부터 이미지를 로드하는 함수
	HRESULT Init(const char* fileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	HRESULT BgInit(const char* fileName, const char* subFileName, int width, int height, bool isTransparent = FALSE, COLORREF transColor = FALSE);

	// 파일로부터 이미지를 로드하는 함수
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, int maxFrameY,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);
	// 가변 크기 maxFrameX를 사용 시
	HRESULT Init(const char* fileName, int width, int height,
		int maxFrameX, vector<int> vMaxFrameX, int maxFrameY,
		bool isTransparent = FALSE, COLORREF transColor = FALSE);

	// 화면에 출력
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

