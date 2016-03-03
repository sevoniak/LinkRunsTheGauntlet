#include "Weapon.h"


Weapon::Weapon(float xLength, float yLength, int dmg, WCHAR* textureFileName, Player* player):
	QuadTexturedModel(xLength, yLength, textureFileName),
	pOwner(player),
	mOwner(NULL),
	damage(dmg),
	canHit(false),
	fwd(true),
	widthX(xLength),
	heightY(yLength)
{
	radius = (xLength + yLength)/4;
}

Weapon::Weapon(float xLength, float yLength, int dmg, WCHAR* textureFileName, Enemy* mob):
	QuadTexturedModel(xLength, yLength, textureFileName),
	pOwner(NULL),
	mOwner(mob),
	damage(dmg)
{
}

Weapon::~Weapon(void)
{
}

bool Weapon::isDeadly() { return canHit; }

void Weapon::makeDeadly(bool val)
{
	canHit = val;
}

void Weapon::collideWith(Player* player)
{
	makeDeadly(false);
}

void Weapon::collideWith(Enemy* mob)
{
	makeDeadly(false);
	if (damage == 1)
		worldTranslate(0.0f, -getCenter().y-2.0f, 0.0f);
}

void Weapon::getCollisionInfo(float& xPos, float& yPos, float& rad)
{
	XMFLOAT4 center = getCenter();
	xPos = center.x;
	yPos = center.y;
	rad = radius;
}

void Weapon::setFwd(bool fwd_in) { fwd = fwd_in; }

bool Weapon::getFwd() { return fwd; }

int Weapon::getDmg() { return damage; }

void Weapon::setRadius(float rad) { radius = rad; }

float Weapon::getRadius() {return radius; }

float Weapon::getHeight() { return heightY; }

void Weapon::scorePoints(int pts) { pOwner->scorePoints(pts); }