#include <RaceState.hpp>
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

/**
 * @brief Construct a new Race State:: Race State object
 * 
 * @param c_gameData 
 * @param l_playerPU 
 */
RaceState::RaceState(GenericData* c_gameData, HUD* c_HUD)
{
	entityManager  = c_gameData->g_entityManager_ptr;
	render         = c_gameData->g_renderManager_ptr;
	inputManager   = c_gameData->g_inputManager_ptr;
	soundManager   = c_gameData->g_soundManager_ptr;
	powerUpManager = c_gameData->g_powerUpManager_ptr;
	physicsManager = c_gameData->g_physicsManager_ptr;
	checkpointManager = c_gameData->g_checkPointManager_ptr;
	m_mapManager   = c_gameData->g_mapManager_ptr;
	eventManager   = c_gameData->g_eventManager_ptr;
	aiManager	   = c_gameData->g_aiManager_ptr;
	cameraManager  = c_gameData->g_cameraManager_ptr;
	guiManager     = c_gameData->g_guiManager_ptr;
	positionManager= c_gameData->g_positionManager_ptr;
	waypointManager= c_gameData->g_waypointManager_ptr;
	debugGUIManager= c_gameData->g_debugGuiManager_ptr;

	//m_powerUpsTextures_v.emplace_back("shield_mini.jpg");
    //m_powerUpsTextures_v.emplace_back("magnet_mini.jpg");
    //m_powerUpsTextures_v.emplace_back("thief_mini.jpg");
    //m_powerUpsTextures_v.emplace_back("cyberattack_mini.jpg");
    //m_powerUpsTextures_v.emplace_back("freezer_mini.jpg");
    //m_powerUpsTextures_v.emplace_back("mirror_mini.jpg");
	
	m_gameData = c_gameData;
	actualCountDown = 0;
	lastCountDown = 0;

	m_HUD_ptr = c_HUD;

	//m_powerUpsSelected = l_playerPU;

	//m_playerPU1 = (TypePU) (l_playerPU/10);
	//GetPowerUpString(&m_playerPU1, &m_playerPU1_str);

	//m_playerPU2 = (TypePU) (l_playerPU%10);
	//GetPowerUpString(&m_playerPU2, &m_playerPU2_str);
}

/**
 * @brief Destroy the Race State:: Race State object
 * 
 */
RaceState::~RaceState()
{

}

/**
 * @brief 
 * 
 */
void RaceState::Init()
{
	// ====== GET A STATES MACHINE INSTANCE ====== //
	statesMachine = StatesMachine::getInstance();
	// ====== DEPLOY PLAYER ENTITY ====== //
	//Entity* player = entityManager->CreateEntity("Player",glm::vec3(0,0,0));
	//DeployPlayer(player);
	//// ====== LOADING RACING LEVEL ====== //
	//LoadLevel();
	//// ====== SET PLAYER CORRECT POSITION ====== //
	//player->setAfterED((*m_initialPositions_vec)[3], player->getRot());
	//// ====== CREATE OCTREE ====== //
	//physicsManager->CreateOctree();
	////====== DEPLOY AI ENTITIES ====== //
	//DeployAI();
	//// ====== CREATING DRONES LIST ====== //
	//physicsManager->CreateDronesRBList();
	//// ====== INIT AI DATA ====== //
	//aiManager->InitData();
	// ====== GET PLAYER ENTITY ====== //
	m_player = entityManager->GetEntity(0);
	// ====== INIT CLOCK ====== //
	m_clock = new Clock();
	m_clock->Init();
	// ====== GET POINTER TO ISPLAY BOOLEAN ====== //
	isPlay = positionManager->IsPlay();
	// ====== INIT CAMERA ======= //
	cameraManager->InitCameraData(1);
	//======= ACTIVATE SCHEDULING ==== //
	aiManager->SetScheduling(true);

	// render->AddParticlesEmitter("particles", "thunder", "thunderTexture.jpg", {-5973.52, 0, -10973.9}, 25);
	//render->AddLodMesh(glm::vec3(-5973.52, 0, -10973.9));
}

