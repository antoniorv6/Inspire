#include <OnlineRaceState.hpp>
#include <RakString.h>
#include <BitStream.h>

#include <EntityManager.h>
#include <RenderManager.h>
#include <CameraManager.hpp>
#include <InputManager.hpp>
#include <PhysicsManager.h>
#include <MapManager.hpp>
#include <Clock.hpp>
#include <GenericData.hpp>
#include <GUIManager.hpp>
#include <PositionManager.hpp>
#include <PowerUpManager.hpp>
#include <EventManager.h>
#include <CheckpointManager.hpp>
#include <SoundManager.h>
#include <StatesMachine.hpp>
#include <MainMenuState.hpp>


#include <iostream>

#include <OrionEnums.hpp>
#include <OrionStructs.hpp>

struct OnlineGameCallbacks
{
    RakNet::MessageID m_id;
    void (OnlineRaceState::*CallbackFunc)();
};

OnlineGameCallbacks m_clientCallbacks [] = 
{
		{ID_CREATE_PLAYER, 				&OnlineRaceState::CreateMainPlayer 		  },
		{ID_CREATE_EXTERMAL_ENTITY, 	&OnlineRaceState::CreateExternalEnt		  },
		{ID_UPDATE_ENTITY, 				&OnlineRaceState::UpdateEntity		  	  },
		{ID_LOADPOWERUPS,				&OnlineRaceState::LoadPowerUps            },
		{ID_SYNC,                       &OnlineRaceState::SendSyncPacket          },
		{ID_SYNCENTITY,                 &OnlineRaceState::ProcessSyncPacket       },
		{ID_LOAD_MAP, 					&OnlineRaceState::LoadLevel		  	  	  },
		{ID_DISCONNECTION,				&OnlineRaceState::DisconnectClient        },
		{ID_GETPOWERUP,                 &OnlineRaceState::ProcessPowerUps         },
		{	   0	  ,		0		}
};

OnlineRaceState::OnlineRaceState(RakNet::RakPeerInterface*& c_peerInterface, RakNet::NatPunchthroughClient*& c_clientPunch, int c_roomJoined, 
								 RakNet::SystemAddress c_serverAddress, GenericData *& l_gameData_ptr, int l_selected_PowerUp)
{
	std::cout<<"Creando OnlineRaceState"<<std::endl;
	m_gameData_ptr = l_gameData_ptr;

	m_EntityManager_ptr = m_gameData_ptr->g_entityManager_ptr;
	m_InputManager_ptr  = m_gameData_ptr->g_inputManager_ptr;
	m_RenderManager_ptr = m_gameData_ptr->g_renderManager_ptr;
	m_PhysicsManager_ptr= m_gameData_ptr->g_physicsManager_ptr;
	m_CameraManager_ptr = m_gameData_ptr->g_cameraManager_ptr;
	m_MapManager_ptr    = m_gameData_ptr->g_mapManager_ptr;
	m_guiManager_ptr    = m_gameData_ptr->g_guiManager_ptr;

	m_eventManager_ptr    = m_gameData_ptr->g_eventManager_ptr;
	m_powerUpManager_ptr  = m_gameData_ptr->g_powerUpManager_ptr;
	m_positionManager_ptr = m_gameData_ptr->g_positionManager_ptr;
	m_checkpointManager_ptr = m_gameData_ptr->g_checkPointManager_ptr;

	m_soundManager_ptr    = m_gameData_ptr->g_soundManager_ptr;
	

	m_punchClient_ptr = c_clientPunch;
	m_peerInterface_ptr = c_peerInterface;
	m_roomJoined_i = c_roomJoined;
	m_isinLobby = true;
	m_serverAddress = c_serverAddress;

	std::cout<<"Creado"<<std::endl;
	m_clock = new Clock();
	m_clock->Init();

	actualCountDown = 0;
	lastCountDown = 0;
	powerUpReady = false;

	m_playerPU1 = (TypePU) (l_selected_PowerUp/10);
	m_playerPU2 = (TypePU) (l_selected_PowerUp%10);
	GetPowerUpString(&m_playerPU1, &m_playerPU1_str);
	GetPowerUpString(&m_playerPU2, &m_playerPU2_str);
	m_playerPU1_str = "PU1: " + m_playerPU1_str;
	m_playerPU2_str = "PU2: " + m_playerPU2_str;
	m_send_n = 1;


	m_PlayableCharacters_vec.reserve(4);

}

