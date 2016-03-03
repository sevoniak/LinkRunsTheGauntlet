#pragma once
#include "enemy.h"
class EnemyMan : public Enemy
{
public:
	EnemyMan(float, float, float, float, wstring);
	EnemyMan(float, float, float, float, wstring, Item*);
	virtual ~EnemyMan(void);

	void advance(Player*);
	void hit(int);

private:
	XMFLOAT2 homePoint;
	float speed;
	wstring* filename;

	void animateHit();
	void animateMove();
};

