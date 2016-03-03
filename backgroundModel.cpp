#include "backgroundModel.h"

#include "player.h"

BackgroundModel::BackgroundModel(float width, float height):
	m_D3D(0),
	m_back(0),
	m_floor(0),
	widthX(width),
	heightY(height),
	currentTexture_back(0),
	currentTexture_floor(0),
	currentTexture_leftGate(0),
	currentTexture_rightGate(0)
{
	InitializeModel();
}

BackgroundModel::~BackgroundModel(void)
{
}

void BackgroundModel::Shutdown()
{
	if (m_back)
	{
		m_back->Shutdown();
		delete m_back;
		m_back = 0;
	}

	if (m_floor)
	{
		m_floor->Shutdown();
		delete m_floor;
		m_floor = 0;
	}
	if (m_leftGate)
	{
		m_leftGate->Shutdown();
		delete m_leftGate;
		m_leftGate = 0;
	}
	if (m_rightGate)
	{
		m_rightGate->Shutdown();
		delete m_rightGate;
		m_rightGate = 0;
	}
	if (currentTexture_back)
	{
		delete currentTexture_back;
		currentTexture_back = 0;
	}
	if (currentTexture_floor)
	{
		delete currentTexture_floor;
		currentTexture_floor = 0;
	}
	if (currentTexture_leftGate)
	{
		delete currentTexture_leftGate;
		currentTexture_leftGate = 0;
	}
	if (currentTexture_rightGate)
	{
		delete currentTexture_rightGate;
		currentTexture_rightGate = 0;
	}
}

void BackgroundModel::InitializeModel()
{
	//make a background textured quad
	if (currentTexture_back)
		delete currentTexture_back;
	currentTexture_back = new wstring(L"textures/background/level01_back.dds");
	m_back = new QuadTexturedModel(
		                     widthX,  //length in X direction
							 heightY,  //length in Y direction
							 &((*currentTexture_back)[0])  //path name of .dds texture file
							 );

	m_back->worldTranslate(widthX/2-1, (heightY-1)/2, 1.5f); //move to location in the world

	//make a floor textured quad
	if (currentTexture_floor)
		delete currentTexture_floor;
	currentTexture_floor = new wstring(L"textures/background/level01_floor.dds");
	m_floor = new QuadTexturedModel(
		                     widthX,  //length in X direction
							 5.0f,  //length in Y direction
							 &((*currentTexture_floor)[0])  //path name of .dds texture file
							 );
	m_floor->orientRotateX(XM_PIDIV2);
	m_floor->worldTranslate(widthX/2-1, 0.0f, 0.0f); //move to location in the world

	//make a floor textured quad
	if (currentTexture_leftGate)
		delete currentTexture_leftGate;
	currentTexture_leftGate = new wstring(L"textures/background/level01_LGate.dds");
	m_leftGate = new QuadTexturedModel(
		                     4.0f,  //length in X direction
							 9.0f,  //length in Y direction
							 &((*currentTexture_leftGate)[0])  //path name of .dds texture file
							 );
	m_leftGate->orientRotateY(-XM_PIDIV2);
	m_leftGate->worldTranslate(-0.8f, 4.5f, -0.5f); //move to location in the world

	//make a floor textured quad
	if (currentTexture_rightGate)
		delete currentTexture_rightGate;
	currentTexture_rightGate = new wstring(L"textures/background/level01_RGate.dds");
	m_rightGate = new QuadTexturedModel(
		                     4.0f,  //length in X direction
							 9.0f,  //length in Y direction
							 &((*currentTexture_rightGate)[0])  //path name of .dds texture file
							 );
	m_rightGate->orientRotateY(XM_PIDIV2);
	m_rightGate->worldTranslate(99.0f, 4.5f, -0.5f); //move to location in the world
}

ArrayList<GameModel> BackgroundModel::getGameModels()
{
	ArrayList<GameModel> list;
	list.add(m_back);
	list.add(m_floor);
	list.add(m_leftGate);
	list.add(m_rightGate);
	return list;
}

void BackgroundModel::advance()
{
}

void BackgroundModel::changeTexture_back(int lvl)
{
	wstringstream ss;
	ss << L"textures/background/level";
	if (lvl < 10)
		ss << L"0";
	ss << lvl << L"_back.dds";
	if (currentTexture_back)
		delete currentTexture_back;
	currentTexture_back = new wstring(ss.str());
	m_back->setTextureName(&((*currentTexture_back)[0]));
	m_back->initializeTextures(m_D3D->GetDevice());
}

void BackgroundModel::changeTexture_floor(int lvl)
{
	wstringstream ss;
	ss << L"textures/background/level";
	if (lvl < 10)
		ss << L"0";
	ss << lvl << L"_floor.dds";
	if (currentTexture_floor)
		delete currentTexture_floor;
	currentTexture_floor = new wstring(ss.str());
	m_floor->setTextureName(&((*currentTexture_floor)[0]));
	m_floor->initializeTextures(m_D3D->GetDevice());
}

void BackgroundModel::changeTexture_leftGate(int lvl)
{
	wstringstream ss;
	ss << L"textures/background/level";
	if (lvl < 10)
		ss << L"0";
	ss << lvl << L"_LGate.dds";
	if (currentTexture_leftGate)
		delete currentTexture_leftGate;
	currentTexture_leftGate = new wstring(ss.str());
	m_leftGate->setTextureName(&((*currentTexture_leftGate)[0]));
	m_leftGate->initializeTextures(m_D3D->GetDevice());
}

void BackgroundModel::changeTexture_rightGate(int lvl)
{
	wstringstream ss;
	ss << L"textures/background/level";
	if (lvl < 10)
		ss << L"0";
	ss << lvl << L"_RGate.dds";
	if (currentTexture_rightGate)
		delete currentTexture_rightGate;
	currentTexture_rightGate = new wstring(ss.str());
	m_rightGate->setTextureName(&((*currentTexture_rightGate)[0]));
	m_rightGate->initializeTextures(m_D3D->GetDevice());
}

void BackgroundModel::setD3D(D3DClass* d3d)
{
	m_D3D = d3d;
}