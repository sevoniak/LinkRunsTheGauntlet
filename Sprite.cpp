#include "Sprite.h"

Sprite::Sprite(ID3D11Resource* aSpriteSheetResource, ID3D11ShaderResourceView* aSpriteSheet, int numColumns, int numRows, int currCol, int currRow, float scale_in, XMFLOAT2 aLocation)
{

	spriteSheet = aSpriteSheet;

	// Get height and width of spritesheet
	auto txt = reinterpret_cast<ID3D11Texture2D*>( aSpriteSheetResource );
	D3D11_TEXTURE2D_DESC desc;
	txt->GetDesc( &desc );

	float spriteSheetWidth = (float)desc.Width;
	float spriteSheetHeight = (float)desc.Height;

    currentRow = currRow;
    currentColumn = currCol;
    rows = numRows;
    columns = numColumns;
    frameWidth = (int)(spriteSheetWidth / columns);
    frameHeight = (int)(spriteSheetHeight / rows);

    location = aLocation;
	scale = scale_in;

    rotation = 0;
    rotationOrigin = XMFLOAT2(0, 0);
}

void Sprite::Update(float gameTime)
{
    /*clientBounds represents the area the sprite character must remain within
        * typically the game window
        */

    //modify the location based on current speed
/*    location = XMFLOAT2(location.x + speed.x, location.y + speed.y);
	
    //move to the next frame on the sprite sheet
	int elapsedMilliseconds = (int)(gameTime*1000);
	//move to the next frame on the sprite sheet
	if (elapsedMilliseconds - lastRecordedTime > 1000 / animationFrameRate )
    {
		currentColumn = (currentColumn + 1) % columns;
        if (currentColumn == 0) 
			currentRow = (currentRow + 1) % rows;
        lastRecordedTime = elapsedMilliseconds;
    }*/
}

void Sprite::Draw(SpriteBatch* spriteBatch, float gameTime)
{	
	RECT sourceFrame = { 
		currentColumn * frameWidth, 
		currentRow * frameHeight, 
		currentColumn * frameWidth + frameWidth, 
		currentRow * frameHeight + frameHeight
	};
	SpriteEffects directionFlip = SpriteEffects_None;

	spriteBatch->Draw( spriteSheet, location, &sourceFrame, Colors::White, rotation, rotationOrigin, scale, directionFlip, 0.0f );
}

void Sprite::translate(float dx, float dy)
{
	location.x += dx;
	location.y += dy;
}