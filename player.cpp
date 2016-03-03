#include "player.h"

Player::Player(float width, float height):
	m_D3D(0),
	m_camera(0),
	m_collisions(0),
	m_sound(0),
	m_body(0),
	spriteRow(1),
	spriteCol(4),
	moveFrames(0),
	fwd(true),
	hitFrames(30),
	shootingFrames(0),
	swingingFrames(0),
	throwingFrames(0),
	swinging(false),
	shooting(false),
	throwing(false),
	gotHit(false),
	jumping(false),
	dead(false),
	healthPoints(8),
	maxHealth(8),
	lives(2),
	points(0),
	color(0),
	widthX(width),
	heightY(height),
	body_texture(L"textures/player/linkSprites.dds"),
	currentTexture_weapon(L"textures/items/weapon_normal.dds"),
	currentTexture_arrow(L"textures/items/arrow_normal.dds"),
	currentTexture_bomb(L"textures/items/bomb_normal_fwd.dds")
{
	arrowPos.x = 0.0f;
	arrowPos.y = -2.0f;
	arrowPos.z = 0.0f;
	bombPos.x = 0.0f;
	bombPos.y = -2.0f;
	bombPos.z = 0.0f;
	currentTexture_body = body_texture;
	keys[0] = keys[1] = keys[2] = false;
	radius = (width + height) / 8;
	InitializeModel();

	m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
}

Player::~Player(void)
{
	Shutdown();
}

