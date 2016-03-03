#include "Heart.h"


Heart::Heart(float w, float h, WCHAR* textureFile, int t):
Item(w, h, textureFile),
type(t)
{
}


Heart::~Heart(void)
{
}

void Heart::collideWith(Player* player)
{
	if (type == 0)
		player->addHeart();
	if (type == 1)
		player->expandHearts();
	XMFLOAT4 cent = getCenter();
	worldTranslate(-cent.x, -cent.y-2.5f, -cent.z);
}

void Heart::moveToPos(float x)
{
	XMFLOAT4 center = getCenter();
	worldTranslate(-center.x, -center.y, -center.z);
	worldTranslate(x, 2.5f, 0.0f);
}

void Heart::advance()
{
	if (getCenter().y > 0.3f)
		worldTranslate(0.0f, -0.02f, 0.0f);
}


