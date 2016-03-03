#pragma once

#include "quadTexturedModel.h"
#include "player.h"
class Player;

class Item : public QuadTexturedModel
{
public:
	Item(float w, float h, WCHAR* textureFile);
	virtual ~Item();
	void getCollisionInfo(float&, float&, float&);

	virtual void collideWith(Player*)=0;
	virtual void moveToPos(float) = 0;
	virtual void advance() = 0;

private:
	float widthX;
	float heightY;
	float radius;
};

