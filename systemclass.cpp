////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"

SystemClass::SystemClass()
{
	//set pointers to our objects to null so if initialization of them
	//fails they will not mistakenly be used to clean up memory
	m_Input = 0;  
	m_Graphics = 0;
	m_Camera = 0;
	m_Sound = 0;
	m_GameModels = new ArrayList<GameModel>();
	m_persistentModels = new ArrayList<GameModel>();
	m_progress = 0;
	start= true;
	credit = false;
    startSelected = true;
	m_keys = new Key*[3];
	m_heart = 0;
	m_heartCont = 0;
	level = 1;
}


SystemClass::SystemClass(const SystemClass& other)
{
	//always implmement your own copy constructor even if it does nothing
}


SystemClass::~SystemClass()
{
	//Always implement your own destructor even if it does nothing.
	//If your class will ever have subclasses (through inheritance) then make the 
	//destructor virtual as in
	// virtual SystemClass::~SystemClass(){...}
	// otherwise you will likely have memory leaks
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into 
	// InitializeWindows() function which will cause them to get set.
	// They are passed in by reference so can be set from within the InitializeWindows() function

	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	// Create the actual window. This screenWidth and screenHeight variables will get set
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}
		// Initialize the input object.
	m_Input->Initialize();


	// Create the Camera object.
	m_Camera = new CineCameraClass(screenWidth,screenHeight);
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(-20.0f, 3.5f, -10.0f); //10 units along Z in front of origin
	m_Camera->SetDirection(0.0f, 0.0f, 1.0f); //look in positive Z direction
	m_Camera->SetUpDirection(0.0f, 1.0f, 0.0f); //up points in positive Y direction
	/*IMPORTANT:  note camera direction and up must be orthogonal */

	/*---------------------------------------------------------------------------------
    Sound Class Initialization
    ---------------------------------------------------------------------------------*/
    // Create the sound object.
    m_Sound = new SoundClass;
    if (!m_Sound)
    {
        return false;
    }

    // Initialize the sound object.
    result = m_Sound->Initialize(m_hwnd);
    if (!result)
    {
        MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
        return false;
    }

    // Play the .wav file from here
 //   m_Sound->PlayWaveFile();


	//Create the game objects for our game

	float sMenuWidthX = 9.5f;
    float sMenuHeightY = 7.0f;
	m_sMenu = new StartModel(sMenuWidthX, sMenuHeightY);

	//background and floor model
	float backgroundWidthX = 100;
	float backgroundHeightY = 10;
	m_background = new BackgroundModel(backgroundWidthX, backgroundHeightY);

	//create player and mobs
	float playerWidthX = 2.0f;
	float playerHeightY = 2.5f;
	m_player = new Player(playerWidthX, playerHeightY);

	m_heart = new Heart(0.3f, 0.3f, L"textures/items/heart.dds", 0);
	m_heartCont = new Heart(0.6f, 0.6f, L"textures/items/heart.dds", 1);
	m_keys[0] = new Key(0.75f, 0.75f, L"textures/items/key0.dds", 0);
	m_keys[1] = new Key(0.75f, 0.75f, L"textures/items/key1.dds", 1);
	m_keys[2] = new Key(0.75f, 0.75f, L"textures/items/key2.dds", 2);
	m_heartCont->worldTranslate(95.0f, 3.4f, 0.0f);

	m_mobs = new ArrayList<Enemy>();
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 10.0f, 0.2f, L"rookie", m_heart));
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 20.0f, 0.2f, L"rookie"));
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 25.0f, 0.2f, L"rookie", m_heart));
	m_mobs->add(new EnemyGiant(3.3f, 3.0f, 33.0f, 0.2f, L"warrior", m_keys[2]));
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 44.0f, 0.2f, L"rookie", m_heart));
	m_mobs->add(new EnemyGiant(3.3f, 3.0f, 55.0f, 0.2f, L"warrior", m_keys[1]));
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 69.0f, 0.2f, L"rookie", m_keys[0]));
	m_mobs->add(new EnemyGiant(2.7f, 2.45f, 77.0f, 0.2f, L"rookie", m_heart));
	m_mobs->add(new EnemyGiant(3.3f, 3.0f, 85.0f, 0.2f, L"warrior"));

	m_collisions = new Collisions(m_player);
	m_collisions->addItem(m_heart);
	m_collisions->addItem(m_heartCont);
	m_collisions->addItem(m_keys[0]);
	m_collisions->addItem(m_keys[1]);
	m_collisions->addItem(m_keys[2]);
	for (int i = 0; i < m_mobs->size(); i++)
		m_collisions->addMob(m_mobs->elementAt(i));

	
	//Add the  gameModel objects to the GameModels collection
	//that will be rendered by the graphics system
	
	m_persistentModels->add(m_sMenu->getGameModel());
	m_persistentModels->addAll(m_background->getGameModels());	
	m_persistentModels->addAll(m_player->getGameModels());
	m_persistentModels->add(m_heart);
	m_persistentModels->add(m_heartCont);
	m_persistentModels->add(m_keys[0]);
	m_persistentModels->add(m_keys[1]);
	m_persistentModels->add(m_keys[2]);
	m_GameModels->addAll(*m_persistentModels);
	for (int i = 0; i < m_mobs->size(); i++)
		m_GameModels->addAll(m_mobs->elementAt(i)->getGameModels());
	

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object. 	Engine.exe!WinMain(HINSTANCE__ * hInstance, HINSTANCE__ * hPrevInstance, char * pScmdline, int iCmdshow) Line 21	C++
	// The Graphics::Initialize will also call back into our game objects an initialize their ModelClass objects once the GraphicsClass has had
	// a chance to initialize

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd, m_Camera, m_GameModels);
	if(!result)
	{
		return false;
	}
	m_sMenu->setD3D(m_Graphics->getD3D());
	m_background->setD3D(m_Graphics->getD3D());
	m_player->setD3D(m_Graphics->getD3D());
	for (int i = 0; i < m_mobs->size(); i++)
		m_mobs->elementAt(i)->setD3D(m_Graphics->getD3D());
	m_player->setCamera(m_Camera);

	m_player->setSound(m_Sound);
	for (int i = 0; i < m_mobs->size(); i++)
		m_mobs->elementAt(i)->setSound(m_Sound);
	
	m_Sound->PlayBGM(0);

	m_progress = new Progress(m_player, m_Graphics->getD3D(), m_Sound);

	return true;
}


