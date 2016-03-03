#include "Item.h"


Item::Item(float w, float h, WCHAR* textureFile):
QuadTexturedModel(w, h, textureFile),
widthX(w),
heightY(h)
{
	radius = (w + h) / 4;
	worldTranslate(0.0f, -3.0f, 0.0f);
}


Item::~Item(void)
{
}

void Item::getCollisionInfo(float& xPos, float& yPos, float& rad)
{
	XMFLOAT4 center = getCenter();
	xPos = center.x;
	yPos = center.y;
	rad = radius;
}