OnlineRaceState::~OnlineRaceState()
{
    m_peerInterface_ptr->Shutdown(1);
	RakNet::RakPeerInterface::DestroyInstance(m_peerInterface_ptr);
    std::cout<<"[SUCCESS] - Client destroyed correctly"<<std::endl;
}

void OnlineRaceState::Init()
{
	m_MainCharacter_ptr = nullptr;
	m_inputsToSend.reserve(10);
	
	for(size_t i = 0; i<10; i++)
	{
		m_inputsToSend.emplace_back(false);
	}
	
	m_oldInputs.reserve(10);
	for(size_t i = 0; i<10; i++)
	{
		m_oldInputs.emplace_back(false);
	}

	JoinRoomInformation roomInfo;
    roomInfo.typeId = (RakNet::MessageID)ID_CLIENTREADY;
    roomInfo.roomID = m_roomJoined_i;	
	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&roomInfo),sizeof(roomInfo),HIGH_PRIORITY,RELIABLE_ORDERED,0,m_serverAddress,false);
	
	isPlay = m_positionManager_ptr->IsPlay();

	m_CameraManager_ptr->InitCameraData(1);
}

void OnlineRaceState::Update(float &delta)
{
	ProccessPackets();
	if(powerUpReady){
		if(*isPlay)
		{
			if(delta >= m_loopTime)
			{

				CompareInputVectors();
				m_eventManager_ptr->update();
				m_PhysicsManager_ptr->Update(delta, m_loopTime);
				m_powerUpManager_ptr->Update();
				m_positionManager_ptr->OnlineUpdate();
				m_CameraManager_ptr->UpdateCameraPosition(delta);

				UpdateGUI();

				delta = 0;

			}
			
			InterpolateDrones(delta);
		}
		else
		{	
			if(isCountdown)
				Countdown(delta);
			else
			{
				EndOfTheRace(delta);
			}
		}
	}
}

void OnlineRaceState::Countdown(float& delta)
{	
	delta = 0;
	m_PhysicsManager_ptr->Update(delta, m_loopTime);
	m_positionManager_ptr->Update();
	m_CameraManager_ptr->UpdateCameraPosition(delta);
	UpdateGUI();
	InterpolateDrones(delta);

	actualCountDown = (int)m_clock->GetElapsedTime();

	if((actualCountDown - lastCountDown) == 1)
	{
		m_soundManager_ptr->SetEventVariableValue(m_MainCharacter_ptr, "Countdown", "CountDown", actualCountDown);
		m_soundManager_ptr->Play(m_MainCharacter_ptr, "Countdown");
		lastCountDown = actualCountDown;
	}

	switch (actualCountDown)
	{
		case 1:

			std::cout << "3...\n";
			break;
		case 2:
			std::cout << "2...\n";
			break;
		case 3:
			std::cout << "1...\n";
			break;
		case 4:
			std::cout << "GO!!!!!!!\n";
			*isPlay = true;
			isCountdown = false;
			m_clock->Pause();
			m_soundManager_ptr->PlayAllSounds();
			break;
	
		default:
			break;
	}	
}

void OnlineRaceState::EndOfTheRace(float& delta)
{
	if(m_clock->IsPaused())
	{
		m_clock->Init();
	}

	if(delta >= m_loopTime)
	{
		m_eventManager_ptr->update();
		m_PhysicsManager_ptr->Update(delta, m_loopTime);
		m_powerUpManager_ptr->Update();
		m_positionManager_ptr->OnlineUpdate();

		m_CameraManager_ptr->UpdateCameraPosition(delta);
		//Update the listener position 
		m_soundManager_ptr->Update(m_CameraManager_ptr->GetActiveCameraPosition());

		UpdateGUI();
		delta = 0; 
	}
	
	InterpolateDrones(delta);

	if(m_clock->GetElapsedTime() > 5.0f)
	{
		//IR AL MENU PRINCIPAL
		ChangeState();
	}

}

