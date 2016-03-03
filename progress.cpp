#include "progress.h"
#include "collisions.h"
#include "player.h"

using namespace DirectX;

Progress::Progress(Player* p, D3DClass* d3d, SoundClass* snd):
	lives(3),
	livesGiven(0),
	score(0),
	gameover(false),
	won(false),
	gameTime(180),
	m_Player(p),
	m_D3D(d3d),
	m_sound(snd),
	heartSpriteSheetRes(0),
	heartSpriteSheetShadRes(0),
	keySpriteSheetRes(0),
	keySpriteSheetShadRes(0)
{
	m_masterSprites = new ArrayList<Sprite>();
	m_heartSprites = new ArrayList<Sprite>();
	m_keySprites = new ArrayList<Sprite>();
	m_fonts = new ArrayList<FontItem>();

	CreateWICTextureFromFile(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), 
							L"images/items/heartSprites.png", &heartSpriteSheetRes, &heartSpriteSheetShadRes);
	maxHP = m_Player->getMaxHP();
	healthPoints = m_Player->getHealth();
	for (int i = 0; i < healthPoints/2; i++)
		addHeartSprite(0);

	CreateWICTextureFromFile(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), 
							L"images/items/keySprites.png", &keySpriteSheetRes, &keySpriteSheetShadRes);
	m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 0, 1, 1.0f, XMFLOAT2(600, 20)));
	m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 1, 1, 1.0f, XMFLOAT2(660, 20)));
	m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 2, 1, 1.0f, XMFLOAT2(720, 20)));

	m_masterSprites->addAll(*m_keySprites);
	m_masterSprites->addAll(*m_heartSprites);

	m_fonts->add(new FontItem(L"Score: ", 10.0f, 80.0f));
	m_fonts->add(new FontItem(L"Time", 330.0f, 10.0f));
	m_fonts->add(new FontItem(L"Lives", 10.0f, 50.0f));
	wstringstream wss, wss2, wss3;
	wss << gameTime;
	wss2 << m_Player->getLives();
	wss3 << m_Player->getPoints();
	m_fonts->add(new FontItem(wss3.str(), 90.0f, 90.0f));
	m_fonts->add(new FontItem(wss2.str(), 90.0f, 50.0f));
	m_fonts->add(new FontItem(wss.str(), 335.0f, 40.0f));
}

Progress::~Progress(void)
{
	Shutdown();
}

void Progress::Shutdown()
{	
	if (m_masterSprites)
	{
		for (int i = 0; i < m_masterSprites->size(); i++)
			delete m_masterSprites->elementAt(i);
		delete m_masterSprites;
		m_masterSprites = 0;
	}
	if (m_heartSprites)
	{
		delete m_heartSprites;
		m_heartSprites = 0;
	}
	if (m_keySprites)
	{
		delete m_keySprites;
		m_keySprites = 0;
	}
	if (m_fonts)
	{
		for (int i = 0; i < m_fonts->size(); i++)
		{
			if (m_fonts->elementAt(i))
				delete m_fonts->elementAt(i);
		}
		delete m_fonts;
		m_fonts = 0;
	}
}

void Progress::addHeartSprite(int type)
{
	int size = m_heartSprites->size();
	m_heartSprites->add(new Sprite(heartSpriteSheetRes, heartSpriteSheetShadRes, 3, 1, type, 0, 0.15f, XMFLOAT2(10.0f+27.0f*size, 20.0f)));
}

ArrayList<Sprite>* Progress::getSprites()
{
	if (gameover || won)
	{
		delete m_masterSprites;
		m_masterSprites = new ArrayList<Sprite>();
		return NULL;
	}
	return m_masterSprites;
}

void Progress::updateSprites()
{
	bool changed = ((m_Player->getHealth() - healthPoints) != 0 || (m_Player->getMaxHP() - maxHP) != 0) ? true : false;
	if (changed)
	{
		healthPoints = m_Player->getHealth();
		maxHP = m_Player->getMaxHP();
		int size = m_heartSprites->size();
		for (int i = 0; i < size; i++)
			delete (m_heartSprites->removeLast());
		int num = maxHP/2;
		int full = healthPoints/2;
		for (int i = 0; i < full; i++)
			addHeartSprite(0);
		if (healthPoints % 2 == 1)
		{
			addHeartSprite(1);
			num--;
		}
		for (int i = 0; i < num-full; i++)
			addHeartSprite(2);
	}

	bool* playerKeys = m_Player->getKeys();
	for (int i = 0; i < 3; i++)
	{
		if (keys[i] != playerKeys[i])
		{
			changed = true;
			keys[i] = playerKeys[i];
			for (int j = 0; j < 3; j++)
				delete (m_keySprites->removeLast());
			m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 0, keys[0] ? 0 : 1, 1.0f, XMFLOAT2(600, 20)));
			m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 1, keys[1] ? 0 : 1, 1.0f, XMFLOAT2(660, 20)));
			m_keySprites->add(new Sprite(keySpriteSheetRes, keySpriteSheetShadRes, 3, 2, 2, keys[2] ? 0 : 1, 1.0f, XMFLOAT2(720, 20)));
		}
	}

	if (changed)
	{
		delete m_masterSprites;
		m_masterSprites = new ArrayList<Sprite>();
		m_masterSprites->addAll(*m_keySprites);
		m_masterSprites->addAll(*m_heartSprites);
	}
}

ArrayList<FontItem>* Progress::getFonts() 
{ 
	if (!gameover && !won)
	{
		static DWORD lastUpdateTime = 0;

		DWORD dwTimeCur = GetTickCount();
		if( lastUpdateTime == 0 )
			lastUpdateTime = dwTimeCur;
		float deltaT = (dwTimeCur - lastUpdateTime) / 1000.0f; //elapsed time in seconds

		if(deltaT > 1.0f && gameTime > 0)
		{
			lastUpdateTime = dwTimeCur;
			gameTime--;
			if (gameTime < 15)
				m_sound->PlayDesiredFile(21);
		}
		if (m_Player->getPoints() / 5000 > livesGiven)
		{
			m_Player->addLife();
			livesGiven++;
		}
		delete m_fonts->removeLast();
		delete m_fonts->removeLast();
		delete m_fonts->removeLast();
		wstringstream ss, ss2, ss3;
		ss << gameTime;
		ss2 << m_Player->getLives();
		ss3 << m_Player->getPoints();
		m_fonts->add(new FontItem(ss3.str(), 90.0f, 80.0f));
		m_fonts->add(new FontItem(ss2.str(), 90.0f, 50.0f));
		m_fonts->add(new FontItem(ss.str(), 335.0f, 40.0f));
		
		return m_fonts; 
	}
	else
	{
		int size = m_fonts->size();
		for (int i = 0; i < size; i++)
			delete m_fonts->removeLast();
		return NULL;
	}
}

void Progress::setGameover(bool go) { gameover = go; }

bool Progress::isGameover() { return gameover; }

bool Progress::hasAllKeys() { return keys[0] && keys[1] && keys[2]; }

void Progress::setTime(int t) { gameTime = t; }

int Progress::getTime() { return gameTime; }

bool Progress::getWon() { return won; }

void Progress::setWon (bool w) { won = w; }

bool Progress::timeIsUp() { return gameTime == 0 ? true : false; }