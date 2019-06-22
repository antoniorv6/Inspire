#include <LoadingScreenState.hpp>
#include <RenderManager.h>
#include <EntityManager.h>
#include <EventManager.h>
#include <PhysicsManager.h>
#include <AIManager.hpp>
#include <InputManager.hpp>
#include <SoundManager.h>
#include <WaypointManager.hpp>
#include <CameraManager.hpp>
#include <CheckpointManager.hpp>
#include <PowerUpManager.hpp>
#include <GUIManager.hpp>
#include <StatesMachine.hpp>
#include <InGameMenuState.hpp>
#include <Event.h>
#include <MapManager.hpp>
#include <GenericData.hpp>
#include <MainMenuState.hpp>
#include <SelectorMenuState.hpp>
#include <PodiumState.hpp>
#include <glfw3.h>
#include <HUD.hpp>
#include <GUI.hpp>
#include <RaceState.hpp>

LoadingScreenState::LoadingScreenState(GenericData* c_gameData, int l_playerPU, std::string& c_playerName, std::string& c_playerDrone, std::string& c_playerTexture, const std::string& l_droneName)
{
    entityManager     = c_gameData->g_entityManager_ptr;
	render            = c_gameData->g_renderManager_ptr;
	inputManager      = c_gameData->g_inputManager_ptr;
	soundManager      = c_gameData->g_soundManager_ptr;
	powerUpManager    = c_gameData->g_powerUpManager_ptr;
	physicsManager    = c_gameData->g_physicsManager_ptr;
	checkpointManager = c_gameData->g_checkPointManager_ptr;
	m_mapManager      = c_gameData->g_mapManager_ptr;
	eventManager      = c_gameData->g_eventManager_ptr;
	aiManager	      = c_gameData->g_aiManager_ptr;
	cameraManager     = c_gameData->g_cameraManager_ptr;
	guiManager        = c_gameData->g_guiManager_ptr;
	positionManager   = c_gameData->g_positionManager_ptr;
	waypointManager   = c_gameData->g_waypointManager_ptr;
	debugGUIManager   = c_gameData->g_debugGuiManager_ptr;

	m_powerUpsTextures_v.emplace_back("shield_mini.jpg");
    m_powerUpsTextures_v.emplace_back("magnet_mini.jpg");
    m_powerUpsTextures_v.emplace_back("thief_mini.jpg");
    m_powerUpsTextures_v.emplace_back("cyberattack_mini.jpg");
    m_powerUpsTextures_v.emplace_back("freezer_mini.jpg");
    m_powerUpsTextures_v.emplace_back("mirror_mini.jpg");

	m_powerUpsSelected = l_playerPU;

	m_playerPU1 = (TypePU) (l_playerPU/10);
	//GetPowerUpString(&m_playerPU1, &m_playerPU1_str);

	m_playerPU2 = (TypePU) (l_playerPU%10);
	//GetPowerUpString(&m_playerPU2, &m_playerPU2_str);

	m_GUI_ptr = new GUI(c_gameData);
	//Now we have to draw the loading sprite in order to enable the player to see the loading screen
	m_GUI_ptr->AddLoadingBar(640, 550, 600, 200);

	m_gameData = c_gameData;

	playerData = {c_playerName, c_playerDrone, c_playerTexture, l_droneName,  1};
	
	m_possibleDrones.emplace_back(DroneData{"Jack", "rocket", "plaino.jpg", "rocket", 12});
    m_possibleDrones.emplace_back(DroneData{"Karen", "spider", "spiderdrone_texture.png", "spider", 12});
    m_possibleDrones.emplace_back(DroneData{"Lyanna", "science", "ScienceDroneTexture.jpg", "science", 12});
    m_possibleDrones.emplace_back(DroneData{"Vladimir", "music", "plaino.jpg", "music", 12});


	SetCharacterNames();
}

void LoadingScreenState::Init()
{
	statesMachine = StatesMachine::getInstance();
	//std::cout<<"Loading player..."<<std::endl;
	LoadPlayer();
	//std::cout<<"Player loaded!"<<std::endl;
	m_GUI_ptr->AddProgressToLoadingBar(10.0);
	render->GetRenderFacade()->DrawGUI();

	m_mapManager->SetGUI(m_GUI_ptr);
	m_mapManager->SetPlayer(m_player);
	
	//std::cout<<"Loading map..."<<std::endl;
	LoadMap();
	//std::cout<<"Map loaded!"<<std::endl;

	m_player->setAfterED((*m_initialPositions_vec)[3], m_player->getRot());
	physicsManager->CreateOctree();

	//std::cout<<"Loadign AI..."<<std::endl;
	LoadAI();
	physicsManager->CreateDronesRBList();
	// ====== INIT AI DATA ====== //
	aiManager->InitData();
	//std::cout<<"AI loaded!"<<std::endl;
	m_GUI_ptr->AddProgressToLoadingBar(10.0);
	render->RenderScene(m_player);

}

void LoadingScreenState::Update(float &dt)
{
	ChangeState();
}

void LoadingScreenState::SetCharacterNames()
{
	for(size_t i = 0; i<m_possibleDrones.size(); i++)
	{
		if(m_possibleDrones[i].m_characterName == playerData.m_characterName)
		{
			m_possibleDrones.erase(m_possibleDrones.begin() + i);
			break;
		}
	}
}

