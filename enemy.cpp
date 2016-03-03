#include "enemy.h"

Enemy::Enemy(float width, float height, float x, float z):
	m_D3D(0),
	m_camera(0),
	m_body(0),
	fwd(true),
	frames(0),
	hitFrames(0),
	spriteRow(0),
	spriteCol(0),
	deadlyCounter(80),
	animating(false),
	swinging(false),
	shooting(false),
	throwing(false),
	dead(false),
	category(L"rookie"),
	posX(0),
	posY(0),
	widthX(width),
	heightY(height),
	waypoint(0.0f),
	drop(0),
	points(0),
	color(0),
	currentTexture_body(L"textures/enemies/giant.dds")
{
	radius = 0.6f*(widthX + heightY)/4;
	setStats();
	collisionOffset.x = 0.0f;
	collisionOffset.y = 0.0f;
	InitializeModel(x, z);
}

Enemy::~Enemy(void)
{
	Shutdown();
}

void Enemy::Shutdown()
{
	if (m_body)
	{
		m_body->Shutdown();
		delete m_body;
		m_body = 0;
	}

	if (m_weapon)
	{
		m_weapon->Shutdown();
		delete m_weapon;
		m_weapon = 0;
	}

	if (cent)
	{
		cent->Shutdown();
		delete cent;
		cent = 0;
	}

	if (color)
	{
		delete color;
		color = 0;
	}
}

void Enemy::InitializeModel(float x, float z)
{
	m_body = new QuadTexturedModel(
		                     widthX,  //length in X direction
							 heightY,  //length in Y direction
							 currentTexture_body  //path name of .dds texture file
							 );
	m_body->worldTranslate(x, heightY/2.0f, z);

	m_weapon = 0;

	color = new XMFLOAT4(0.5f,0.3f,0.7f,1.0f);
	cent = new QuadModel(2*radius,2*radius, color);
	int factor = fwd ? 1 : -1;
	cent->worldTranslate(m_body->getCenter().x + factor*collisionOffset.x, m_body->getCenter().y + collisionOffset.y, 0.3f);
}

ArrayList<GameModel> Enemy::getGameModels()
{
	ArrayList<GameModel> list;
	list.add(m_body);
	//list.add(m_weapon);
//	list.add(cent);
	return list;
}

void Enemy::setD3D(D3DClass* d3d)
{
	m_D3D = d3d;
}

void Enemy::setCamera(CineCameraClass* cam)
{
	m_camera = cam;
}

void Enemy::setSound(SoundClass* snd)
{
	m_sound = snd;
}

void Enemy::moveUp()
{
	m_body->worldTranslate(0.0f, speed, 0.0f);
	m_weapon->worldTranslate(0.0f, speed, 0.0f);
	posY += speed;
}

void Enemy::moveDown()
{
	if (posY - speed >= -0.10f)
	{
		m_body->worldTranslate(0.0f, -speed, 0.0f);
		m_weapon->worldTranslate(0.0f, -speed, 0.0f);
		posY -= speed;
	}
}

void Enemy::moveForward()
{
	if (!fwd)
	{
		spriteRow = 1;
		fwd = true;
	}
	m_body->worldTranslate(speed, 0.0f, 0.0f);
//	m_weapon->worldTranslate(FWD_SPEED_ENEMY, 0.0f, 0.0f);
}

void Enemy::moveBackward()
{
	if (fwd)
	{
		spriteRow = 0;
		fwd = false;
	}
	m_body->worldTranslate(-speed, 0.0f, 0.0f);
}

void Enemy::setStats()
{
	if(category == L"rookie")
	{
		healthPoints = 4;
		damage = 1;
		speed = 0.04f;
		points = 20;
	}
	else if(category == L"warrior")
	{
		damage = 2;
		healthPoints = 7;
		speed = 0.05f;
		points = 50;
	}
	else if(category == L"elite")
	{
		damage = 3;
		healthPoints = 11;
		speed = 0.075f;
		points = 70;
	}
}

void Enemy::changeTexture_body(WCHAR* newTextureFile)
{
	m_body->setTextureName(newTextureFile);
	m_body->initializeTextures(m_D3D->GetDevice());
}

bool Enemy::doesOverlap(Weapon* weapon)
{
	float weapX, weapY, weapRad;
	weapon->getCollisionInfo(weapX, weapY, weapRad);
	int factor = fwd ? 1 : -1;
	float deltaX = weapX - (m_body->getCenter().x + factor*collisionOffset.x);
	float deltaY = weapY - (m_body->getCenter().y + collisionOffset.y);
	float deltaD = sqrt(deltaX*deltaX + deltaY*deltaY);
	if (deltaD < radius + weapRad)
		return true;
	return false;
}

void Enemy::collideWith(Weapon* weapon)
{
	hit(weapon->getDmg()); 
	if (dead)
		weapon->scorePoints(points);
}

void Enemy::collideWith(Player* player)
{
	makeDeadly(false);

}

void Enemy::getCollisionInfo(float& xPos, float& yPos, float& rad)
{
	int factor = fwd ? 1 : -1;
	xPos = m_body->getCenter().x + factor*collisionOffset.x;
	yPos = m_body->getCenter().y + collisionOffset.y;
	rad = radius;
}

float Enemy::getPosX()
{
	int factor = fwd ? 1 : -1;
	return m_body->getCenter().x + factor*collisionOffset.x;
}

int Enemy::getDmg() { return damage; }

bool Enemy::isDeadly() { return deadly; }

bool Enemy::isDead(){ return dead; }

void Enemy::makeDeadly(bool d) { deadly = d; }

void Enemy::dropItem()
{
	if (drop != NULL)
	{
		XMFLOAT4 center = m_body->getCenter();
		drop->moveToPos(center.x);
	}
}