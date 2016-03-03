#pragma once

#include <directxmath.h>
#include <d3d11.h>
#include "SpriteBatch.h"
#include "Effects.h"

using namespace DirectX;

class Sprite
{
public:
	Sprite(ID3D11Resource* spriteSheetResource, ID3D11ShaderResourceView* spriteSheet, 
		int numColumns, int numRows, int currCol, int currRow, float scale_in, XMFLOAT2 aLocation);

public:
	void Update(float gameTime);
	void Draw(SpriteBatch* spriteBatch, float gameTime);
	void translate(float, float);

public:
    XMFLOAT2 location; //X,Y location relative screen
    float rotation;  //rotation to be applied in radians
    XMFLOAT2 rotationOrigin; //location around which to rotate

protected:
	ID3D11ShaderResourceView* spriteSheet;

	int rows; //number of rows in sprite sheet
    int columns; //number of columns in sprite sheet
    int frameWidth; //the width of a single sprite image on sprite sheet
    int frameHeight; //height of a single sprite image on sprite sheet

    int currentRow;
    int currentColumn;
	float scale;
};