#include "Key.h"


Key::Key(float w, float h, WCHAR* textureFile, int t):
Item(w, h, textureFile),
type(t)
{
}


Key::~Key(void)
{
}

void Key::collideWith(Player* player)
{
	player->addKey(type);
	XMFLOAT4 cent = getCenter();
	worldTranslate(-cent.x, -cent.y-3.0f, -cent.z);
}

void Key::moveToPos(float x)
{
	XMFLOAT4 center = getCenter();
	worldTranslate(-center.x, -center.y, -center.z);
	worldTranslate(x, 2.5f, 0.0f);
}

void Key::advance()
{
	if (getCenter().y > 0.4f)
		worldTranslate(0.0f, -0.05f, 0.0f);
}
