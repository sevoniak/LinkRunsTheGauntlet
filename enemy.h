#pragma once

#include <DirectXMath.h>
#include "gamemodel.h"
#include "quadModel.h"
#include "quadTexturedModel.h"
#include "arraylist.h"
#include "d3dclass.h"
#include "cinecameraclass.h"
#include "collisions.h"
#include "SoundClass.h"
#include "Weapon.h"
#include "player.h"
#include "Item.h"
#include <sstream>
using std::wstringstream;
using namespace DirectX;

class Collisions;
class Weapon;
class Player;
class Item;

const float FWD_SPEED_ENEMY = 0.03f; //adjust this to make forward motion look good


class Enemy
{
public:
	Enemy(float, float, float, float);
	virtual ~Enemy(void);
	virtual void Shutdown();
	ArrayList<GameModel> getGameModels();
	
	void moveUp();
	void moveDown();
	void moveForward();
	void moveBackward();

	void changeTexture_body(WCHAR*);
	void setD3D(D3DClass*);
	void setCamera(CineCameraClass*);
	void setSound(SoundClass*);
//	void tryAnimate(int);
	virtual void advance(Player*) = 0;

	bool doesOverlap(Weapon*);
	void collideWith(Weapon*);
	void collideWith(Player*);
	void getCollisionInfo(float&, float&, float&);
	float getPosX();

	virtual void hit(int) = 0;
	int getDmg();
	bool isDeadly();
	bool isDead();
	void makeDeadly(bool);

protected:
	virtual void InitializeModel(float, float);
	void setStats();
	void dropItem();

	int frames;
	int hitFrames;
	int deadlyCounter;
	int spriteRow;
	int spriteCol;
	bool animating;
	bool shooting;
	bool swinging;
	bool throwing;
	bool fwd;
	bool dead;
	bool deadly;
	wstring category;
	float posX;
	float posY;
	float widthX;
	float heightY;
	float radius;
	float speed;
	int healthPoints;
	int points;
	float waypoint;
	Item* drop;
	XMFLOAT2 collisionOffset;
	int damage;
	D3DClass* m_D3D;
	CineCameraClass* m_camera;
	SoundClass* m_sound;

	WCHAR* currentTexture_body;
	WCHAR* currentTexture_weapon;
	
	QuadTexturedModel* m_body;
	QuadTexturedModel* m_weapon;

	XMFLOAT4* color;
	QuadModel* cent;
	
};