void OnlineRaceState::ChangeState()
{
	std::cout << "He acabado la carrera" << std::endl;
	CloseState();
	//SetVariablesDisconnection(m_MainCharacter_ptr->GetId());
	//CompareInputVectors();

	m_positionManager_ptr->EndGame();
	m_RenderManager_ptr->Reset();

	m_EntityManager_ptr->ClearList();
	m_InputManager_ptr->ClearList();
	m_soundManager_ptr->ClearList();
	m_powerUpManager_ptr->ClearList();
	m_eventManager_ptr->ClearList();
	m_PhysicsManager_ptr->ClearList();
	m_checkpointManager_ptr->ClearList();
	m_CameraManager_ptr->ClearList();
	m_guiManager_ptr->ClearList();
	m_positionManager_ptr->ClearList();

	m_MainCharacter_ptr = nullptr;
	delete m_clock;
	m_clock = nullptr;

	StatesMachine::getInstance()->AddState(std::make_unique<MainMenuState>(m_gameData_ptr));
}

void OnlineRaceState::UpdateGUI()
{
	//[WARNING] UNUSED
	//TransformComponent* nitroComp = ((TransformComponent*)m_EntityManager_ptr->GetComponent(m_MainCharacter_ptr->GetId(), "Physics"));
	//float nitroValue = nitroComp->GetPhysicsData()->chargeNITRO;
}

void OnlineRaceState::InterpolateDrones(float &delta)
{
	//Interpolate Drone's Positions in PhysicsManager
	m_PhysicsManager_ptr->InterpolatePositions(delta,m_loopTime);

	//Update Node's Positions
	for(unsigned int i=0; i<m_PhysicsManager_ptr->GetPhysicsComponentList().size(); i++){

		Entity* entity = m_EntityManager_ptr->GetEntity(m_PhysicsManager_ptr->GetPhysicsComponentList()[i]->GetEntityId());

		    if(m_PhysicsManager_ptr->GetDebugBB())
                m_RenderManager_ptr->TranslateBSphere(entity->GetId(),entity->getPos());
				
            m_RenderManager_ptr->GetRenderFacade()->TranslateSceneNode(entity->GetId(),entity->getPos());
            m_RenderManager_ptr->GetRenderFacade()->RotateSceneNode(entity->GetId(),entity->getRot());

	}

	//Interpolate Camera's Position
	m_CameraManager_ptr->InterpolateCameras(delta, m_loopTime);
}

void OnlineRaceState::Draw()
{
	if(m_MainCharacter_ptr!=nullptr)
	{
		m_RenderManager_ptr->RenderScene(m_MainCharacter_ptr);

		//Update the listener position 
		m_soundManager_ptr->Update(m_CameraManager_ptr->GetActiveCameraPosition());
	}
}

void OnlineRaceState::DrawStateGUI()
{
	ImGui::SetNextWindowSize(ImVec2(500, 300));
    ImGui::SetNextWindowPos(ImVec2(20, 100));
    ImGui::Begin("Online Control");
    if(m_MainCharacter_ptr)
	{
		for(Entity* entity : m_PlayableCharacters_vec)
		{
			std::string intro = "Physics Data of entity ->" + std::to_string(entity->GetId());
			ImGui::Text(intro.c_str());
			std::string position = "Position: X - " + std::to_string(entity->getPos().x)+ " , Y - " + std::to_string(entity->getPos().y) + " , Z - " + std::to_string(entity->getPos().z);
			ImGui::Text(position.c_str());
			std::string rotation = "Rotation: X - " + std::to_string(entity->getRot().x)+ " , Y - " + std::to_string(entity->getRot().y) + " , Z - " + std::to_string(entity->getRot().z);
			ImGui::Text(rotation.c_str());
			if(entity->GetId()==m_MainCharacter_ptr->GetId())
			{
				entity->GetComponentList();
			}
			ImGui::Text(m_playerPU1_str.c_str());
			ImGui::Text(m_playerPU2_str.c_str());
			
			TransformComponent* component = (TransformComponent*)m_EntityManager_ptr->GetComponent(entity->GetId(), "Physics");
			std::string speed = "Speed: X - " + std::to_string(component->GetPhysicsData()->v_velocity.x)+ " , Y - " + std::to_string(component->GetPhysicsData()->v_velocity.y) + " , Z - " + std::to_string(component->GetPhysicsData()->v_velocity.z);
			ImGui::Text(speed.c_str());
			std::string acceleration = "Acceleration: - " + std::to_string(component->GetPhysicsData()->v_acceleration.x)+ " , Y - " + std::to_string(component->GetPhysicsData()->v_acceleration.y) + " , Z - " + std::to_string(component->GetPhysicsData()->v_acceleration.z);
			ImGui::Text(acceleration.c_str());
		}
	}
    ImGui::End();
}

