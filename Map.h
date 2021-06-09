#pragma once
#include "GameNode.h"

enum class MAP_NAME { HENESIS_SQUARE, MUSHROOM_HILL, END_OF_MAP_NAME};

//씬이 바뀌면 storage에서 Map 이미지 리소스 정보 받아와서 Image*로 들고 있는다.
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

