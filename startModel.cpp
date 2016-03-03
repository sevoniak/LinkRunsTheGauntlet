#include "startModel.h"

#include "player.h"

StartModel::StartModel(float width, float height):
	m_D3D(0),
	m_back(0),
	//m_floor(0),
	widthX(width),
	heightY(height),
	currentTexture_back(L"textures/background/startmenu-start.dds")
	//currentTexture_floor(L"textures/background/dirt_ground.dds")
{
	InitializeModel();
}

StartModel::~StartModel(void)
{
	Shutdown();
}

void StartModel::Shutdown()
{
	if (m_back)
	{
		m_back->Shutdown();
		delete m_back;
		m_back = 0;
	}
}

void StartModel::InitializeModel()
{
	//make a background textured quad
	m_back = new QuadTexturedModel(
		                     widthX,  //length in X direction
							 heightY,  //length in Y direction
							 currentTexture_back  //path name of .dds texture file
							 );

	m_back->worldTranslate(-20.0f, heightY/2, -1.6f); //move to location in the world
}

ArrayList<GameModel> StartModel::getGameModels()
{
	ArrayList<GameModel> list;
	list.add(m_back);
	return list;
}
QuadTexturedModel* StartModel::getGameModel()
{
	return m_back;
}
void StartModel::advance()
{
}

void StartModel::changeTexture_back(WCHAR* newTextureFile)
{
	m_back->setTextureName(newTextureFile);
	m_back->initializeTextures(m_D3D->GetDevice());
}

void StartModel::setD3D(D3DClass* d3d)
{
	m_D3D = d3d;
}

void StartModel::move(float x, float y, float z)
{
	XMFLOAT4 c = m_back->getCenter();
	m_back->worldTranslate(x, y, z);
}

float StartModel::getPosX() { return m_back->getCenter().x; }