void OnlineRaceState::ProccessPackets()
{
    for(m_PacketReceiver_ptr = m_peerInterface_ptr->Receive(); m_PacketReceiver_ptr; m_peerInterface_ptr->DeallocatePacket(m_PacketReceiver_ptr), m_PacketReceiver_ptr = m_peerInterface_ptr->Receive())
    {
		OnlineGameCallbacks* l_nextCall = m_clientCallbacks;
		while(l_nextCall->m_id)
		{
			if(m_PacketReceiver_ptr->data[0] == l_nextCall->m_id)
			{
				(this->*l_nextCall->CallbackFunc)();
			}
			++l_nextCall;
		}
    }
}

void OnlineRaceState::CreateMainPlayer()
{
	PlayerData* receivedData = (PlayerData*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(PlayerData));
	if (m_PacketReceiver_ptr->length != sizeof(PlayerData))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}
	m_MainCharacter_ptr = m_EntityManager_ptr->CreateEntity("Player",receivedData->m_position);
	
	m_RenderManager_ptr->CreateComponent(m_MainCharacter_ptr, "Mesh", "drone.obj");
	m_PhysicsManager_ptr->CreateComponent(m_MainCharacter_ptr);
	m_PhysicsManager_ptr->CreateRigidBodySphere(m_MainCharacter_ptr, 3.0f, 10.0f);
	m_InputManager_ptr->CreateInputComponent(m_MainCharacter_ptr);
	m_RenderManager_ptr->AddNode(m_MainCharacter_ptr);
	m_positionManager_ptr->CreateComponent(m_MainCharacter_ptr);

	m_soundManager_ptr->CreateComponent(m_MainCharacter_ptr, "Drone", "event:/DroneSounds/Drone", 1);
	m_soundManager_ptr->CreateComponent(m_MainCharacter_ptr,"Music","event:/Music/First_Circuit", 0);
	m_soundManager_ptr->CreateComponent(m_MainCharacter_ptr, "Countdown", "event:/RaceEvents/Race_start", 0);

	m_RenderManager_ptr->AddTexture(m_MainCharacter_ptr, "assets/texture.jpg");
	//m_CameraManager_ptr->CreateComponent(m_MainCharacter_ptr, glm::vec3(0,40,20));
	m_CameraManager_ptr->SetTarget(m_MainCharacter_ptr);
	//m_guiManager_ptr->CreateGUIInterface(m_MainCharacter_ptr,130,370,350,10,0,255,255);
	//m_guiManager_ptr->CreateMiniMap(m_MainCharacter_ptr, "./assets/Minimap.jpg", 550, 300, 200, 200);
	
	m_PlayableCharacters_vec.emplace_back(m_MainCharacter_ptr);

	std::cout<<"-------------------------------------"<<std::endl;
	std::cout<<"Entity identification: "<<m_MainCharacter_ptr->GetName()<<std::endl;
	std::cout<<"Entity id: "<<m_MainCharacter_ptr->GetId()<<std::endl;
	std::cout<<"-------------------------------------"<<std::endl;

	if(m_PlayableCharacters_vec.size() == receivedData->howmanyplayers)
	{
		JoinRoomInformation bringit;
		bringit.typeId = (RakNet::MessageID)ID_CHARACTERSCREATED;
		bringit.roomID = m_roomJoined_i;
		m_peerInterface_ptr->Send(reinterpret_cast<char*>(&bringit), sizeof(bringit), HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);
	}
}

void OnlineRaceState::LoadLevel()
{
	m_MapManager_ptr->LoadLevel(0,true,true,true);
}

