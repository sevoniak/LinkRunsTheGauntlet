#pragma once

#include "d3dclass.h"
#include <string>
using std::wstring;

class FontItem
{
public:
	FontItem(wstring, float=0.0f, float=0.0f);
	~FontItem(void);
	void setText(wstring);
	void setPos(float, float);
	wstring getText();
	XMFLOAT2 getPos();

private:
	wstring text;
	XMFLOAT2 pos;
};

