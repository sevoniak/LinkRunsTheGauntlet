#pragma once
#include "enemy.h"
class EnemyGiant : public Enemy
{
public:
	EnemyGiant(float, float, float, float, wstring);
	EnemyGiant(float, float, float, float, wstring, Item*);
	virtual ~EnemyGiant(void);

	void advance(Player*);
	void hit(int);

private:
	bool blocking;
	int blockFrames;
	bool attacking;
	int attackFrames;
	bool attackAgain;

	wstring* filename;

	void animateAttack();
	void animateBlock();
	void animateHit();
};