void OnlineRaceState::CreateExternalEnt()
{
	std::cout<<"Creating external"<<std::endl;
	PlayerData* receivedData = (PlayerData*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(PlayerData));
	if (m_PacketReceiver_ptr->length != sizeof(PlayerData))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	Entity* external = m_EntityManager_ptr->CreateEntity(std::to_string(receivedData->entityID),receivedData->m_position);
	
	m_RenderManager_ptr->CreateComponent(external, "Mesh", "drone.obj");
	m_PhysicsManager_ptr->CreateComponent(external);
	m_PhysicsManager_ptr->CreateRigidBodySphere(external, 3.0f, 10.0f);
	m_positionManager_ptr->CreateComponent(external);
	m_RenderManager_ptr->AddNode(external);
	m_RenderManager_ptr->AddTexture(external, "assets/textura_ia.png");
	m_RenderManager_ptr->ScaleSceneNode(external, 5.0f);

	m_soundManager_ptr->CreateComponent(external,external->GetName(), "event:/DroneSounds/Drone", 1);

	m_PlayableCharacters_vec.emplace_back(external);

	std::cout<<"-------------------------------------"<<std::endl;
	std::cout<<"Entity identification: "<<external->GetName()<<std::endl;
	std::cout<<"-------------------------------------"<<std::endl;

	if(m_PlayableCharacters_vec.size() == receivedData->howmanyplayers)
	{
		JoinRoomInformation bringit;
		bringit.typeId = (RakNet::MessageID)ID_CHARACTERSCREATED;
		bringit.roomID = m_roomJoined_i;
		m_peerInterface_ptr->Send(reinterpret_cast<char*>(&bringit), sizeof(bringit), HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);


	}
}

void OnlineRaceState::InputBroadcast()
{
	TransformComponent* physics = (TransformComponent*)m_EntityManager_ptr->GetComponent(m_MainCharacter_ptr->GetId(), "Physics");
	PICASSO::PhysicsVariables * physicsData = physics->GetPhysicsData();
	

	//InputsToSend is now updated with the player's inputs
	InputDataPacket packet_inputs;
	packet_inputs.typeId = (RakNet::MessageID)ID_PLAYER_INPUT;
	packet_inputs.roomID = m_roomJoined_i;
	packet_inputs.entityID =m_MainCharacter_ptr->GetId();
	packet_inputs.W = m_inputsToSend[0];
	packet_inputs.A = m_inputsToSend[1];
	packet_inputs.S = m_inputsToSend[2];
	packet_inputs.D = m_inputsToSend[3];
	packet_inputs.K = m_inputsToSend[4];
	packet_inputs.I = m_inputsToSend[5];
	packet_inputs.SPACE = m_inputsToSend[6];
	packet_inputs.J     = m_inputsToSend[7];
	packet_inputs.L     = m_inputsToSend[8];

	packet_inputs.accFWRD = physicsData->accFWRD;
	packet_inputs.accUP   = physicsData->accUP;
	packet_inputs.Position= m_MainCharacter_ptr->getPos();
	packet_inputs.Rotation= m_MainCharacter_ptr->getRot();
	packet_inputs.Velocity= physicsData->v_velocity;



	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&packet_inputs),sizeof(packet_inputs),HIGH_PRIORITY,RELIABLE_ORDERED,0,m_serverAddress,false);
	
	m_PhysicsManager_ptr->UpdateKeys(m_inputsToSend[0], m_inputsToSend[1], m_inputsToSend[2], m_inputsToSend[3], m_inputsToSend[4], m_inputsToSend[5], m_inputsToSend[6], m_MainCharacter_ptr->GetId());
	if(m_inputsToSend[7]){
		//POWER UP J
		std::cout << "TIRO UN PU " << std::endl;
		m_powerUpManager_ptr->CreatePUEvent(m_MainCharacter_ptr->GetId(),0);
	}
	if(m_inputsToSend[8]){
		//POWER UP L 
		std::cout << "TIRO UN PU " << std::endl;
		m_powerUpManager_ptr->CreatePUEvent(m_MainCharacter_ptr->GetId(),1);
	}
}

void OnlineRaceState::CompareInputVectors()
{
	if(m_MainCharacter_ptr!=nullptr)
	{
		m_InputManager_ptr->Update(m_inputsToSend);
		
		for(size_t i = 0; i<m_inputsToSend.size(); i++)
		{
			if(m_inputsToSend[i]!=m_oldInputs[i])
			{
				InputBroadcast();
				m_oldInputs = m_inputsToSend;
				return;
			}
		}
	
	}

}