void Player::Shutdown()
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

	if (m_arrow)
	{
		m_arrow->Shutdown();
		delete m_arrow;
		m_arrow = 0;
	}

	if (m_bomb)
	{
		m_bomb->Shutdown();
		delete m_bomb;
		m_bomb = 0;
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

void Player::InitializeModel()
{
	//Body
	m_body = new QuadTexturedModel(
		                     widthX,  //length in X direction
							 heightY,  //length in Y direction
							 currentTexture_body  //path name of .dds texture file
							 );

	m_body->worldTranslate(1.0f, heightY/2-0.35f, 0.0f); //move to location in the world

	//weapon
	m_weapon = new Weapon(  1.5f,  //length in X direction
							0.4f,  //length in Y direction
							2,
							currentTexture_weapon,  //path name of .dds texture file
							this);

	m_weapon->orientRotateZ(XM_PIDIV4);
	m_weapon->orientTranslate(1.0f, 1.0f, 0.0f); //move to location in the world
	m_weapon->worldTranslate(0.9f, 0.4f, 0.0f);
	
	//arrow
	m_arrow = new Weapon(	1.0f,  //length in X direction
							0.2f,  //length in Y direction
							1,
							currentTexture_arrow,  //path name of .dds texture file
							this);

	m_arrow->worldTranslate(arrowPos.x, arrowPos.y, arrowPos.z);
	
	//bomb
	m_bomb = new Weapon(	0.5f,  //length in X direction
							0.5f,  //length in Y direction
							2,
							currentTexture_bomb,  //path name of .dds texture file
							this);

	m_bomb->worldTranslate(bombPos.x, bombPos.y, bombPos.z);
	m_bomb->setRadius(0.85f);

	color = new XMFLOAT4(0.5f,0.3f,0.7f,1.0f);
	cent = new QuadModel(2*radius,2*radius, color);
	cent->worldTranslate(m_body->getCenter().x, m_body->getCenter().y, 0.1f);
}
ArrayList<GameModel> Player::getGameModels()
{
	ArrayList<GameModel> list;
	list.add(m_body);
	list.add(m_weapon);
	list.add(m_arrow);
	list.add(m_bomb);
//	list.add(cent);
	return list;
}

void Player::moveUp()
{
	if (!jumping && !gotHit && !dead)
	{
		m_body->worldTranslate(0.0f, JUMP_HEIGHT, 0.0f);
		m_weapon->worldTranslate(0.0f, JUMP_HEIGHT, 0.0f);
		jumping = true;
	}
}

void Player::moveDown()
{
	if (getPosY() - FWD_SPEED_PLAYER >= -0.01f)
	{
		m_body->worldTranslate(0.0f, -FWD_SPEED_PLAYER*0.75f, 0.0f);
		m_weapon->worldTranslate(0.0f, -FWD_SPEED_PLAYER*0.75f, 0.0f);
	}
	else
		jumping = false;
}

void Player::moveForward()
{
	if (!gotHit && !dead)
	{
		if (!fwd)
		{
			spriteRow = 1;
			moveFrames = 0;
			m_body->changeUV(0.20f*spriteCol, 0.3333f*spriteRow, 0.20f, 0.3333f);
			m_weapon->worldRotateY(-XM_PI);
			m_weapon->orientRotateX(-XM_PI);
			m_weapon->orientRotateZ(XM_PIDIV2);
			fwd = true;
		}

		if (getPosX() < 98.0f)
		{
			m_body->worldTranslate(FWD_SPEED_PLAYER, 0.0f, 0.0f);
			m_weapon->worldTranslate(FWD_SPEED_PLAYER, 0.0f, 0.0f);
		}
		if (moveFrames %5 == 0)
		{
			spriteCol++;
			if (spriteCol > 4)
				spriteCol = 0;
			m_body->changeUV(0.20f*spriteCol, 0.3333f*spriteRow, 0.20f, 0.3333f);
			m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
			moveFrames = 0;
		}
		moveFrames++;
	}
}

void Player::moveBackward()
{
	if (!gotHit && !dead)
	{
		if (fwd)
		{
			moveFrames = -1;
			spriteRow = 0;
			m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
			m_weapon->worldRotateY(XM_PI);
			m_weapon->orientRotateX(XM_PI);
			m_weapon->orientRotateZ(XM_PIDIV2);
			fwd = false;
		}

		if (getPosX() > 0.3f)
		{
			m_body->worldTranslate(-FWD_SPEED_PLAYER, 0.0f, 0.0f);
			m_weapon->worldTranslate(-FWD_SPEED_PLAYER, 0.0f, 0.0f);
		}
		if (moveFrames %5 == 0)
		{
			spriteCol++;
			if (spriteCol > 4)
				spriteCol = 0;
			m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
			m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
			moveFrames = 0;
		}
		moveFrames++;
	}
}

void Player::tryAnimate(int type)
{
	if (!dead){
		switch (type)
		{
		case 0: //swing sword
				if (!swinging)
				{
					swinging = true;
					swingingFrames = 30;
					m_sound->PlayDesiredFile(1);
				}
				break;

		case 1: //shoot bow
				if(!shooting)
				{
					shooting = true;
					shootingFrames = 60;  
					m_arrow->makeDeadly(true); 
					m_arrow->setFwd(fwd);
					if (fwd)
						updateProjPos(m_arrow, &arrowPos, getPosX()+1.0f, getPosY()+3.0f, 0.0f);
					else
					{
						updateProjPos(m_arrow, &arrowPos, getPosX(), getPosY()+3.0f, 0.0f);
						m_arrow->orientRotateZ(XM_PI);
					}
					m_sound->PlayDesiredFile(2);
				}
				break;

		case 2: //throw bomb
				if (!throwing)
				{
					throwing = true;
					throwingFrames = 120;  
					m_bomb->setFwd(fwd);
					if (fwd)
						m_bomb->setTextureName(L"textures/items/bomb_normal_fwd.dds");
					else
						m_bomb->setTextureName(L"textures/items/bomb_normal_bkw.dds");
					m_bomb->initializeTextures(m_D3D->GetDevice());
					updateProjPos(m_bomb, &bombPos, getPosX(), getPosY()+3.0f, 0.0f);
					m_sound->PlayDesiredFile(3);
				}
				break;
		}	
	}
}

void Player::advance()
{
	cent->worldTranslate(-cent->getCenter().x, -cent->getCenter().y, 0.0f);
	cent->worldTranslate(m_body->getCenter().x, m_body->getCenter().y, 0.0f);
	moveDown();
	if (swinging)
	{
		int factor;
		if (fwd) factor = 1;
		else factor = -1;
		if (swingingFrames == 19)
			m_weapon->makeDeadly(true);

		switch (swingingFrames/10)
		{
		case 0: m_weapon->worldRotateZ(factor*XM_PIDIV4/15); break;
		case 1: m_weapon->worldRotateZ(-2*factor*XM_PIDIV4/15); break;
		case 3:
		case 2: m_weapon->worldRotateZ(factor*XM_PIDIV4/15); break;
		}
		swingingFrames--;
		if (swingingFrames == 0)
		{
			swinging = false;
			m_weapon->makeDeadly(false);
		}
	}

	if (shooting)
	{
		if (m_arrow->getFwd())
			updateProjPos(m_arrow, &arrowPos, 0.15f, 0.0f, 0.0f);
		else
			updateProjPos(m_arrow, &arrowPos, -0.15f, 0.0f, 0.0f);
		shootingFrames--;
		if (shootingFrames == 0)
		{
			shooting = false;
			m_arrow->makeDeadly(false);
			updateProjPos(m_arrow, &arrowPos, -arrowPos.x, -m_arrow->getCenter().y-2.0f, arrowPos.z);
			if (!m_arrow->getFwd())
				m_arrow->orientRotateZ(XM_PI);
		}
	}

	if (throwing)
	{
		float factor = 1;
		if (!m_bomb->getFwd())
			factor = -1;
		switch (throwingFrames/30)
		{
		case 3: updateProjPos(m_bomb, &bombPos, factor*0.075f, 0.05f, 0.0f); break;
		case 2: updateProjPos(m_bomb, &bombPos, factor*0.075f, -0.075f, 0.0f); break;
		case 1: 
			if (bombPos.y - m_bomb->getHeight()/2 > 0.0f)
			{
				updateProjPos(m_bomb, &bombPos, factor*0.075f, -0.075f, 0.0f);
				throwingFrames++;
			}
			break;
		case 0: 
			if (throwingFrames == 29)
			{
				m_bomb->makeDeadly(true);
				m_sound->PlayDesiredFile(4);
			}
			if (throwingFrames%4 > 1)
			{
				m_bomb->setTextureName(L"textures/items/explosion1.dds");
				m_bomb->initializeTextures(m_D3D->GetDevice());
			}
			else
			{
				m_bomb->setTextureName(L"textures/items/explosion2.dds");
				m_bomb->initializeTextures(m_D3D->GetDevice());
			}
			break;
		default: break;
		}
		throwingFrames--;
		if (throwingFrames == 0)
		{
			throwing = false;
			updateProjPos(m_bomb, &bombPos, -bombPos.x, -bombPos.y-2.0f, bombPos.z);
			m_bomb->makeDeadly(false);
		}
	}

	if (gotHit)
	{
		if (hitFrames > 0)
		{
			if (fwd)
			{
				spriteCol = 1;
				if (hitFrames %8 > 4)
					spriteRow = 2;
				else
					spriteRow = 1;
			}
			else
			{
				spriteCol = 0;
				if (hitFrames %8 > 4)
					spriteRow = 2;
				else
					spriteRow = 0;
			}
			m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
			m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
			hitFrames--;
		}
		else
		{
			gotHit = false;
			if (dead)
			{
				spriteCol = 3;
				spriteRow = 2;
				m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
				m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
				m_weapon->worldTranslate(0.0f, -2.0f, 0.0f);
			}
		}
	}
}

void Player::changeTexture_body(WCHAR* newTextureFile)
{
	m_body->setTextureName(newTextureFile);
	m_body->initializeTextures(m_D3D->GetDevice());
}

void Player::setD3D(D3DClass* d3d)
{
	m_D3D = d3d;
}

void Player::setCamera(CineCameraClass* cam)
{
	m_camera = cam;
}

void Player::setCollisions(Collisions* col)
{
	m_collisions = col;
	col->addPlayerWeapon(m_weapon);
	col->addPlayerWeapon(m_arrow);
	col->addPlayerWeapon(m_bomb);
}

void Player::setSound(SoundClass* snd)
{
	m_sound = snd;
}

bool Player::doesOverlap(Weapon* weapon)
{
	float weapX, weapY, weapRad;
	weapon->getCollisionInfo(weapX, weapY, weapRad);
	float deltaX = weapX - m_body->getCenter().x;
	float deltaY = weapY - m_body->getCenter().y;
	float deltaD = sqrt(deltaX*deltaX + deltaY*deltaY);
	if (deltaD < radius + weapRad)
		return true;
	return false;
}

bool Player::doesOverlap(Enemy* mob)
{
	float mobX, mobY, mobRad;
	mob->getCollisionInfo(mobX, mobY, mobRad);
	float deltaX = mobX - m_body->getCenter().x;
	float deltaY = mobY - m_body->getCenter().y;
	float deltaD = sqrt(deltaX*deltaX + deltaY*deltaY);
	if (deltaD < radius + mobRad)
		return true;
	return false;
}

bool Player::doesOverlap(Item* item)
{
	float itemX, itemY, itemRad;
	item->getCollisionInfo(itemX, itemY, itemRad);
	float deltaX = itemX - m_body->getCenter().x;
	float deltaY = itemY - m_body->getCenter().y;
	float deltaD = sqrt(deltaX*deltaX + deltaY*deltaY);
	if (deltaD < radius + itemRad)
		return true;
	return false;
}

void Player::collideWith(Weapon* weapon)
{
	hit(weapon->getDmg());
}

void Player::collideWith(Enemy* mob)
{
	hit(mob->getDmg());
	float pushback = 2.5f;
	if (getPosX() < 3.5f)
		pushback = getPosX() - 0.4f;
	if (getPosX() > 98.0f)
		pushback = 98.0f - getPosX();
	if (getPosX() < mob->getPosX())
	{
		m_body->worldTranslate(-pushback, 0.0f, 0.0f);
		m_weapon->worldTranslate(-pushback, 0.0f, 0.0f);		
	}
	else
	{
		m_body->worldTranslate(pushback, 0.0f, 0.0f);
		m_weapon->worldTranslate(pushback, 0.0f, 0.0f);
	}
}

void Player::collideWith(Item* item)
{
}

void Player::hit(int dmg)
{
	gotHit = true;
	hitFrames = 30;
	m_sound->PlayDesiredFile(0);
	healthPoints -= dmg;
	if (healthPoints <= 0)
	{
		dead = true;
		m_sound->PlayDesiredFile(10);
		hitFrames = 60;
	}
}

void Player::respawn(bool loseLife)
{
	XMFLOAT4 center = m_body->getCenter();
	m_body->worldTranslate(-center.x, -center.y, -center.z);
	m_body->worldTranslate(1.0f, heightY/2-0.35f, 0.0f);

	center = m_weapon->getCenter();
	m_weapon->worldTranslate(-center.x, -center.y, -center.z);
	if (fwd)
		m_weapon->worldTranslate(1.9f, 1.4f, 0.0f);
	else
	{
		m_weapon->worldRotateY(-XM_PI);
		m_weapon->worldRotateX(-XM_PI);
		m_weapon->worldRotateZ(XM_PIDIV2);
		m_weapon->worldTranslate(0.0f, 1.4f, 0.0f);
	}

	spriteCol = 4;
	spriteRow = 1;
	m_body->changeUV(0.20f*spriteCol, 0.33333f*spriteRow, 0.20f, 0.33333f);
	m_body->GetVertexModel()->Initialize(m_D3D->GetDevice());
	
	swinging = shooting = throwing = jumping = false;
	swingingFrames = shootingFrames = throwingFrames = moveFrames = hitFrames = 0;
	dead = false;
	fwd = true;
	if (loseLife)
	{
		lives--;
		healthPoints = maxHealth;
	}
	m_camera->SetPosition(3.4f, 3.5f, -10.0f);
}

void Player::updateProjPos(Weapon* weap, XMFLOAT3* projectilePos, float nx, float ny, float nz)
{
	projectilePos->x += nx;
	projectilePos->y += ny;
	projectilePos->z += nz;
	weap->worldTranslate(nx, ny, nz);
}

float Player::getPosX() { return m_body->getCenter().x; }

float Player::getPosY() { return m_body->getCenter().y-heightY/2+0.35f; }

void Player::addKey(int type)
{
	keys[type] = true;
	m_sound->PlayDesiredFile(13+type);
}

void Player::addHeart()
{
	healthPoints += 2;
	if (healthPoints > maxHealth)
		healthPoints = maxHealth;
	m_sound->PlayDesiredFile(11);
}

void Player::expandHearts()
{
	maxHealth += 2;
	healthPoints = maxHealth;
	m_sound->PlayDesiredFile(12);
}

int Player::getHealth() { return healthPoints; }

int Player::getMaxHP() { return maxHealth; }

bool* Player::getKeys() { return keys; }

int Player::getLives() { return lives; }

bool Player::isDead() { return dead; }

bool Player::isShooting() { return shooting; }

void Player::clearKeys()
{
	for (int i = 0; i < 3; i++)
		keys[i] = false;
}

void Player::scorePoints(int pts) { points += pts; }

int Player::getPoints() { return points; }

void Player::addLife() { lives++; }