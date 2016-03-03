#pragma once

#include <DirectXMath.h>
#include "gamemodel.h"
#include "quadTexturedModel.h"
#include "arraylist.h"
#include "d3dclass.h"

class StartModel : GameModel
{
public:
	StartModel(float, float);
	virtual ~StartModel(void);
	virtual void Shutdown();
	ArrayList<GameModel> getGameModels();
    QuadTexturedModel* getGameModel();

	void advance();

	void changeTexture_back(WCHAR*);
	void setD3D(D3DClass*);
	void move(float, float, float);
	float getPosX();

protected:
	virtual void InitializeModel();

private:
	float widthX;
	float heightY;
	D3DClass* m_D3D;
	WCHAR* currentTexture_back;
	QuadTexturedModel* m_back;
};