/**
 * @brief: Updates the entire level
 * 
 * @param delta 
 */
void RaceState::Update(float &delta)
{
	if(*isPlay)
	{
		if(delta >= loopTime)
		{
			//std::cout << "update" << std::endl;
			inputManager->Update();
			eventManager->update();
			physicsManager->Update(delta,loopTime);
			aiManager->Update();
			powerUpManager->Update();
			positionManager->Update();
			cameraManager->UpdateCameraPosition(delta);
			UpdateGUI(m_player);
			//soundManager->Stop(player, "Drone");
			delta = 0; //reset accumulated time
		}
		Scheduling();
		InterpolateDrones(delta);
	}
	else
	{	
		if(isCountdown)
		{
			Countdown(delta);
		}
		else
		{
			EndOfTheRace(delta);
		}
	} 
}

/**
 * @brief 
 * 
 * @param delta 
 */
void RaceState::Countdown(float& delta)
{	
	delta = 0;
	physicsManager->Update(delta,loopTime);
	positionManager->Update();
	cameraManager->UpdateCameraPosition(delta);
	UpdateGUI(m_player);
	InterpolateDrones(delta);

	actualCountDown = (int)m_clock->GetElapsedTime();

	if((actualCountDown - lastCountDown) == 1)
	{
		soundManager->SetEventVariableValue(m_player, "Countdown", "CountDown", actualCountDown);
		soundManager->Play(m_player, "Countdown");
		lastCountDown = actualCountDown;
	}

	switch (actualCountDown)
	{
		case 1:
			m_HUD_ptr->UpdateCountDownHUD(2);
			break;
		case 2:
			m_HUD_ptr->UpdateCountDownHUD(1);
			break;
		case 3:
			m_HUD_ptr->UpdateCountDownHUD(0);
			break;
		case 4:
			//system("clear");
			//std::cout << "GO!!!!!!!\n";
			*isPlay = true;
			isCountdown = false;
			soundManager->PlayAllSounds();
			soundManager->CreateComponent(m_player, "PowerUP", "event:/RaceEvents/PowerUP", 0);
			soundManager->CreateComponent(m_player, "MenuSounds", "event:/InterfaceEvents/MenuButtons", 0);
			m_HUD_ptr->UpdateCountDownHUD(-1);
			break;
	
		default:
			break;
	}	
}

/**
 * @brief 
 * 
 * @param delta 
 */
void RaceState::EndOfTheRace(float& delta)
{
	if(!isEndRace)
	{
		m_clock->Init();
		aiManager->SetScheduling(false);
		isEndRace = true;
	}

	if(delta >= loopTime)
	{
		
		eventManager->update();
		physicsManager->Update(delta,loopTime);
		aiManager->Update();
		powerUpManager->Update();
		positionManager->Update();
		cameraManager->UpdateCameraPosition(delta);
		UpdateGUI(m_player);
		delta = 0; 
	}
	
	Scheduling();
	InterpolateDrones(delta);

	if(m_clock->GetElapsedTime() > 5.0f)
	{
		//IR AL MENU PRINCIPAL
		ChangeState();
	}

}

/**
 * @brief 
 * 
 */
void RaceState::Scheduling()
{
	if(aiManager->GetScheduling())
	{
		double frameTime = m_clock->GetElapsedTime();
		m_clock->Restart();
		aiManager->Scheduling(frameTime);

		FPS(frameTime);
	}
	else
	{
		aiManager->ClearAIQueue();
	}
}

/**
 * @brief 
 * 
 * @param frametime 
 */
