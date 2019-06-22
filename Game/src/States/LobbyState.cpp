#include <LobbyState.hpp>

#include <CameraManager.hpp>
#include <EntityManager.h>
#include <InputManager.hpp>
#include <RenderManager.h>
#include <StatesMachine.hpp>
#include <OnlineRaceState.hpp>
#include <OrionHTTPRequester.hpp>
#include <Entity.h>
#include <Clock.hpp>
#include <GenericData.hpp>

#include <BitStream.h>
#include <GetTime.h>

#include <rapidxml.hpp>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include <OrionEnums.hpp>
#include <OrionStructs.hpp>
#include <OrionHTTPStructs.hpp>



struct LobbyCallBack
{
    RakNet::MessageID m_id;
    void (LobbyState::*CallbackFunc)();
};

LobbyCallBack m_lobbycallbacks [] = 
{
		{ID_CONNECTION_REQUEST_ACCEPTED,  &LobbyState::RegisterServer},
		{ID_STARTGAME, 	                  &LobbyState::ChangeState},
        {ID_ROOMCREATED,                  &LobbyState::RegisterAndJoinRoom},
        {ID_UNCONNECTED_PONG,             &LobbyState::CalculatePing},
		{		     0		               ,		  0		         }
};

LobbyState::LobbyState(GenericData *&l_gameData)
{
    m_gameData_ptr = l_gameData;

    m_EntityManager_ptr = m_gameData_ptr->g_entityManager_ptr;
    m_renderManager_ptr = m_gameData_ptr->g_renderManager_ptr;
    m_inputManager_ptr  = m_gameData_ptr->g_inputManager_ptr;
    m_CameraManager_ptr = m_gameData_ptr->g_cameraManager_ptr;
    
    m_isPlayerReady_b = false;
    m_roomJoined_i = -1;
    m_HTTPRequester_ptr = new OrionHTTPRequester("zenongames.herokuapp.com");
}

void LobbyState::Init()
{
    m_mainEntity_ptr = m_EntityManager_ptr->CreateEntity("DemoCube",glm::vec3(0,0,0));
    m_renderManager_ptr->CreateComponent(m_mainEntity_ptr, "Cube", "");
    m_renderManager_ptr->AddNode(m_mainEntity_ptr); 

    m_clientPunch_ptr = RakNet::NatPunchthroughClient::GetInstance();
    m_peerInterface_ptr = RakNet::RakPeerInterface::GetInstance();
	m_peerInterface_ptr->AttachPlugin(m_clientPunch_ptr);
	m_peerInterface_ptr->Startup(1,&m_socketDescriptor_sock,1);

    m_actualState_en = LobbyStates::LOGIN_STATE;

    m_requestsent = true;

    m_clock = new Clock();
    m_pingClock = new Clock();
    m_pingClock->Init();
    m_clock->Init();
}

void LobbyState::Update(float &delta)
{
    ProccessPackets();
    if(m_actualState_en == LobbyStates::SELECT_ROOM_STATE)
    {
        if(m_pingClock->GetElapsedTime() > 3.0)
        {
             SendPing();
             m_pingClock->Restart();
        }
    }
}

void LobbyState::Draw()
{
    if(m_mainEntity_ptr!= nullptr)
    {
        m_renderManager_ptr->RenderScene(m_mainEntity_ptr);
    }
}

LobbyState::~LobbyState()
{

}

