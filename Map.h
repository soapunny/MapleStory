#pragma once
#include "GameNode.h"

enum class MAP_NAME { HENESIS_SQUARE, MUSHROOM_HILL, END_OF_MAP_NAME};

//���� �ٲ�� storage���� Map �̹��� ���ҽ� ���� �޾ƿͼ� Image*�� ��� �ִ´�.
class Image;
class Map : public GameNode
{
private:
	//Data from storage
	MAP_NAME mapName;//key
	string mapImgName;//foreignkey
	string tileCollisionImgName;//foreignkey

	Image* worldmapImg;
	Image* mapImg;
	Image* minimapImg;
	Image* tileCollisionMapImg;
	
public:

};