void RaceState::FPS(double frametime)
{
	m_FPStime_d += frametime;

	if(m_FPStime_d >= 1.0f)
	{
		m_FPS_n = m_FPScounter_n;
		m_FPStime_d = 0;
		m_FPScounter_n = 0;		
	}

	m_FPScounter_n++;
}

/**
 * @brief 
 * 
 * @param delta 
 */
void RaceState::InterpolateDrones(float &delta)
{
	//Interpolate Drone's Positions in PhysicsManager
	physicsManager->InterpolatePositions(delta,loopTime);
	//Update Node's Positions
	for(unsigned int i=0; i<physicsManager->GetPhysicsComponentList().size(); i++)
	{

		Entity* entity = entityManager->GetEntity(physicsManager->GetPhysicsComponentList()[i]->GetEntityId());

		if(physicsManager->GetDebugBB())
			render->TranslateBSphere(entity->GetId(),entity->getPos());
			
		render->GetRenderFacade()->TranslateSceneNode(entity->GetId(),entity->getPos());
		render->GetRenderFacade()->RotateSceneNode(entity->GetId(),entity->getRot());

	}
	//Interpolate Camera's Position
	cameraManager->InterpolateCameras(delta, loopTime);

	//Interpolate Active Billboard
	positionManager->BillboardsUpdate();
	powerUpManager->UpdateBillboardsPU();
}

/**
 * @brief 
 * 
 */
void RaceState::Draw()
{
	if(m_player)
	{
		render->RenderScene(m_player);
		UpdateSound(m_player);
	}
}

/**
 * @brief 
 * 
 */
void RaceState::LoadLevel()
{
	m_mapManager->LoadLevel(0, true, true, false);
	m_initialPositions_vec = m_mapManager->GetInitialPosition();
}

/**
 * @brief 
 * 
 * @param player 
 */
void RaceState::UpdateSound(Entity* player)
{
	soundManager->Update( cameraManager->GetActiveCameraPosition() );
}

/**
 * @brief 
 * 
 */
void RaceState::DeployPlayer(Entity * player)
{
	cameraManager->SetTarget(player);
	render->CreateComponent(player, "Animated", "ScienceDrone");
	physicsManager->CreateComponent(player);
	physicsManager->CreateRigidBodySphere(player, 4.0f, 10.0f);	
	positionManager->CreateComponent(player);
	powerUpManager->CreatePowerUp(m_playerPU1, player);	//First Power Up -> Shield
	powerUpManager->CreatePowerUp(m_playerPU2 ,player);	//Second Power Up
	soundManager->CreateComponent(player, "Drone", "event:/DroneSounds/Drone", 1);
	inputManager->CreateInputComponent(player);

	render->AddNode(player);
	render->AddAnimatedSecuence(player, "Base", {"frame1", "frame2", "frame3", "frame4"}, true);
	render->SetSecuence(player->GetId(), "Base");
	render->AddTexture(player, "spiderdrone_texture.png", "Base");
	
	//render->ScaleSceneNode(player, 1.3f);

	//m_HUD_ptr = new HUD(m_gameData);
	//m_HUD_ptr->AddNitroHUDElement(635, 650, 0, 25);
	//m_HUD_ptr->AddPowerupHUDElement(1100, 650, m_powerUpsTextures_v[(m_powerUpsSelected/10)], true);
	//m_HUD_ptr->AddPowerupHUDElement(1200, 650, m_powerUpsTextures_v[(m_powerUpsSelected%10)], false);

	positionManager->InitHUD(m_HUD_ptr);
    
	soundManager->CreateComponent(player,"Music","event:/Music/First_Circuit", 0);
	soundManager->CreateComponent(player, "Countdown", "event:/RaceEvents/Race_start", 0);
	debugGUIManager->CreateComponent(player, 20, 200, 150, 90);
}

/**
 * RaceState::LoadAI
 * 
 * @brief 
 * 
 */