void LoadingScreenState::LoadPlayer()
{
	Entity* player = entityManager->CreateEntity(playerData.m_characterName,glm::vec3(0,0,0));
	cameraManager->SetTarget(player);
	render->CreateComponent(player, "Animated", playerData.droneModel);
	physicsManager->CreateComponent(player);
	physicsManager->CreateRigidBodySphere(player, 4.0f, 10.0f);	
	positionManager->CreateComponent(player);
	powerUpManager->CreatePowerUp(m_playerPU1, player);	//First Power Up -> Shield
	powerUpManager->CreatePowerUp(m_playerPU2 ,player);	//Second Power Up
	soundManager->CreateComponent(player, "Drone", "event:/DroneSounds/Drone", 1);
	soundManager->CreateComponent(player, "Collisions", "event:/DroneSounds/Collisions", 1);
	inputManager->CreateInputComponent(player);
	render->AddNode(player, 1);
	render->AddAnimatedSecuence(player, "Base", RenderUtilities::getInstance()->GetLODNamesVector(playerData.droneName, 3), true);
	render->SetSecuence(player->GetId(), "Base");
	render->GetRenderFacade()->SetAnimationFrameRate(player->GetId(), 1 / 3);
	render->AddTexture(player, playerData.droneName + "_1024.png", "Base");
	soundManager->CreateComponent(player,"Music","event:/Music/First_Circuit", 0);
	soundManager->CreateComponent(player, "Countdown", "event:/RaceEvents/Race_start", 0);
	debugGUIManager->CreateComponent(player, 20, 200, 150, 90);
	render->RenderScene(player);
	m_player = player;
}

void LoadingScreenState::LoadMap()
{
	m_mapManager->LoadLevel(0, true, true, false);
	m_initialPositions_vec = m_mapManager->GetInitialPosition();
}

void LoadingScreenState::LoadAI()
{
	srand (time(NULL));

	int rad = 15.0f;

	CreateAI(m_possibleDrones[0], (*m_initialPositions_vec)[0], rad, 1.3f,  4.5f);
	CreateAI(m_possibleDrones[1], (*m_initialPositions_vec)[1], rad,  1.2f, 4.8f);
	CreateAI(m_possibleDrones[2], (*m_initialPositions_vec)[2], rad,  1.4f, 5.0f);
}

void LoadingScreenState::CreateAI(const DroneData& c_droneData, const glm::vec3& l_position_v3, float l_radius_f, float l_mass_f, float l_force_f)
{
	TypePU l_PU_1;
	TypePU l_PU_2;
	int personality;
	IARandomValues(&l_PU_1, &l_PU_2, &personality);

	// ====== CREATING AI ENTITY ====== //
	Entity* AI = entityManager->CreateEntity(c_droneData.m_characterName, l_position_v3);
	render->CreateComponent(AI, "Animated", c_droneData.droneModel);
	physicsManager->CreateComponent(AI);
	physicsManager->CreateRigidBodySphere(AI, l_radius_f, l_mass_f);
	positionManager->CreateComponent(AI);
	//powerUpManager->CreatePowerUp(TypePU::reflector, AI);	//First Power Up 
	//powerUpManager->CreatePowerUp(TypePU::reflector, AI);	//Second Power Up
	powerUpManager->CreatePowerUp(l_PU_1, AI);	//First Power Up 
	powerUpManager->CreatePowerUp(l_PU_2, AI);	//Second Power Up 
	render->AddNode(AI);
	render->AddAnimatedSecuence(AI, "Base", RenderUtilities::getInstance()->GetLODNamesMatrix(c_droneData.droneModel, 3, 3), true);
	render->SetSecuence(AI->GetId(), "Base");
	render->AddTexture(AI, c_droneData.droneModel, "Base");
	render->ScaleSceneNode(AI, 15.0f);
	render->GetRenderFacade()->SetAnimationFrameRate(AI->GetId(), 1 / 3);
	aiManager->CreateDecisionComponent(AI, l_force_f, personality);
	soundManager->CreateComponent(AI,c_droneData.m_characterName, "event:/DroneSounds/Drone", 1);
}

void LoadingScreenState::IARandomValues(TypePU* PU1, TypePU* PU2, int* personality)
{
    *PU1 = (TypePU)(rand() % 6);
	*PU2 = (TypePU)(rand() % 6);

	while(*PU1 == *PU2)
	{
		*PU2 = (TypePU)(rand()%6);
	}

	*personality = (rand()%11);
}

void LoadingScreenState::ChangeState()
{
	delete m_GUI_ptr;
	m_GUI_ptr = nullptr;

	m_HUD_ptr = new HUD(m_gameData);
	m_HUD_ptr->AddNitroHUDElement(635, 650, 0, 25);
	
	m_HUD_ptr->AddCountDownHUDElement(635, 150, 200, 200);

	powerUpManager->InitHUD(m_HUD_ptr);
	positionManager->InitHUD(m_HUD_ptr);

	StatesMachine::getInstance()->AddState(std::make_unique<RaceState>(m_gameData, m_HUD_ptr));
	std::vector<std::string>empty;
    m_powerUpsTextures_v.swap(empty);
	m_possibleDrones.clear();
}

void LoadingScreenState::GetPowerUpString(TypePU* type, std::string* name)
{
	switch(*type)
    {
        case shield:
		*name = "Shield"; break;
        
        case magnet:
		*name = "Magnet"; break;
        
        case thief:
		*name = "Thief";  break;
        
        case cyberattack:
		*name = "Cyberattack";  break;
        
        case reflector:
		*name = "Reflector";  break;

        case freezing:
		*name = "Freezing";  break;
    }
}