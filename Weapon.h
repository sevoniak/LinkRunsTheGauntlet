#pragma once
#include "quadTexturedModel.h"
#include "enemy.h"
class Player;
class Enemy;

class Weapon : public QuadTexturedModel
{
public:
	Weapon(float, float, int, WCHAR*, Player*);
	Weapon(float, float, int, WCHAR*, Enemy*);
	~Weapon(void);
	void makeDeadly(bool);
	bool isDeadly();
	void collideWith(Player*);
	void collideWith(Enemy*);
	void getCollisionInfo(float&, float&, float&);
	void setFwd(bool);
	bool getFwd();
	int getDmg();
	void setRadius(float);
	float getRadius();
	float getHeight();
	void scorePoints(int);
	
private:
	Player* pOwner;
	Enemy* mOwner;
	float widthX;
	float heightY;
	int damage;
	float radius;
	bool canHit;
	bool fwd;
};

