#pragma once
#include "arraylist.h"
#include "player.h"
#include "enemy.h"
#include "Weapon.h"
#include "Item.h"
class Player;
class Enemy;
class Item;

class Collisions
{
private:
	Player* player;
	ArrayList<Enemy> mobs;
	ArrayList<Weapon> playerWeapons;
	ArrayList<Weapon> mobWeapons;
	ArrayList<Item> items;

public:
	Collisions(Player*);
	~Collisions(void);

	void addPlayerWeapon(Weapon*);
	void addMobWeapon(Weapon*);
	void addMob(Enemy*);
	void addItem(Item*);
	void removePlayerWeapon(Weapon*);
	void removeMobWeapon(Weapon*);
	void removeMob(Enemy*);
	void removeItem(Item*);

	void check();

};