void RaceState::DeployAI()
{
	srand (time(NULL));

	int rad = 15.0f;

	CreateAI("Karen", (*m_initialPositions_vec)[0], rad, 1.3f,  4.5f, "celeste.jpg");
	CreateAI("Lyanna", (*m_initialPositions_vec)[1], rad,  1.2f, 4.8f, "naranja.jpg");
	CreateAI("Vladimir", (*m_initialPositions_vec)[2], rad,  1.4f, 5.0f, "lila.jpg");

	// glm::vec3 pos = (*m_initialPositions_vec)[0];
	// pos = { -pos.x, pos.z, -pos.y}; //irrlitch axis to openGL axis
	// float rad = 20;
	// render->AddDebugCube(1, glm::vec3{pos.x, pos.y - (rad/2), pos.z}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(rad, 0.1f, rad));
	// render->AddDebugCube(2, glm::vec3{pos.x, pos.y + (rad/2), pos.z}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(rad, 0.1f, rad));
	// render->AddDebugCube(3, glm::vec3{pos.x - (rad/2), pos.y, pos.z}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, rad, rad));
	// render->AddDebugCube(4, glm::vec3{pos.x + (rad/2), pos.y, pos.z}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, rad, rad));
	// render->AddDebugCube(5, glm::vec3{pos.x, pos.y, pos.z - (rad/2)}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(rad, rad, 0.1f));
	// render->AddDebugCube(6, glm::vec3{pos.x, pos.y, pos.z + (rad/2)}, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(rad, rad, 0.1f));

}

/**
 * RaceState::CreateAI
 * 
 * @brief 
 * 
 * @param position 
 * @param radius 
 * @param mass 
 * @param force 
 */
void RaceState::CreateAI(const std::string& l_id_s, const glm::vec3& l_position_v3, float l_radius_f, float l_mass_f, float l_force_f, const std::string l_texture_str)
{	
	TypePU l_PU_1;
	TypePU l_PU_2;
	int personality;
	IARandomValues(&l_PU_1, &l_PU_2, &personality);

	// ====== CREATING AI ENTITY ====== //
	Entity* AI = entityManager->CreateEntity(l_id_s, l_position_v3);
	render->CreateComponent(AI, "Mesh", "spiderdrone_scaled");
	physicsManager->CreateComponent(AI);
	physicsManager->CreateRigidBodySphere(AI, l_radius_f, l_mass_f);
	positionManager->CreateComponent(AI);
	powerUpManager->CreatePowerUp(l_PU_1, AI);	//First Power Up 
	powerUpManager->CreatePowerUp(l_PU_2, AI);	//Second Power Up 
	render->AddNode(AI);
	render->AddTexture(AI, l_texture_str);
	render->ScaleSceneNode(AI, 15.0f);
	aiManager->CreateDecisionComponent(AI, l_force_f, personality);
	soundManager->CreateComponent(AI,l_id_s, "event:/DroneSounds/Drone", 1);
}

/**
 * @brief 
 * 
 * @param PU1 
 * @param PU2 
 * @param personality 
 */
void RaceState::IARandomValues(TypePU* PU1, TypePU* PU2, int* personality)
{
    *PU1 = (TypePU)(rand() % 6);
	*PU2 = (TypePU)(rand() % 6);

	while(*PU1 == *PU2)
	{
		*PU2 = (TypePU)(rand()%6);
	}

	*personality = (rand()%11);
}

/**
 * @brief 
 * 
 * @param player 
 */
void RaceState::UpdateGUI(Entity * player)
{
	//guiManager->MakeResponsiveAdaptation();
	TransformComponent* nitroComp = ((TransformComponent*)entityManager->GetComponent(player->GetId(), "Physics"));
	float nitroValue = nitroComp->GetPhysicsData()->chargeNITRO;
	m_HUD_ptr->UpdateNitro(nitroValue);
	//guiManager->ResizeNitroRectangle(player,nitroValue);
}

/**
 * @brief 
 * 
 */
