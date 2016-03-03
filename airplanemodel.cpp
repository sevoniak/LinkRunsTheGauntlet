#include "gamemodel.h"
#include "modelclass.h"
#include "airplanemodel.h"
#include "arraylist.h"

/*
This class represents a simple airplane model built out of cube and prism game model components
It can be moved forward in the direction it is facing and turned left or right.

Exercise: it would be cool if it actually banked while turning.
How would you do this?
*/
AirPlaneModel::AirPlaneModel()
{
	m_Fuselage = 0;
	m_Wing = 0;
	m_Propeller = 0;

	//direction plane is facing as constructed in the model co-ordinates
    m_directionVector = XMFLOAT3(1.0f,0.0f,0.0f); //face in positive X direction

	InitializeModel();
}

AirPlaneModel::~AirPlaneModel(void)
{ 
	Shutdown();
}

ArrayList<GameModel> AirPlaneModel::GetGameModels(){

	//Provide an arraylist of simple GameModel components that make up this airplane
	//These will be given to the GraphicsClass object to render using the graphics pipeline

	ArrayList<GameModel> list;
	list.add(m_Fuselage);
	list.add(m_Wing);
	list.add(m_Propeller);

	return list;
}


void AirPlaneModel::InitializeModel()
{

	/*
	Create the airplane out of simple CubeModel and PrismModel parts
	*/

	//Define dimensions of parts of the plane so they can easily be changed in one place

	float fuselageLength = 4.0f; //length of body of plane
	float fuselageRadius = fuselageLength/6; //radius of plane's body
	float wingSpan = fuselageLength;
	float wingWidth = fuselageRadius * 2;
	float wingThickness = 0.2f;
	float propellerLength = fuselageRadius * 3.0f;
	float propellerWidth = fuselageRadius * 0.5f;
	float propellerThickness = 0.1f;;

	//fuselage colours
	XMFLOAT4 prismColors[] = {
	    XMFLOAT4(230.0f/255, 0.0f, 0.0f, 1.0f), //prism faces red1
	    XMFLOAT4(204.0f/255, 0.0f, 0.0f, 1.0f), //prism ends red2
	};

	m_Fuselage = new PrismModel( fuselageLength,  //height
							     fuselageRadius,  //radius
							     12,      //number of faces
							     prismColors //side and end colours 
							    ); 

    m_Fuselage->orientRotateZ(XM_PIDIV2); //orient fuselage along X axis
    m_directionVector = XMFLOAT3(1.0f,0.0f,0.0f); //plane is facing in positive X direction



	XMFLOAT4 wingColors[] = {
	    XMFLOAT4(153.0f/255, 76.0f/255, 0.0f, 1.0f), //front face brown1
	    XMFLOAT4(153.0f/255, 76.0f/255, 0.0f, 1.0f), //back face brown1
	    XMFLOAT4(204.0f/255, 102.0f/255, 0.0f, 1.0f), //right side brown2
	    XMFLOAT4(204.0f/255, 102.0f/255, 0.0f, 1.0f), //left side brown2
	    XMFLOAT4(255.0f/255, 128.0f/255, 0.0f, 1.0f), //top face brown3
	    XMFLOAT4(255.0f/255, 128.0f/255, 0.0f, 1.0f), //bottom face brown3
	};

	//Create Wing of airplane
	m_Wing = new CubeModel(  wingWidth,  //width of wing
							 wingSpan,  //wing span
							 wingThickness, //thickness of wing
							 wingColors //XMFLOAT4[6] with face colours
							 );

	//position wing relative to the fuselage
	m_Wing->orientRotateX(XM_PIDIV2);
	m_Wing->orientTranslate(fuselageRadius/4, fuselageRadius, 0.0f);

	//Create propeller of airplane
	m_Propeller = new CubeModel( propellerThickness,  //length in X direction
							     propellerLength,  //length in Y direction
							     propellerWidth, //length in Z direction
							     wingColors //XMFLOAT4[6] with face colours
							    );

	//position plane relative to fuselage
	m_Propeller->orientTranslate(fuselageLength/2 + 0.2f, 0.0f, 0.0f);


}

void AirPlaneModel::Advance(){
	//Advance moving parts of the plane
	//This method should be called on a per-frame basis
	
	//Spin the propeller
	float propellerRadianAngularIncrement = -XM_PIDIV4/5; //sign changes direction of the propeller
	m_Propeller->orientRotateX(propellerRadianAngularIncrement);
}

void AirPlaneModel::MoveForward(){

	  //Move the airplane in the direction it is facing based on its
	  //direction vector and the speed factor which can be adjusted for smooth motion

	  XMFLOAT3 effectiveDirectionVector;
	  XMFLOAT4X4 fuselageDirectionMatrix = m_Fuselage->GetWorldRotateMatrix();
      XMStoreFloat3( &effectiveDirectionVector,  XMVector3Transform( XMLoadFloat3(&m_directionVector), XMLoadFloat4x4(&fuselageDirectionMatrix) ));

	  float deltaX = effectiveDirectionVector.x*AIRPLANE_FORWARD_SPEED_FACTOR;
	  float deltaY = effectiveDirectionVector.y*AIRPLANE_FORWARD_SPEED_FACTOR;
	  float deltaZ = effectiveDirectionVector.z*AIRPLANE_FORWARD_SPEED_FACTOR;

	  //Move all the  component parts of the airplane relative to the world
	  //co-ordinate system

	  m_Fuselage->worldTranslate(deltaX, deltaY, deltaZ);
	  m_Wing->worldTranslate(deltaX, deltaY, deltaZ);
	  m_Propeller->worldTranslate(deltaX, deltaY, deltaZ);

}


void AirPlaneModel::TurnLeft(){
	
	//Rotate, or Yaw, the plane about the Y axis

	float rotationRadianAngle = -XM_PIDIV4/40;
	m_Fuselage->worldRotateY(rotationRadianAngle);
	m_Wing->worldRotateY(rotationRadianAngle);
	m_Propeller->worldRotateY(rotationRadianAngle);

}
void AirPlaneModel::TurnRight(){

	//Rotate, or Yaw, the plane about the Y axis

	float rotationRadianAngle = XM_PIDIV4/40;
	m_Fuselage->worldRotateY(rotationRadianAngle);
	m_Wing->worldRotateY(rotationRadianAngle);
	m_Propeller->worldRotateY(rotationRadianAngle);

}



void AirPlaneModel::Shutdown()
{
	//Release the memory of the component parts of the airplane

	if(m_Fuselage)
	{
		delete m_Fuselage;
		m_Fuselage = 0;
	}

	if(m_Wing)
	{
		delete m_Wing;
		m_Wing = 0;
	}

	if(m_Propeller)
	{
	  delete m_Propeller;
	  m_Propeller = 0;
	}


}
