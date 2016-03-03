#include "enemyGiant.h"


EnemyGiant::EnemyGiant(float width, float height, float x, float z, wstring rank): 
Enemy(width, height, x, z)
{
	wstringstream ss;
	ss << L"textures/enemies/enemy1_" << rank << L".dds";
	filename = new wstring(ss.str());
	m_body->setTextureName(&((*filename)[0]));
	m_body->changeUV(0.0f, 0.0f, 0.2f, 1.0f/6.0f);
	category = rank;
	setStats();
	points += 10;
	collisionOffset.x = width/5;
	collisionOffset.y = -height/5;
	blocking = false;
	blockFrames = 0;
	attacking = false;
	attackAgain = false;
}

EnemyGiant::EnemyGiant(float width, float height, float x, float z, wstring rank, Item* item):
Enemy(width, height, x, z)
{
	wstringstream ss;
	ss << L"textures/enemies/enemy1_" << rank << L".dds";
	filename = new wstring(ss.str());
	m_body->setTextureName(&((*filename)[0]));
	m_body->changeUV(0.0f, 0.0f, 0.2f, 1.0f/6.0f);
	category = rank;
	setStats();
	points += 10;
	drop = item;
	collisionOffset.x = width/5;
	collisionOffset.y = -height/5;
	blocking = false;
	blockFrames = 0;
	attacking = false;
	attackAgain = false;
}


EnemyGiant::~EnemyGiant(void)
{
	if (filename)
	{
		delete filename;
		filename = 0;
	}
}

void EnemyGiant::hit(int dmg)
{
	if (!blocking)
	{
		healthPoints -= dmg;
		if (healthPoints <= 0)
		{
			 dead = true;
			 m_sound->PlayDesiredFile(7);
		}
		else
			m_sound->PlayDesiredFile(6);
		
		if (!attacking)
		{
			hitFrames = 20;
			attacking = true;
			attackFrames = 100;
		}
		else
			attackAgain = true;
	}
	else
	{
		m_sound->PlayDesiredFile(17);
	}
}

void EnemyGiant::advance(Player* player)
{
	int factor = fwd ? 1 : -1;
	cent->worldTranslate(-cent->getCenter().x, -cent->getCenter().y, 0.0f);
	cent->worldTranslate(m_body->getCenter().x + factor*collisionOffset.x, m_body->getCenter().y + collisionOffset.y, 0.0f);

	if (blocking)	//reset sprite afer player finished shooting
	{
		blocking = player->isShooting();
		if (!blocking)
		{
			spriteCol = 0;
			spriteRow = 0;
			m_body->changeUV(0.2f*spriteCol, 1.0f/6.0f*spriteRow, 0.2f, 1.0f/6.0f);
			m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
		}
	}
	else
		blocking = player->isShooting();	//block when player shoots

	if (!deadly)	//recover after hit
	{
		deadlyCounter--;
		if (deadlyCounter == 0)
		{
			deadlyCounter = 80;
			deadly = true;
		}
	}
	
	if (blocking)		//block (cancels attacks in progress and queued attacks)
		animateBlock();

	if (hitFrames > 0)		//get hit
		animateHit();

	else if (attackFrames > 0)		//attack
		animateAttack();

	else if (frames >= 175)		//"random" attack
	{
		frames = 0;
		attacking = true;
		attackFrames = 100;
		spriteCol = 0;
		spriteRow = 0;
		m_body->changeUV(0.20f*spriteCol, 1.0f/6.0f*spriteRow, 0.20f, 1.0f/6.0f);
		m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
	}

	if (dead && m_body->getCenter().y > 0.0f)		//dead
	{
		m_body->worldTranslate(0.0f, -5.0f, 0.0f);
		dropItem();
	}
	
	else if (!attacking)
		frames++;
}

void EnemyGiant::animateAttack()
{
	if (attackFrames %5 == 0)
	{
		m_body->changeUV(0.2f*spriteCol, 1.0f/6.0f*spriteRow, 0.2f, 1.0f/6.0f);
		m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
		spriteCol++;
		if (spriteCol == 5)
		{
			spriteCol = 0;
			spriteRow = (spriteRow+1) % 5;
		}
		if (attackFrames == 40)
		{
			damage++;
			radius *= 2;
		}
	}
	attackFrames--;
	if (attackFrames == 0)
	{
		damage--;
		radius /= 2;
		if (attackAgain)
		{
			spriteCol = 0;
			spriteRow = 0;
			attackFrames = 100;
			attackAgain = false;
		}
		else
			attacking = false;
	}
}

void EnemyGiant::animateBlock()
{
	if (attacking)
	{
		attacking = false;
		attackAgain = false;
		if (attackFrames < 40)
		{
			damage--;
			radius /= 2;
		}
		attackFrames = 0;
	}
	spriteRow = 5;
	spriteCol %= 4;
	if (blockFrames % 5 == 4)
	{
		spriteCol = (spriteCol+1)%4;
		blockFrames = 0;
	}
	blockFrames++;
	m_body->changeUV(0.2f*spriteCol, 1.0f/6.0f*spriteRow, 0.2f, 1.0f/6.0f);
	m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
}

void EnemyGiant::animateHit()
{
	if (hitFrames %6 > 3)
	{
		spriteCol = 2;
		spriteRow = 4;
	}
	else
	{
		spriteCol = 0;
		spriteRow = 0;
	}
	m_body->changeUV(0.20f*spriteCol, 1.0f/6.0f*spriteRow, 0.20f, 1.0f/6.0f);
	m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
	hitFrames--;
}
