#include "collisions.h"


Collisions::Collisions(Player* player_in):
	player(player_in)
{
	player->setCollisions(this);
}


Collisions::~Collisions(void)
{
}

void Collisions::addPlayerWeapon(Weapon* weapon)
{
	playerWeapons.add(weapon);
}

void Collisions::addMobWeapon(Weapon* weapon)
{
	mobWeapons.add(weapon);
}

void Collisions::addMob(Enemy* mob)
{
	mobs.add(mob);
}

void Collisions::addItem(Item* item)
{
	items.add(item);
}

void Collisions::removePlayerWeapon(Weapon* weapon)
{
	playerWeapons.remove(weapon);
}

void Collisions::removeMobWeapon(Weapon* weapon)
{
	mobWeapons.remove(weapon);
}

void Collisions::removeMob(Enemy* mob)
{
	mobs.remove(mob);
}

void Collisions::removeItem(Item* item)
{
	items.remove(item);
}

void Collisions::check()
{
	//check for weapons hitting the player
	for (int i = 0; i < mobWeapons.size(); i++)
	{
		Weapon* weapon = mobWeapons.elementAt(i);
		if (player->doesOverlap(weapon))
		{
			player->collideWith(weapon);
			weapon->collideWith(player);
		}
	}

	//check for mobs hitting the player
	for (int i = 0; i < mobs.size(); i++)
	{
		Enemy* mob = mobs.elementAt(i);
		if (mob->isDeadly() && player->doesOverlap(mob) && !player->isDead())
		{
			player->collideWith(mob);
			mob->collideWith(player);
		}
	}

	//check for weapons hitting a mob
	for (int m = 0; m < mobs.size(); m++)
	{
		Enemy* mob = mobs.elementAt(m);
		for (int i = 0; i < playerWeapons.size(); i++)
		{
			Weapon* weapon = playerWeapons.elementAt(i);
			if (weapon->isDeadly() && !mob->isDead() && mob->doesOverlap(weapon))
			{
				mob->collideWith(weapon);
				weapon->collideWith(mob);
			}
		}
	}

	//check for player getting items
	for (int i = 0; i < items.size(); i++)
	{
		Item* item = items.elementAt(i);
		if (player->doesOverlap(item))
		{
			player->collideWith(item);
			item->collideWith(player);
		}

	}
}