void SystemClass::Shutdown()
{
	//Shut down our game character objects and release their memory

	if (m_player)
	{
		m_player->Shutdown();
		delete m_player;
		m_player = 0;
	}

	if (m_mobs)
	{
		for (int i = 0; i < m_mobs->size(); i++)
		{
			if (m_mobs->elementAt(i))
			{
				m_mobs->elementAt(i)->Shutdown();
				delete m_mobs->elementAt(i);
			}
		}
		delete m_mobs;
		m_mobs = 0;
	}

	if (m_background)
	{
		m_background->Shutdown();
		delete m_background;
		m_background = 0;
	}

	//Shut down the graphics pipeline object and release its memory
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object's memory.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

    // Release the camera object's memory.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_collisions)
	{
		delete m_collisions;
		m_collisions = 0;
	}

	if (m_sMenu)
    {
        m_sMenu->Shutdown();
        delete m_sMenu;
        m_sMenu = 0;
    }

	if (m_Sound)
    {
		m_Sound->Shutdown();
        delete m_Sound;
        m_Sound = 0;
    }

	if (m_progress)
	{
		delete m_progress;
		m_progress = 0;
	}

	if (m_heart)
	{
		delete m_heart;
		m_heart = 0;
	}

	if (m_heartCont)
	{
		delete m_heartCont;
		m_heartCont = 0;
	}
	if (m_keys)
	{
		for (int i = 0; i < 3; i++)
		{
			if (m_keys[i])
				delete m_keys[i];
			m_keys[i] = 0;
		}
		delete[] m_keys;
		m_keys = 0;
	}

	//release the memory for the m_GameModels collection
	//all objects in it should have been released my the code immediately above
	if(m_GameModels)
	{
		delete m_GameModels;
		m_GameModels = 0;
	}

	if (m_persistentModels)
	{
		delete m_persistentModels;
		m_persistentModels = 0;
	}

	// Shutdown the actual Window's window.
	ShutdownWindows();
	
	return;
}