void OnlineRaceState::UpdateEntity()
{
	InputDataPacket* inputstoupdate = (InputDataPacket*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(InputDataPacket));
	if (m_PacketReceiver_ptr->length != sizeof(InputDataPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	TransformComponent* physics = (TransformComponent*)m_EntityManager_ptr->GetComponent(inputstoupdate->entityID, "Physics");
	PICASSO::PhysicsVariables * physicsData = physics->GetPhysicsData();

	physicsData->accUP = inputstoupdate->accUP;
	physicsData->accFWRD = inputstoupdate->accFWRD;
	physicsData->v_velocity = inputstoupdate->Velocity;
	Entity * entity = m_EntityManager_ptr->GetEntity(inputstoupdate->entityID);
	entity->setBeforeED(inputstoupdate->Position, inputstoupdate->Rotation);
	entity->setAfterED(inputstoupdate->Position, inputstoupdate->Rotation);
	entity->setPos(inputstoupdate->Position);
	entity->setRot(inputstoupdate->Rotation);

	m_PhysicsManager_ptr->UpdateKeys(inputstoupdate->W, inputstoupdate->A, inputstoupdate->S, inputstoupdate->D, inputstoupdate->K, inputstoupdate->I, inputstoupdate->SPACE, inputstoupdate->entityID);
	if(inputstoupdate->J){
		//POWER UP J
		m_powerUpManager_ptr->CreatePUEvent(inputstoupdate->entityID,0);
	}

	if(inputstoupdate->L){
		//POWER UP L
		m_powerUpManager_ptr->CreatePUEvent(inputstoupdate->entityID,1);
	}

}

//=============================
//=========POWER UPS===========
//=============================

//When all players have been created we create the power-ups.
void OnlineRaceState::LoadPowerUps()
{
	//Power ups of externals players
	SendPURequest();

	//Power ups of main player
	std::cout << "Creo mi power up" << std::endl;
	m_powerUpManager_ptr->CreatePowerUp(m_playerPU1, m_MainCharacter_ptr);
	m_powerUpManager_ptr->CreatePowerUp(m_playerPU2, m_MainCharacter_ptr);

	std::cout<<"Created"<<std::endl;

	m_PhysicsManager_ptr->CreateOctree();
	m_PhysicsManager_ptr->CreateDronesRBList();
}

//I send my power ups to the server so that it is sent to the other clients
void OnlineRaceState::SendPURequest()
{
	SendPUPacket data;
	data.typeId = (RakNet::MessageID)ID_GETPOWERUP;
	data.roomID = m_roomJoined_i;
	data.entityID = m_MainCharacter_ptr->GetId();
	data.powerUp1  = (int)m_playerPU1;
	data.powerUp2  = (int)m_playerPU2;

	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&data), sizeof(data), HIGH_PRIORITY, RELIABLE_ORDERED,0, m_serverAddress, false);
}

//Create power ups of external players
void OnlineRaceState::ProcessPowerUps(){
	SendPUPacket* puPacket     = (SendPUPacket*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(SendPUPacket));
	if (m_PacketReceiver_ptr->length != sizeof(SendPUPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	for(Entity* character : m_PlayableCharacters_vec)
	{
		if((unsigned int)character->GetId() == puPacket->entityID){
			m_powerUpManager_ptr->CreatePowerUp((TypePU)puPacket->powerUp1, character);
			m_powerUpManager_ptr->CreatePowerUp((TypePU)puPacket->powerUp2, character);

			m_send_n += 1;

			if(m_send_n == m_PlayableCharacters_vec.size()){
				powerUpReady = true;
			}
		}
	}
}

void OnlineRaceState::GetPowerUpString(TypePU* type, std::string* name)
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


//=============================
//======SYNCRONIZATION=========
//=============================

void OnlineRaceState::SendSyncPacket()
{
	std::cout << "Envia paquete de sync" << m_MainCharacter_ptr->GetId() << std::endl;
	EntityUpdatePacket data;
	data.typeId = (RakNet::MessageID)ID_SYNCENTITY;
	data.roomID = m_roomJoined_i;
	data.entityID = m_MainCharacter_ptr->GetId();

	TransformComponent* physics = (TransformComponent*)m_EntityManager_ptr->GetComponent(m_MainCharacter_ptr->GetId(), "Physics");
	PICASSO::PhysicsVariables * physicsData = physics->GetPhysicsData();

	data.accUP = physicsData->accUP;
	data.accFWRD = physicsData->accFWRD;
	data.Velocity = physicsData->v_velocity;

	data.Position = m_MainCharacter_ptr->getPos();
	data.Rotation = m_MainCharacter_ptr->getRot();

	PositionComponent * l_positionComponent_ptr = (PositionComponent*)m_EntityManager_ptr->GetComponent(m_MainCharacter_ptr->GetId(), "Position");
	data.checkpoint      = l_positionComponent_ptr->GetCheckpoint();
	data.lap 			 = l_positionComponent_ptr->GetLap();
	data.next_checkpoint = l_positionComponent_ptr->GetNextCheckpoint();

	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&data), sizeof(data), HIGH_PRIORITY, RELIABLE_ORDERED,0, m_serverAddress, false);
}

