#pragma once

#include <DirectXMath.h>
#include "vertextype.h"
#include "modelclass.h"
#include "gamemodel.h"


#include <string>

using namespace DirectX;

class CubeModel : public GameModel
{
public:
	CubeModel(float lengthX, float lengthY, float lengthZ, XMFLOAT4 * pColors = 0);
	virtual ~CubeModel(void);
	virtual void Shutdown();

protected:
	virtual void InitializeModel(float lengthX, float lengthY, float lengthZ, XMFLOAT4 * pColors);

};

