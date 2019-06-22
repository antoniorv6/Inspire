#include <Game.h>
#include <Event.h>
#include <AIManager.hpp>
#include <SoundManager.h>
#include <CameraManager.hpp>
#include <GUIManager.hpp>
#include <MainMenuState.hpp>
#include <MapManager.hpp>
#include <RenderManager.h>
#include <InputManager.hpp>
#include <PowerUpManager.hpp>
#include <SoundManager.h>
#include <GenericData.hpp>
#include <CheckpointManager.hpp>
#include <EventManager.h>


Game::Game(int l_graphicEngine_n) : m_graphicEngine_n(l_graphicEngine_n)
{
	m_entityManager_ptr 	= new EntityManager();
	m_eventManager_ptr		= new EventManager();
	m_checkPointManager_ptr = new CheckpointManager();
	m_physicManager_ptr 	= new PhysicsManager();
	m_positionManager_ptr   = new PositionManager();
	m_renderManager_ptr 	= new RenderManager();
	m_inputManager_ptr  	= new InputManager(m_graphicEngine_n);
	m_guiManager_ptr        = new GUIManager();
	m_debugGuiManager_ptr   = new DebugGUIManager();
	m_soundManager_ptr  	= new SoundManager();
	m_powerUpManager_ptr	= new PowerUpManager();
	m_waypointManager_ptr   = new WaypointManager();
	m_mapManager_ptr		= new MapManager();
	m_AIManager_ptr         = new AIManager();
	m_cameraManager_ptr		= new CameraManager();
	m_statesMachine_ptr     = StatesMachine::getInstance();

	m_gameData_ptr = new GenericData(m_entityManager_ptr, m_renderManager_ptr, m_soundManager_ptr, m_inputManager_ptr, m_physicManager_ptr, m_powerUpManager_ptr, 
									 m_checkPointManager_ptr, m_mapManager_ptr, m_eventManager_ptr, m_AIManager_ptr, m_cameraManager_ptr, m_guiManager_ptr,
									 m_debugGuiManager_ptr, m_positionManager_ptr, m_waypointManager_ptr);

	//INITIALIZATION ZONE
	m_checkPointManager_ptr->Init(m_gameData_ptr);
	m_physicManager_ptr->Init(m_gameData_ptr);
	m_renderManager_ptr->Init(m_graphicEngine_n, m_gameData_ptr);
	m_positionManager_ptr->Init(m_gameData_ptr);
	m_inputManager_ptr->Init(m_graphicEngine_n, m_gameData_ptr);
	m_guiManager_ptr->Init(m_gameData_ptr);
	m_debugGuiManager_ptr->Init(m_gameData_ptr);
	//1 == irrlicht, 0 == Genesis
	m_soundManager_ptr->Init(m_gameData_ptr);
	m_powerUpManager_ptr->Init(m_gameData_ptr);
	m_waypointManager_ptr->Init(m_gameData_ptr);
	m_mapManager_ptr->Init(m_gameData_ptr);
	m_AIManager_ptr->Init(m_gameData_ptr);
	m_cameraManager_ptr->Init(m_gameData_ptr);

	m_statesMachine_ptr->AddState(std::make_unique<MainMenuState>(m_gameData_ptr));
	m_statesMachine_ptr->ProcessStateChanges(clock, totalTime);
	this->Run();
}

void Game::Run()
{
	clock = new Clock();
	clock->Init();
	
	while(m_renderManager_ptr->getPlay())
	{
		deltaTime = (float)clock->GetElapsedTime();
		totalTime += deltaTime;
		clock->Restart();

		m_statesMachine_ptr->ProcessStateChanges(clock, totalTime);
		m_statesMachine_ptr->GetActiveState()->Update(totalTime);
		m_statesMachine_ptr->GetActiveState()->Draw();
	}
	m_statesMachine_ptr->GetActiveState()->CloseState();
	m_renderManager_ptr->GetRenderFacade()->CloseWindow();
	m_soundManager_ptr->StopAllSounds();

	delete m_entityManager_ptr;
	delete m_inputManager_ptr;
	delete m_renderManager_ptr;
	delete m_soundManager_ptr;
	delete m_powerUpManager_ptr;
	delete m_checkPointManager_ptr;
	delete m_AIManager_ptr;
	delete m_mapManager_ptr;
	delete m_eventManager_ptr;
	delete m_cameraManager_ptr;
	delete m_gameData_ptr;
	delete m_guiManager_ptr;
	delete m_debugGuiManager_ptr;
	delete m_positionManager_ptr;
	delete m_waypointManager_ptr;
	delete m_physicManager_ptr;

	m_entityManager_ptr     = nullptr;
	m_inputManager_ptr      = nullptr;
	m_renderManager_ptr     = nullptr;
	m_soundManager_ptr      = nullptr;
	m_powerUpManager_ptr    = nullptr;
	m_checkPointManager_ptr = nullptr;
	m_AIManager_ptr         = nullptr;
	m_mapManager_ptr		= nullptr;
	m_eventManager_ptr		= nullptr;
	m_cameraManager_ptr		= nullptr;
	m_gameData_ptr			= nullptr;
	m_guiManager_ptr		= nullptr;
	m_debugGuiManager_ptr	= nullptr;
	m_positionManager_ptr	= nullptr;
	m_waypointManager_ptr	= nullptr;
	m_physicManager_ptr		= nullptr;

	
	delete clock;
	clock = nullptr;
}