void OnlineRaceState::ProcessSyncPacket()
{
	//std::cout << "ESTOY SINCRONIZANDO" << std::endl;
	EntityUpdatePacket* updatePacket     = (EntityUpdatePacket*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(EntityUpdatePacket));
	if (m_PacketReceiver_ptr->length != sizeof(EntityUpdatePacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}
	std::cout << "Proceso paquete de: " << updatePacket->entityID << std::endl;
	TransformComponent* physics = (TransformComponent*)m_EntityManager_ptr->GetComponent(updatePacket->entityID, "Physics");
	PICASSO::PhysicsVariables * physicsData = physics->GetPhysicsData();
	PositionComponent * l_positionComponent_ptr = (PositionComponent*)m_EntityManager_ptr->GetComponent(updatePacket->entityID, "Position");

	physicsData->accUP = updatePacket->accUP;
	physicsData->accFWRD = updatePacket->accFWRD;
	physicsData->v_velocity = updatePacket->Velocity;

	Entity * entity = m_EntityManager_ptr->GetEntity(updatePacket->entityID);
	//entity->setBeforeED(updatePacket->Position, updatePacket->Rotation);
	entity->setAfterED(updatePacket->Position, updatePacket->Rotation);
	//entity->setPos(updatePacket->Position);
	//entity->setRot(updatePacket->Rotation);

	l_positionComponent_ptr->setOnline(updatePacket->checkpoint, updatePacket->next_checkpoint, updatePacket->lap);
}

//=============================
//======DISCONNECTION==========
//=============================

void OnlineRaceState::DisconnectClient()
{

	DisconnectionPacket* l_disconnectPacket_ptr = (DisconnectionPacket*)m_PacketReceiver_ptr->data;
	assert(m_PacketReceiver_ptr->length == sizeof(DisconnectionPacket));
	if (m_PacketReceiver_ptr->length != sizeof(DisconnectionPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	SetVariablesDisconnection(l_disconnectPacket_ptr->entityID);
}

void OnlineRaceState::CloseState()
{
	//m_peerInterface_ptr->DetachPlugin(m_clientPunch_ptr);
	DisconnectionPacket disconnectionPacket;
	disconnectionPacket.typeId = (RakNet::MessageID)ID_DISCONNECTION;
	disconnectionPacket.roomID = m_roomJoined_i;
	disconnectionPacket.entityID = m_MainCharacter_ptr->GetId();

	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&disconnectionPacket), sizeof(disconnectionPacket), HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);
	std::cout<<"[SUCCESS] - Disconnection for server set"<<std::endl;
	
	//m_peerInterface_ptr->CloseConnection(m_serverAddress, false);
    m_peerInterface_ptr->Shutdown(100,0);
	RakNet::RakPeerInterface::DestroyInstance(m_peerInterface_ptr);

    std::cout<<"[SUCCESS] - Client shutted down correclty"<<std::endl;
}

void OnlineRaceState::SetVariablesDisconnection(unsigned int l_entityId_n)
{
	Entity * entity = m_EntityManager_ptr->GetEntity(l_entityId_n);

	IComponent* l_physicsComponent_ptr =  m_EntityManager_ptr->GetComponent(l_entityId_n, "Physics");
	TransformComponent* l_transformComponent_ptr = ((TransformComponent*) l_physicsComponent_ptr);
    PICASSO::PhysicsVariables* l_data_ptr = l_transformComponent_ptr->GetPhysicsData();

	//Set variables
	l_data_ptr->v_velocity = glm::vec3(0,0,0);
	l_data_ptr->v_acceleration = glm::vec3(0,0,0);
	m_PhysicsManager_ptr->UpdateKeys(false,false,false,false, false, false, false, l_entityId_n);

	entity->setPos(glm::vec3(0,0,0));
	entity->setBeforeED(glm::vec3(0,0,0),glm::vec3(0,0,0));
	entity->setAfterED(glm::vec3(0,0,0),glm::vec3(0,0,0));
}