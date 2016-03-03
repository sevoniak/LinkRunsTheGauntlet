#pragma once

#include <DirectXMath.h>
#include "gamemodel.h"
#include "quadTexturedModel.h"
#include "arraylist.h"
#include "d3dclass.h"

class BackgroundModel
{
public:
	BackgroundModel(float, float);
	virtual ~BackgroundModel(void);
	virtual void Shutdown();
	ArrayList<GameModel> getGameModels();

	void advance();

	void changeTexture_back(int);
	void changeTexture_floor(int);
	void changeTexture_leftGate(int);
	void changeTexture_rightGate(int);
	void setD3D(D3DClass*);

protected:
	virtual void InitializeModel();

private:
	float widthX;
	float heightY;
	D3DClass* m_D3D;
	wstring* currentTexture_back;
	wstring* currentTexture_floor;
	wstring* currentTexture_leftGate;
	wstring* currentTexture_rightGate;
	QuadTexturedModel* m_back;
	QuadTexturedModel* m_floor;
	QuadTexturedModel* m_leftGate;
	QuadTexturedModel* m_rightGate;
};

