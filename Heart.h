#pragma once
#include "item.h"
class Heart : public Item
{
public:
	Heart(float w, float h, WCHAR* textureFile, int type);
	virtual ~Heart(void);

	virtual void collideWith(Player*);
	void moveToPos(float);
	void advance();

private:
	int type;
};

