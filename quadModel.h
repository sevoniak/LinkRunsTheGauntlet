#pragma once

#include <DirectXMath.h>
#include "vertextype.h"
#include "modelclass.h"
#include "gamemodel.h"


#include <string>

using namespace DirectX;

class QuadModel : public GameModel
	//This model represents a quad made up of two trianges.
	//One of lengthX, lengthY, or lengthZ must be 0;
{
public:
	QuadModel(float lengthX, float lengthY, XMFLOAT4 * pColor = 0);
	virtual ~QuadModel(void);
	virtual void Shutdown();

protected:
	virtual void InitializeModel(float lengthX, float lengthY, XMFLOAT4 * pColor);

};