void LobbyState::DrawStateGUI()
{   
    std::future<std::vector<RoomData>> result;
    
    if(!m_requestsent && m_clock->GetElapsedTime() > 10.0)
    {
        //std::cout<<"Making HTTP request"<<std::endl;
        result = m_HTTPRequester_ptr->GetAllRooms();
        m_clock->Restart();
        m_requestsent = true;
    }
    if(m_actualState_en == LobbyStates::IN_ROOM_STATE)
    {
        DrawPowerUpSelector();
    }
    else{
    ImGui::SetNextWindowSize(ImVec2(250, 300));
    ImGui::SetNextWindowPos(ImVec2(20, 100));
    ImGui::Begin("Welcome to the Inspire Multiplayer");

    if(m_actualState_en == LobbyStates::SELECT_ROOM_STATE)
    {
        if(m_requestsent)
        {
            result.wait();
            m_roomsAvailable = result.get();
            m_requestsent = false;
        }

        for(auto& room : m_roomsAvailable)
        {
            if(ImGui::Button(room.room_name.c_str(), ImVec2(95,40)))
            {
                JoinRoom(room.roomID);
                m_requestsent = true;
            }
        }
    }
    else
    {
        static char username[128];
        ImGui::Text("Username:");
        ImGui::InputText(" ", username, sizeof(username));
        
        static char password[128];
        ImGui::Text("Password:");
        ImGui::InputText("  ", password, sizeof(password));

        if(ImGui::Button("Login", ImVec2(95,40)))
        {
            std::string username_str = username;
            std::string password_str = password;

            Login(username_str, password_str);
        }
    }

        ImGui::End();
    }
    if(m_actualState_en == LobbyStates::SELECT_ROOM_STATE)
    {
        ImGui::SetNextWindowSize(ImVec2(200, 200));
        ImGui::SetNextWindowPos(ImVec2(500, 100));
        ImGui::Begin("Create a custom room");
        static char roomname[128];
        ImGui::Text("Room name:");
        ImGui::InputText(" ", roomname, sizeof(roomname));
        if(ImGui::Button("Create", ImVec2(95,40)))
        {
            m_customRoomName_str = roomname;
            JoinRoomInformation l_packet;
            if(m_requestsent)
            {
                result.wait();
            }
            l_packet.typeId = (RakNet::MessageID)ID_CREATEROOM;
            m_peerInterface_ptr->Send(reinterpret_cast<char*>(&l_packet),sizeof(l_packet),HIGH_PRIORITY,RELIABLE_ORDERED,0,m_serverAddress,false);
        }
        ImGui::End();
    }
}

void LobbyState::SendPing()
{
    m_peerInterface_ptr->Ping(m_serverIP, m_serverPort, false);
}

void LobbyState::ProccessPackets()
{
    for(m_packetReceiver_ptr = m_peerInterface_ptr->Receive(); m_packetReceiver_ptr; m_peerInterface_ptr->DeallocatePacket(m_packetReceiver_ptr), m_packetReceiver_ptr = m_peerInterface_ptr->Receive())
    {
        LobbyCallBack* l_nextCall = m_lobbycallbacks;
		while(l_nextCall->m_id)
		{
			if(m_packetReceiver_ptr->data[0] == l_nextCall->m_id)
			{
				(this->*l_nextCall->CallbackFunc)();
			}
			++l_nextCall;
		}
    }
}

void LobbyState::CalculatePing()
{
    RakNet::TimeMS l_time;
    RakNet::BitStream l_bitStreamIN(m_packetReceiver_ptr->data, m_packetReceiver_ptr->length, false);
    l_bitStreamIN.IgnoreBytes(1);

    l_bitStreamIN.Read(l_time);

}

void LobbyState::ReadNetworkData()
{
    using namespace rapidxml;

    xml_document<> xml;
    xml_node<>* root_node;

    std::ifstream file("assets/netConfig.xml");
    std::string buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    xml.parse<0>(&buffer[0]);

	root_node = xml.first_node("config");

    m_serverIP = root_node->first_attribute("ip")->value();
    m_serverPort = (atof(root_node->first_attribute("port")->value()));
}

void LobbyState::RegisterServer()
{
    ReadNetworkData();
    std::cout<<"Connection accepted from the server"<<std::endl;
    m_serverAddress = m_packetReceiver_ptr->systemAddress;
    m_actualState_en = LobbyStates::SELECT_ROOM_STATE;
    m_requestsent = false;
}

