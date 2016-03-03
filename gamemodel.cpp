#include "gamemodel.h"
#include "modelclass.h"

GameModel::GameModel()
{

	//initialize all matrices to identity matrix
	XMStoreFloat4x4(&m_orientRotateMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_orientTranslateMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_dimensionScaleMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_worldRotateMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_worldTranslateMatrix, XMMatrixIdentity());

	m_center.x = 0.0f;
	m_center.y = 0.0f;
	m_center.z = 0.0f;
	m_center.w = 1.0f;

}

GameModel::~GameModel(void)
{
	Shutdown();
}





void GameModel::Shutdown()
{
	if(m_colorVertices)
	{
		delete[] m_colorVertices;
		m_colorVertices = 0;
	}

	if(m_textureVertices)
	{
		delete[] m_textureVertices;
		m_textureVertices = 0;
	}


	if(m_indices)
	{
		delete[] m_indices;
		m_indices = 0;
	}

	if(m_VertexModel)
	{
		delete m_VertexModel;
		m_VertexModel = 0;
	}


}

bool GameModel::isColorVertexModel(){
	if(m_colorVertices) return true;
	else return false;
}
bool GameModel::isTextureVertexModel(){
	if(m_textureVertices) return true;
	else return false;

}

bool GameModel::initializeTextures(ID3D11Device* device){
	//subclasses who have textures are expected to overide this method
	return false;
}
ID3D11ShaderResourceView* GameModel::GetTexture(){
	//subclasses who have textures are expected to overide this method
	return 0;
}



ColorVertexType* GameModel::GetColorVertices()
{
	return m_colorVertices;
}

TextureVertexType* GameModel::GetTextureVertices()
{
	return m_textureVertices;
}

unsigned long* GameModel::GetIndices()
{
	return m_indices;
}

int GameModel::GetVertexCount() {
	return m_vertexCount;
}

int GameModel::GetIndexCount() {
	return m_indexCount;
}


ModelClass* GameModel::GetVertexModel() {return m_VertexModel; }

XMFLOAT4X4 GameModel::GetWorldMatrix(){
    
	//Build the world matrix to give to the graphis system
	XMFLOAT4X4 worldMatrix; 
	XMStoreFloat4x4(&worldMatrix, 
		XMLoadFloat4x4(&m_orientRotateMatrix) * 
		XMLoadFloat4x4(&m_orientTranslateMatrix) * 
		XMLoadFloat4x4(&m_worldRotateMatrix) * 
		XMLoadFloat4x4(&m_worldTranslateMatrix)

		);

	return worldMatrix;

}

XMFLOAT4X4 GameModel::GetWorldRotateMatrix(){
	return m_worldRotateMatrix;
}

void GameModel::orientRotateX(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_orientRotateMatrix, XMLoadFloat4x4(&m_orientRotateMatrix) * XMMatrixRotationX(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationX(radianAngle)));

}
void GameModel::orientRotateY(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_orientRotateMatrix, XMLoadFloat4x4(&m_orientRotateMatrix) * XMMatrixRotationY(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationY(radianAngle)));
}
void GameModel::orientRotateZ(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_orientRotateMatrix, XMLoadFloat4x4(&m_orientRotateMatrix) * XMMatrixRotationZ(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationZ(radianAngle)));
}

void GameModel::orientTranslate(float deltaX, float deltaY, float deltaZ){
		XMStoreFloat4x4(&m_orientTranslateMatrix, XMLoadFloat4x4(&m_orientTranslateMatrix) * XMMatrixTranslation(deltaX, deltaY, deltaZ));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixTranslation(deltaX, deltaY, deltaZ)));
}

void GameModel::worldRotateX(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_worldRotateMatrix, XMLoadFloat4x4(&m_worldRotateMatrix) * XMMatrixRotationX(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationX(radianAngle)));

}
void GameModel::worldRotateY(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_worldRotateMatrix, XMLoadFloat4x4(&m_worldRotateMatrix) * XMMatrixRotationY(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationY(radianAngle)));
}
void GameModel::worldRotateZ(float radianAngle){
        // orientationMatrix *= Matrix.CreateRotationY(ry);
		XMStoreFloat4x4(&m_worldRotateMatrix, XMLoadFloat4x4(&m_worldRotateMatrix) * XMMatrixRotationZ(radianAngle));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixRotationZ(radianAngle)));
}

void GameModel::worldTranslate(float deltaX, float deltaY, float deltaZ){
		XMStoreFloat4x4(&m_worldTranslateMatrix, XMLoadFloat4x4(&m_worldTranslateMatrix) * XMMatrixTranslation(deltaX, deltaY, deltaZ));
	//	XMStoreFloat4(&m_center, XMVector4Transform(XMLoadFloat4(&m_center), XMMatrixTranslation(deltaX, deltaY, deltaZ)));
}

XMFLOAT4 GameModel::getCenter()
{
	XMFLOAT4 ret;
	XMFLOAT4X4 world = GetWorldMatrix();
	XMStoreFloat4(&ret, XMVector4Transform(XMLoadFloat4(&m_center), XMLoadFloat4x4(&world)));
	return ret;

}





//User Control Moved Methods

void GameModel::MoveLeft()
{
	worldTranslate(-TRANSLATION_INCREMENT, 0.0f, 0.0f);
}

void GameModel::MoveRight()
{
	worldTranslate(TRANSLATION_INCREMENT, 0, 0);
}

void GameModel::MoveUp()
{
	worldTranslate(0.0f, TRANSLATION_INCREMENT, 0.0f);
}

void GameModel::MoveDown()
{
	worldTranslate(0.0f, -TRANSLATION_INCREMENT, 0.0f);
}

void GameModel::RotateLeft()
{
	orientRotateY(-XM_PIDIV4*ROTATION_SPEED);
}

void GameModel::RotateRight()
{
	orientRotateY(XM_PIDIV4*ROTATION_SPEED);

}