void RaceState::EndGame()
{
	positionManager->EndGame();
	render->ActivatePPEffect("defaultPP");
}

/**
 * @brief 
 * 
 */
void RaceState::ChangeState()
{
	if(*isPlay)
		StatesMachine::getInstance()->AddState(std::make_unique<MainMenuState>(m_gameData));
	else
	{
		std::string playername = m_player->GetName();
		StatesMachine::getInstance()->AddState(std::make_unique<PodiumState>(m_gameData, playername));
	}
		
	EndGame();
    render->Reset();
	//soundManager->StopAllSounds();

    entityManager->ClearList();         
	inputManager->ClearList();

	soundManager->StopAllSounds(); 
	soundManager->ClearList();   
	
	powerUpManager->ClearList();
	eventManager->ClearList(); 
	physicsManager->ClearList(); 
	checkpointManager->ClearList();   
	aiManager->ClearList();	   
	cameraManager->ClearList();  
	guiManager->ClearList();     
	waypointManager->ClearList();
	debugGUIManager->ClearList();
	positionManager->ClearList();
	
	soundManager->UpdateSoundEngine();

	std::vector<std::string>empty;
    
    m_powerUpsTextures_v.swap(empty);

	m_player = nullptr;
	delete m_clock;
	m_clock = nullptr;
	
	delete m_HUD_ptr;
	m_HUD_ptr = nullptr;
}

/**
 * @brief 
 * 
 */
void RaceState::CloseState()
{
	delete m_clock;
	m_clock = nullptr;
	
	delete m_HUD_ptr;
	m_HUD_ptr = nullptr;

	std::vector<std::string>empty;
    m_powerUpsTextures_v.swap(empty);
}

/**
 * @brief 
 * 
 */
void RaceState::DrawStateGUI()
{
	ImGui::SetNextWindowSize(ImVec2(150, 100));
    ImGui::SetNextWindowPos(ImVec2(20, 100));
    ImGui::Begin("RaceState Control");
    if(ImGui::Button("Pause", ImVec2(90,40)))
	{
		this->Pause();
	}
    ImGui::End();

	debugGUIManager->AddTextToInterface(m_player, "FPS: " + std::to_string(m_FPS_n));
	debugGUIManager->AddTextToInterface(m_player, "Pos: " + std::to_string(((PositionComponent*)entityManager->GetComponent(m_player->GetId(), "Position"))->GetPosition()) + "º");
	debugGUIManager->AddTextToInterface(m_player, "L: " + m_playerPU1_str);
	debugGUIManager->AddTextToInterface(m_player, "R: " + m_playerPU2_str);
}

/**
 * @brief 
 * 
 */
void RaceState::Pause()
{
	PauseClocks();
	soundManager->SetEventVariableValue(m_player,"Music", "Pause", 1.0);
	soundManager->PauseAllDynamicSounds();
	statesMachine->AddState(std::make_unique<InGameMenuState>(m_gameData, entityManager, render, soundManager, eventManager),false);
	//std::cout<<"I have to pause this game"<<std::endl;
}

/**
 * @brief 
 * 
 */
void RaceState::PauseClocks()
{
	m_clock->Pause();
	powerUpManager->PauseClocks();
	physicsManager->PauseClocks();
}

/**
 * @brief 
 * 
 */
void RaceState::Resume()
{
	PlayClocks();
	soundManager->SetEventVariableValue(m_player,"Music", "Pause", 0.0);
	soundManager->ResumeAllDynamicSounds();
	//std::cout<<"He resumeado"<<std::endl;
}

/**
 * @brief 
 * 
 */
void RaceState::PlayClocks()
{
	m_clock->Play();
	powerUpManager->PlayClocks();
	physicsManager->PlayClocks();
}

/**
 * @brief 
 * 
 * @param type 
 * @param name 
 */
void RaceState::GetPowerUpString(TypePU* type, std::string* name)
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