void LobbyState::JoinRoom(unsigned int c_room_id)
{
    std::cout<<"I am going to join the room"<<std::endl;
	JoinRoomInformation roomInfo;

	roomInfo.typeId = (RakNet::MessageID)ID_JOIN_ROOM;
	roomInfo.roomID = c_room_id;

	m_roomJoined_i = c_room_id;

	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&roomInfo),sizeof(roomInfo),HIGH_PRIORITY,RELIABLE_ORDERED,0,m_serverAddress,false);

    m_actualState_en = LobbyStates::IN_ROOM_STATE;
}

void LobbyState::SendReadyStatus()
{
    JoinRoomInformation roomInfo;
    roomInfo.typeId = (RakNet::MessageID)ID_CLIENTREADY;
    roomInfo.roomID = m_roomJoined_i;	
	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&roomInfo),sizeof(roomInfo),HIGH_PRIORITY,RELIABLE_ORDERED,0,m_serverAddress,false);
}

void LobbyState::ConnectToServer()
{
    ReadNetworkData();
    std::cout<<"I'm going to try to connect to the server"<<std::endl;
    m_peerInterface_ptr->Connect(m_serverIP,m_serverPort,0,0);
    std::cout<<"Connecting..."<<std::endl;
}

void LobbyState::ChangeState()
{
    std::cout<<"Borrando renderManager"<<std::endl;
    m_renderManager_ptr->Reset();
    std::cout<<"Borrando entityManager"<<std::endl;
    m_EntityManager_ptr->ClearList();
    std::cout<< "Borrando inputsManager"<<std::endl;
    m_inputManager_ptr->ClearList();
    std::cout<<"Borrando cameraManager"<<std::endl;
    m_CameraManager_ptr->ClearList();

    delete m_HTTPRequester_ptr;
    m_HTTPRequester_ptr = nullptr;
    delete m_clock;
    m_clock = nullptr;

    StatesMachine::getInstance()->AddState(std::make_unique<OnlineRaceState>(m_peerInterface_ptr, m_clientPunch_ptr, m_roomJoined_i, m_serverAddress, 
                                                                             m_gameData_ptr, m_selected_PowerUp));
    m_mainEntity_ptr = nullptr;
}

void LobbyState::Login(std::string c_username_str, std::string c_password_str)
{
    //if(m_HTTPRequester_ptr->Login(c_username_str, c_password_str))
    //{
    //    ConnectToServer();
    //}
    //else
    //    std::cout<<"Error"<<std::endl; 
    ConnectToServer();
}

void LobbyState::RegisterAndJoinRoom()
{
    std::cout<<"My room has been registered correctly"<<std::endl;

    JoinRoomInformation* receivedData = (JoinRoomInformation*)m_packetReceiver_ptr->data;
	assert(m_packetReceiver_ptr->length == sizeof(JoinRoomInformation));
	if (m_packetReceiver_ptr->length != sizeof(JoinRoomInformation))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

    std::future<bool> queryResult = m_HTTPRequester_ptr->RegisterRoom(receivedData->roomID, m_customRoomName_str, "");
    queryResult.wait();
    if(queryResult.get())
    {
        JoinRoom(receivedData->roomID);
    }
}

void LobbyState::CloseState()
{
    m_peerInterface_ptr->DetachPlugin(m_clientPunch_ptr);
    m_peerInterface_ptr->Shutdown(100,0);
	RakNet::RakPeerInterface::DestroyInstance(m_peerInterface_ptr);

    delete m_HTTPRequester_ptr;
    delete m_clock;
    delete m_pingClock;
    delete m_clientPunch_ptr;
}

