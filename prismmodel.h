#pragma once

#include <DirectXMath.h>
#include "vertextype.h"
#include "modelclass.h"
#include "gamemodel.h"


#include <string>

using namespace DirectX;

class PrismModel : public GameModel
{
public:
	PrismModel(float height, float radius, int nFaces, XMFLOAT4 * pColors = 0);
	virtual ~PrismModel(void);
	virtual void Shutdown();

protected:
	virtual void InitializeModel(float height, float radius, int nFaces, XMFLOAT4 * pColors);

};

