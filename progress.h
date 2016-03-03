#pragma once

#include <DirectXMath.h>
#include "arraylist.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "player.h"
#include "Sprite.h"
#include "WICTextureLoader.h"
#include "FontItem.h"

using namespace DirectX;

class Progress{
public:
	Progress(Player*, D3DClass*, SoundClass*);
	~Progress(void);
	void Shutdown();

	ArrayList<Sprite>* getSprites();
	void updateSprites();
	ArrayList<FontItem>* getFonts();
	void setGameover(bool);
	bool isGameover();
	bool hasAllKeys();
	int getTime();
	void setTime(int);
	bool getWon();
	void setWon(bool);
	bool timeIsUp();

private:
	int lives; 
	int livesGiven;
	int score;
	int gameTime;
	int healthPoints;
	int maxHP;
	bool keys[3];
	bool gameover;
	bool won;
	std::unique_ptr<SpriteBatch>    g_Sprites;
	std::unique_ptr<SpriteFont>   g_Font;

	D3DClass* m_D3D;
	ID3D11Resource*	heartSpriteSheetRes;
	ID3D11ShaderResourceView* heartSpriteSheetShadRes;
	ID3D11Resource*	keySpriteSheetRes;
	ID3D11ShaderResourceView* keySpriteSheetShadRes;
	SoundClass* m_sound;

	Player* m_Player;
	ArrayList<Sprite>* m_masterSprites;
	ArrayList<Sprite>* m_heartSprites;
	ArrayList<Sprite>* m_keySprites;
	ArrayList<FontItem>* m_fonts;

	void addHeartSprite(int type);	
};