void SystemClass::Run()
{
	/*
	The Run function is where our application will loop and do all the application processing until we decide to quit. 
	The application processing is done in the Frame() function which is called each loop. 
	This is an important concept to understand as now the rest of our application must be written with this in mind. 
	The pseudo code looks like the following: 

	while not done {
        check for windows system messages
        process system messages
        process application loop
        check if user wanted to quit during the frame processing
		}


	*/
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	/*
	The following Frame function is where all the processing for our application is done. 
	So far it is fairly simple, we check the input object to see if the user has pressed escape and wants to quit. 
	If they don't want to quit then we call the graphics object to do its frame processing 
	which will render the graphics for that frame. 
	As the application grows we'll place more code here. 
	*/

	//Handle user inputs
	bool result;

	const int ascii_A = 65;
	const int ascii_B = 66;
	const int ascii_C = 67;
	const int ascii_D = 68;
	const int ascii_E = 69;
	const int ascii_F = 70;
	const int ascii_P = 80;
	const int ascii_R = 82;
	const int ascii_S = 83;
	const int ascii_W = 87;
	const int ascii_X = 88;
	const int ascii_Y = 89;
	const int ascii_Z = 90;
	const int ascii_1 = 49;
    const int ascii_2 = 50;
    const int ascii_3 = 51;
    const int ascii_4 = 52;
    const int ascii_5 = 53;
    const int ascii_6 = 54;

	if(start)
    {
        if(m_Input->IsKeyDown(VK_RETURN))
        {
            if(startSelected){
                m_Camera->SetPosition(3.4f, 3.5f, -10.0f);
                m_Sound->PlayDesiredFile(19);
                start = false;
                m_Graphics->setSprite(m_progress->getSprites());
                m_Graphics->setFont(m_progress->getFonts());
                m_Sound->PlayDesiredFile(5);
                m_Sound->PlayBGM(1);
            }else{
                m_Sound->PlayDesiredFile(19);
                m_sMenu->changeTexture_back(L"textures/background/credit.dds");
                credit = true;
            }
        }
        if(m_Input->IsKeyDown(VK_ESCAPE)){
            m_sMenu->changeTexture_back(L"textures/background/startmenu-start.dds");
            credit = false;
            startSelected = true;
           m_Sound->PlayDesiredFile(19);
        }
        if(!credit){
            if(m_Input->IsKeyDown(VK_DOWN)){
                if(startSelected){
                   m_Sound->PlayDesiredFile(18);
                    startSelected = false;
                    m_sMenu->changeTexture_back(L"textures/background/startmenu-credits.dds");
                }
            }
            if(m_Input->IsKeyDown(VK_UP)){
                if(!startSelected){
                   m_Sound->PlayDesiredFile(18);
                    startSelected = true;
                    m_sMenu->changeTexture_back(L"textures/background/startmenu-start.dds");
                }
            }
        }
    }
	else
	{
		// Check if the user pressed escape and wants to exit the application.
		if(m_Input->IsKeyDown(VK_ESCAPE))
		{
			return false;
		}

		//Move camera or models based on input
		//We will combinations for a key + arrow keys to control the camera
		if ( m_Input->IsKeyDown(VK_SHIFT) ){

		   if ( m_Input->IsKeyDown(VK_LEFT) ) //Move Camera Left
			  m_Camera->StrafeLeft();

		   if ( m_Input->IsKeyDown(VK_RIGHT) ) //Move Camera Right
			  m_Camera->StrafeRight();

		   if ( m_Input->IsKeyDown(VK_UP) ) //Camera Move Forward
			  m_Camera->MoveForward();

		   if ( m_Input->IsKeyDown(VK_DOWN) ) //Camera Pull Back
			  m_Camera->MoveBackward();
		}
		else if ( m_Input->IsKeyDown(VK_CONTROL) ){

		   if ( m_Input->IsKeyDown(VK_LEFT) ) //Pan Camera Left
			  m_Camera->PanLeft();

		   if ( m_Input->IsKeyDown(VK_RIGHT) ) //Pan Camera Right
			  m_Camera->PanRight();

		   if ( m_Input->IsKeyDown(VK_UP) ) //Tilt Camera Downward
			  m_Camera->TiltDown();

		   if ( m_Input->IsKeyDown(VK_DOWN) ) //Tilt Camera Upward
			  m_Camera->TiltUp();
		}
		else if ( m_Input->IsKeyDown(ascii_C) ){


		   if ( m_Input->IsKeyDown(VK_UP) ) //Crane Up
			  m_Camera->CraneUp();

		   if ( m_Input->IsKeyDown(VK_DOWN) ) //Crane Down
			  m_Camera->CraneDown();
		}
		else if ( m_Input->IsKeyDown(ascii_R) ){

		   if ( m_Input->IsKeyDown(VK_LEFT) ) //Roll Left
			  m_Camera->RollLeft();

		   if ( m_Input->IsKeyDown(VK_RIGHT) ) //Roll Right
			  m_Camera->RollRight();	
		}

		else if ( m_Input->IsKeyDown(ascii_Z) ){

		   if ( m_Input->IsKeyDown(VK_UP) ) //Zoom In
			  m_Camera->ZoomIn();

		   if ( m_Input->IsKeyDown(VK_DOWN) ) //Zoom Out
			  m_Camera->ZoomOut();
		}

		//MOVE GAME OBJECTS

		else {  
		//Just the arrow keys alone

		if ( m_Input->IsKeyDown(VK_LEFT)){
			m_player->moveBackward();
			if (m_Camera->GetPosition().x > 3.5f && m_player->getPosX() - m_Camera->GetPosition().x < -3.5f && !m_player->isDead())
				m_Camera->MoveForward(-FWD_SPEED_PLAYER);
		}

		if ( m_Input->IsKeyDown(VK_RIGHT)){
			m_player->moveForward();
			if (m_Camera->GetPosition().x < 94.5f && m_player->getPosX() - m_Camera->GetPosition().x > -0.5f && m_Camera->GetPosition().x > 0.0f && !m_player->isDead())
				m_Camera->MoveForward(FWD_SPEED_PLAYER);
		}

		if ( m_Input->IsKeyDown(VK_UP))
			m_player->moveUp();

		if ( m_Input->IsKeyDown(VK_DOWN))
			m_player->moveDown();

		if (m_Input->IsKeyDown(ascii_A))
			m_player->tryAnimate(0);

		if (m_Input->IsKeyDown(ascii_S))
			m_player->tryAnimate(1);

		if (m_Input->IsKeyDown(ascii_D))
			m_player->tryAnimate(2);
		}

		if (m_progress->timeIsUp())
		{
			m_progress->setTime(90);
			m_player->hit(m_player->getHealth());
		}
		if (m_player->isDead()) 
		{
			if (m_sMenu->getPosX() < 0.0f && !m_progress->isGameover())
			{
                m_sMenu->changeTexture_back(L"textures/background/dead.dds");
				m_sMenu->move(20.0f+m_Camera->GetPosition().x, 0.0f, 0.0f);
           //     m_Camera->SetPosition(-20.0f, 3.5f, -10.0f);
            }
			if (m_Input->IsKeyDown(VK_RETURN))
			{
				if (m_sMenu->getPosX() > 0.0f)
					m_sMenu->move(-(20.0f+m_Camera->GetPosition().x), 0.0f, 0.0f);
				if (m_player->getLives() > 0)
					m_player->respawn(true);
				else
				{
					if (!m_progress->isGameover())
					{
						m_sMenu->changeTexture_back(L"textures/background/gameover.dds");
						m_progress->setGameover(true);
						m_Camera->SetPosition(-20.0f, 3.5f, -10.0f);
						m_Sound->StopBGM();
						m_Sound->PlayDesiredFile(16);
					}
				}
			}

		}

		m_player->advance();
		for (int i = 0; i < m_mobs->size(); i++)
			m_mobs->elementAt(i)->advance(m_player);

		m_collisions->check();

		m_progress->updateSprites();
		m_Graphics->setSprite(m_progress->getSprites());
		m_Graphics->setFont(m_progress->getFonts());

		m_heart->advance();
		for (int i = 0; i < 3; i++)
			m_keys[i]->advance();
	}

	if (m_player->getPosX() >= 97.0f && m_progress->hasAllKeys() && !m_progress->getWon())
	{
		if (level == 4)  //last level
		{
			m_progress->setWon(true);
			m_Input->resetInput();
			m_sMenu->changeTexture_back(L"textures/background/win.dds");
			m_Camera->SetPosition(-20.0f, 3.5f, -10.0f);
			m_Sound->StopBGM();
			m_Sound->PlayDesiredFile(20);
		}
		else
		{
			wstringstream ss;
			ss << L"textures/background/lvl" << level+1 << L".dds";
			wstring str = ss.str();
			m_sMenu->changeTexture_back(&str[0]);
			m_Camera->SetPosition(-20.0f, 3.5f, -10.0f);
			if (m_Input->IsKeyDown(VK_RETURN))
				nextLevel();
		}
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame();
	if(!result)
	{
		return false;
	}
	return true;
}

void SystemClass::nextLevel()
{
	level++;
	m_Input->resetInput();
	int size = m_mobs->size();
	for (int i = 0; i < size; i++)
	{
		Enemy* temp = m_mobs->removeLast();
		m_collisions->removeMob(temp);
		delete temp;
	}

	m_player->scorePoints(10 * m_progress->getTime());
	m_player->respawn(false);
	m_player->clearKeys();

	generateMobsForLevel(level);
	
	m_background->changeTexture_back(level);
	m_background->changeTexture_floor(level);
	m_background->changeTexture_rightGate(level);
	m_background->changeTexture_leftGate(level);

	delete m_GameModels;
	m_GameModels = new ArrayList<GameModel>();
	m_GameModels->addAll(*m_persistentModels);
	for (int i = 0; i < m_mobs->size(); i++)
		m_GameModels->addAll(m_mobs->elementAt(i)->getGameModels());
	m_Graphics->resetGameModels(m_GameModels);

	XMFLOAT4 center = m_heartCont->getCenter();
	m_heartCont->worldTranslate(-center.x, -center.y, -center.z);
	m_heartCont->worldTranslate(95.0f, 0.4f, 0.0f);
}

void SystemClass::generateMobsForLevel(int lvl)
{
	switch (lvl)
	{
/*	case 1:
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 20.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyMan(2.5f, 2.0f, 12.0f, 0.2f, L"warrior", m_heart));
		m_mobs->add(new EnemyMan(3.0f, 2.4f, 30.0f, 0.2f, L"elite", m_keys[1]));

		m_mobs->add(new EnemyGiant(2.7f, 2.45f, 45.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 55.0f, 0.2f, L"warrior", m_keys[2]));
		m_mobs->add(new EnemyGiant(4.0f, 3.6f, 65.0f, 0.2f, L"elite", m_keys[0]));
		m_progress->setTime(250);
		break;
*/	case 2:
		m_mobs->add(new EnemyGiant(2.7f, 2.45f, 6.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyGiant(2.7f, 2.45f, 20.0f, 0.2f, L"rookie", m_keys[1]));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 35.0f, 0.2f, L"warrior", m_heart));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 45.0f, 0.2f, L"warrior", m_keys[2]));
		m_mobs->add(new EnemyGiant(4.0f, 3.6f, 55.0f, 0.2f, L"elite", m_keys[0]));
		m_mobs->add(new EnemyGiant(4.0f, 3.6f, 65.0f, 0.2f, L"elite", m_heart));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 70.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 85.0f, 0.2f, L"rookie"));
		m_progress->setTime(180);
		break;
	case 3:
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 10.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 17.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyGiant(2.7f, 2.45f, 15.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 23.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyMan(2.5f, 2.0f, 28.0f, 0.2f, L"warrior", m_heart));
		m_mobs->add(new EnemyMan(2.5f, 2.0f, 39.0f, 0.2f, L"warrior", m_keys[0]));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 55.0f, 0.2f, L"warrior", m_keys[2]));
		m_mobs->add(new EnemyGiant(4.0f, 3.6f, 65.0f, 0.2f, L"elite", m_heart));
		m_mobs->add(new EnemyMan(3.0f, 2.4f, 75.0f, 0.2f, L"elite", m_keys[1]));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 82.0f, 0.2f, L"warrior"));
		m_progress->setTime(200);
		break;
	case 4:
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 10.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 15.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 20.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 30.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 33.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyMan(2.5f, 2.0f, 42.0f, 0.2f, L"warrior", m_heart));
		m_mobs->add(new EnemyMan(3.0f, 2.4f, 60.0f, 0.2f, L"elite", m_keys[1]));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 63.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyGiant(2.7f, 2.45f, 65.0f, 0.2f, L"rookie", m_heart));
		m_mobs->add(new EnemyGiant(3.3f, 3.0f, 75.0f, 0.2f, L"warrior", m_keys[2]));
		m_mobs->add(new EnemyMan(2.0f, 1.6f, 80.0f, 0.2f, L"rookie"));
		m_mobs->add(new EnemyGiant(4.0f, 3.6f, 88.0f, 0.2f, L"elite", m_keys[0]));
		m_progress->setTime(250);
		break;
	}

	for (int i = 0; i < m_mobs->size(); i++)
	{
		Enemy* mob = m_mobs->elementAt(i);
		mob->setD3D(m_Graphics->getD3D());
		mob->setSound(m_Sound);
		m_collisions->addMob(mob);
	}
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	/*
	The MessageHandler function is where windows system messages are directed into our application
	Here we can listen for certain information that we are interested in. 
	Currently we will just read if a key is pressed or if a key is released and pass that information on to the input object. 
	All other information we will pass back to the windows default message handler. 
	*/
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			// The input object will be polled by our game logic

			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	/*
	The InitializeWindows will build the actual Windows window we will render to. 
	It sets (returns)  screenWidth and screenHeight back to the calling function
	so these values can be used throughout the application. 
	
	We create the window using some default settings to initialize a plain black window with no borders. 
	The function will make either a small window or make a full screen window depending on 
	the true or false setting of the global variable called FULL_SCREEN. 
	If this is set to true then we make the screen cover the entire users desktop window. 
	If it is set to false we just make a 800x600 window in the middle of the screen. 
	The FULL_SCREEN global variable at the top of the graphicsclass.h file in case you want to modify it. 
	
	It will make sense later why it is placed in that file instead of the header for this file.
	*/
	WNDCLASSEX wc; //struct which describes the window class (properties of the window)

	DEVMODE dmScreenSettings;
	int posX, posY;


	// Set an external static pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Game Project"; //Long-character, or wchar, string

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //http://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
	wc.lpfnWndProc   = WndProc; //our procedure to call back on window events
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance; //set window's application to this application
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW); //use standard arrow cursor for window (when shown)
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //brush for painting background colour
	wc.lpszMenuName  = NULL; //we have no menus with our window
	wc.lpszClassName = m_applicationName; //name of our application
	wc.cbSize        = sizeof(WNDCLASSEX);  //size of this structure in bytes
	
	// Register the window class.
	// The window class must be registered with Window's OS before the window
	// can actually be created.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	//http://msdn.microsoft.com/en-us/library/windows/desktop/ms632679(v=vs.85).aspx

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, 
		                    m_applicationName, 
							m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION, //WS_CAPTION for border with title
						    posX,  //screen X of window's top left corner
							posY, //creen Y of windowns's top left corner
							screenWidth,  //width of screen
							screenHeight, //height of screen
							NULL, 
							NULL, 
							m_hinstance, //the application instance
							NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW); //show  the window
	SetForegroundWindow(m_hwnd); //make window the foreground window
	SetFocus(m_hwnd);            //give window input focus

	// Hide the mouse cursor if you don't want it over your game window.
	//ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor again.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}