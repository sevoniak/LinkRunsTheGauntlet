#pragma once
#include "item.h"

class Key : public Item
{
public:
	Key(float w, float h, WCHAR* textureFile, int t);
	virtual ~Key(void);

	virtual void collideWith(Player*);
	void moveToPos(float);
	void advance();

private:
	int type;
};