//============POWER UP SELECTOR============
void LobbyState::DrawPowerUpSelector()
{
    ImGui::SetNextWindowSize(ImVec2(120, 180));
    ImGui::SetNextWindowPos(ImVec2(20, 100));
    ImGui::Begin("PowerUp LEFT");

    
    if(ImGui::Checkbox("Shield", &m_powerUpsLEFT[0])) { SelectPowerUp(&m_powerUpsLEFT[0], &m_selected_PowerUpLEFT, 0, 'L');}

    if(ImGui::Checkbox("Magnet", &m_powerUpsLEFT[1])) { SelectPowerUp(&m_powerUpsLEFT[1], &m_selected_PowerUpLEFT, 1, 'L');}

    if(ImGui::Checkbox("Thief", &m_powerUpsLEFT[2])) { SelectPowerUp(&m_powerUpsLEFT[2], &m_selected_PowerUpLEFT, 2, 'L');}
    
    if(ImGui::Checkbox("Cyberattack", &m_powerUpsLEFT[3])) { SelectPowerUp(&m_powerUpsLEFT[3], &m_selected_PowerUpLEFT, 3, 'L');}

    if(ImGui::Checkbox("Freezing", &m_powerUpsLEFT[4])) { SelectPowerUp(&m_powerUpsLEFT[4], &m_selected_PowerUpLEFT, 4, 'L');}

    if(ImGui::Checkbox("Reflector", &m_powerUpsLEFT[5])) { SelectPowerUp(&m_powerUpsLEFT[5], &m_selected_PowerUpLEFT, 5, 'L');}

    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(120, 180));
    ImGui::SetNextWindowPos(ImVec2(220, 100));
    ImGui::Begin("PowerUp RIGHT");

    
    if(ImGui::Checkbox("Shield", &m_powerUpsRIGHT[0])) { SelectPowerUp(&m_powerUpsRIGHT[0], &m_selected_PowerUpRIGHT, 0, 'R');}

    if(ImGui::Checkbox("Magnet", &m_powerUpsRIGHT[1])) { SelectPowerUp(&m_powerUpsRIGHT[1], &m_selected_PowerUpRIGHT, 1, 'R');}

    if(ImGui::Checkbox("Thief", &m_powerUpsRIGHT[2])) { SelectPowerUp(&m_powerUpsRIGHT[2], &m_selected_PowerUpRIGHT, 2, 'R');}
    
    if(ImGui::Checkbox("Cyberattack", &m_powerUpsRIGHT[3])) { SelectPowerUp(&m_powerUpsRIGHT[3], &m_selected_PowerUpRIGHT, 3, 'R');}

    if(ImGui::Checkbox("Freezing", &m_powerUpsRIGHT[4])) { SelectPowerUp(&m_powerUpsRIGHT[4], &m_selected_PowerUpRIGHT, 4, 'R');}

    if(ImGui::Checkbox("Reflector", &m_powerUpsRIGHT[5])) { SelectPowerUp(&m_powerUpsRIGHT[5], &m_selected_PowerUpRIGHT, 5, 'R');}

    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(70, 60));
    ImGui::SetNextWindowPos(ImVec2(145, 160));
    ImGui::Begin("Play");
    if(ImGui::Button("Play", ImVec2(50,20)))
    {
        if(SavePlayerPU())
        {
             SendReadyStatus();
        }
    }
    ImGui::End();
}

void LobbyState::SelectPowerUp(bool* l_PU, int* PowerUp, int type, char side)
{  
    for(int i = 0; i < (int)sizeof(m_powerUpsLEFT); i++)
    {
        if(i != type)
        {
            if(side == 'L')
                m_powerUpsLEFT[i] = false;

            else if(side == 'R')
                m_powerUpsRIGHT[i] = false;

        }
    }

    if(*l_PU)
        *PowerUp = (int)type;
    else
        *PowerUp = -1;
    
    
    if(m_selected_PowerUpLEFT == m_selected_PowerUpRIGHT)
    {
        *PowerUp = -1;
        *l_PU = false;
    }

}

bool LobbyState::SavePlayerPU()
{
    if(m_selected_PowerUpLEFT != -1 && m_selected_PowerUpRIGHT != -1)
    {
        m_selected_PowerUp = m_selected_PowerUpLEFT * 10 + m_selected_PowerUpRIGHT;
        return true;
    }
    
    return false;
}
