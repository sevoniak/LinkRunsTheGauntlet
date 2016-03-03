#pragma once

#include <DirectXMath.h>
#include "gamemodel.h"
#include "quadModel.h"
#include "quadTexturedModel.h"
#include "arraylist.h"
#include "d3dclass.h"
#include "cinecameraclass.h"
#include "collisions.h"
#include "Weapon.h"
#include "enemy.h"
#include "SoundClass.h"
#include "Item.h"
#include <sstream>
using namespace DirectX;
class Weapon;
class Collisions;
class Enemy;
class Item;

const float FWD_SPEED_PLAYER = 0.07f; //adjust this to make forward motion look good
const float JUMP_HEIGHT = 3.0f;
class Player
{
public:
	Player(float, float);
	virtual ~Player(void);
	virtual void Shutdown();
	ArrayList<GameModel> getGameModels();

	void moveUp();
	void moveDown();
	void moveForward();
	void moveBackward();
	void advance();

	void changeTexture_body(WCHAR*);
	void setD3D(D3DClass*);
	void setCamera(CineCameraClass*);
	void setCollisions(Collisions*);
	void setSound(SoundClass*);
	void tryAnimate(int);
	bool doesOverlap(Weapon*);
	bool doesOverlap(Enemy*);
	bool doesOverlap(Item* item);
	void collideWith(Weapon*);
	void collideWith(Enemy*);
	void collideWith(Item*);
	void hit(int);
	void respawn(bool);
	float getPosX();
	float getPosY();
	void addKey(int);
	void addHeart();
	void expandHearts();
	int getHealth();
	int getMaxHP();
	bool* getKeys();
	int getLives();
	bool isDead();
	bool isShooting();
	void clearKeys();
	void scorePoints(int);
	int getPoints();
	void addLife();

protected:
	virtual void InitializeModel();

private:
	int hitFrames;
	int spriteRow;
	int spriteCol;
	int moveFrames;
	int shootingFrames;
	int swingingFrames;
	int throwingFrames;
	bool keys[3];
	bool shooting;
	bool swinging;
	bool throwing;
	bool jumping;
	bool fwd;
	bool dead;
	bool gotHit;
	float widthX;
	float heightY;
	float radius;
	int healthPoints;
	int maxHealth;
	int lives;
	int points;
	XMFLOAT3 arrowPos;
	XMFLOAT3 bombPos;
	D3DClass* m_D3D;
	CineCameraClass* m_camera;
	Collisions* m_collisions;
	SoundClass* m_sound;

	WCHAR* currentTexture_body;
	WCHAR* currentTexture_weapon;
	WCHAR* currentTexture_arrow;
	WCHAR* currentTexture_bomb;

	QuadTexturedModel* m_body;
	Weapon* m_weapon;
	Weapon* m_arrow;
	Weapon* m_bomb;

	WCHAR* body_texture;

	void updateProjPos(Weapon*, XMFLOAT3*, float, float, float);

	XMFLOAT4* color;
	QuadModel* cent;
};

