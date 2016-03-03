#include "FontItem.h"


FontItem::FontItem(wstring t, float x, float y)
{
	text = t;
	pos.x = x;
	pos.y =y;
}


FontItem::~FontItem(void)
{		
}

wstring FontItem::getText() { return text; }

XMFLOAT2 FontItem::getPos() { return pos; }

void FontItem::setText(wstring t) { text = t; }

void FontItem::setPos(float x, float y) { pos.x = x; pos.y = y; }
