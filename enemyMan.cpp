#include "enemyMan.h"


EnemyMan::EnemyMan(float width, float height, float x, float z, wstring rank): 
Enemy(width, height, x, z)
{
	wstringstream ss;
	ss << L"textures/enemies/enemy2_" << rank << L".dds";
	filename = new wstring(ss.str());
	m_body->setTextureName(&((*filename)[0]));
	m_body->changeUV(0.0f, 0.0f, 0.333333333f, 0.33333f);
	category = rank;
	setStats();
	homePoint.x = m_body->getCenter().x;
	homePoint.y = m_body->getCenter().y;
	waypoint = homePoint.x - (rand() % 7 + 3.0f);
}

EnemyMan::EnemyMan(float width, float height, float x, float z, wstring rank, Item* item):
Enemy(width, height, x, z)
{
	wstringstream ss;
	ss << L"textures/enemies/enemy2_" << rank << L".dds";
	filename = new wstring(ss.str());
	m_body->setTextureName(&((*filename)[0]));
	m_body->changeUV(0.0f, 0.0f, 0.333333333f, 0.33333f);
	category = rank;
	setStats();
	drop = item;
	homePoint.x = m_body->getCenter().x;
	homePoint.y = m_body->getCenter().y;
	waypoint = homePoint.x - (rand() % 7 + 3.0f);
}


EnemyMan::~EnemyMan(void)
{
	if (filename)
	{
		delete filename;
		filename = 0;
	}
}

void EnemyMan::hit(int dmg)
{
	healthPoints -= dmg;
	if (healthPoints <= 0)
	{
		 dead = true;
		 m_sound->PlayDesiredFile(9);
	}
	else
		m_sound->PlayDesiredFile(8);
		 
	hitFrames = 20;
}

void EnemyMan::advance(Player* player)
{
	cent->worldTranslate(-cent->getCenter().x, -cent->getCenter().y, 0.0f);
	cent->worldTranslate(m_body->getCenter().x, m_body->getCenter().y, 0.0f);
	if (!deadly)
	{
		deadlyCounter--;
		if (deadlyCounter == 0)
		{
			deadlyCounter = 80;
			deadly = true;
		}
	}

	if (hitFrames > 0)
		animateHit();

	else if (!dead)
		animateMove();
}

void EnemyMan::animateHit()
{
	if (fwd)
	{
		if (hitFrames %6 > 3)
		{
			spriteCol = 1;
			spriteRow = 2;
		}
		else
		{
			spriteCol = 0;
			spriteRow = 1;
		}
	}
	else
	{
		if (hitFrames %6 > 3)
		{
			spriteCol = 0;
			spriteRow = 2;
		}
		else
		{
			spriteCol = 0;
			spriteRow = 0;
		}
	}
	m_body->changeUV(0.33333f*spriteCol, 0.33333f*spriteRow, 0.33333f, 0.33333f);
	m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
	hitFrames--;
	
	if (hitFrames == 0)
	{
		if (fwd)
			spriteRow = 1;
		else
			spriteRow = 0;
		spriteCol = 0;
		if (dead && m_body->getCenter().y > 0.0f)
		{
			m_body->worldTranslate(0.0f, -5.0f, 0.0f);
			dropItem();
		}
	}
}

void EnemyMan::animateMove()
{
	if (abs(waypoint - m_body->getCenter().x) < 0.1f)
	{
		if (frames > 0)
			frames = -30;
	}
	else
	{
		if (waypoint - m_body->getCenter().x > 0.0f)
		{
			moveForward();
			if (frames %8 == 4)
			{
				spriteCol = (spriteCol + 1)%3;
				m_body->changeUV(0.33333f*spriteCol, 0.33333f*spriteRow, 0.33333f, 0.33333f);
				m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
			}
		}
		else
		{
			moveBackward();
			if (frames %8 == 4)
			{
				spriteCol = (spriteCol + 1)%3;
				m_body->changeUV(0.33333f*spriteCol, 0.33333f*spriteRow, 0.33333f, 0.33333f);
				m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
			}
		}
	}
	frames++;
	if (frames == 0)
	{
		if (fwd)
			waypoint = homePoint.x - (rand() % 7 + 3.0f);
		else
			waypoint = homePoint.x + (rand() % 7 + 3.0f);
	}
}
	