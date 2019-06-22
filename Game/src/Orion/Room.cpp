#include <Room.hpp>
#include <BitStream.h>

#include <OrionEnums.hpp>
#include <OrionStructs.hpp>

#include <EntityManager.h>
#include <PhysicsManager.h>
#include <CheckpointManager.hpp>
#include <EventManager.h>
#include <MapManager.hpp>
#include <PositionManager.hpp>
#include <GenericData.hpp>
#include <PowerUpManager.hpp>
#include <mutex>
#include <RakSleep.h>
#include <Clock.hpp>

#include <iostream>

std::mutex g_roomMutex_mtx;

Room::Room(unsigned int c_room_id):m_RoomID_i(c_room_id)
{
    m_entityManager = new EntityManager();
	m_eventManager  = new EventManager();

	m_checkPointManager = new CheckpointManager();

	m_positionManager = new PositionManager();

	m_physicsManager = new PhysicsManager();
	
	m_mapManager = new MapManager();

	m_powerUpManager = new PowerUpManager();

	m_genericData = new GenericData(m_entityManager, m_eventManager, m_physicsManager, m_mapManager, m_positionManager, m_checkPointManager, m_powerUpManager);

	m_checkPointManager->Init(m_genericData);
	m_physicsManager->Init(m_genericData);
	m_positionManager->Init(m_genericData);
	m_mapManager->Init(m_genericData);
	m_powerUpManager->Init(m_genericData);

	m_clients.reserve(4);
	
	m_physicsManager->SetOnlineMatch(true);

	std::cout<<"Loading Map..."<<std::endl;
	m_mapManager->LoadLevel(0,true,false, true);
	std::cout<<"Map loaded, now creating Octree"<<std::endl;
	m_physicsManager->CreateOctree();
	std::cout<<"Map Loaded!"<<std::endl;

	m_numclients_i = 0;

    m_roomState_en = RoomStates::LOBBY_STATE;
    std::cout<<"--------------------- ROOM CREATED ---------------------"<<std::endl;
    std::cout<<"The room ID in the  is: "<< m_RoomID_i <<std::endl;
    std::cout<<"The room state is: "<< m_roomState_en <<std::endl;
    std::cout<<"--------------------------------------------------------"<<std::endl;

	m_clock = new Clock();
	m_clock->Init();

	m_positions_v.emplace_back(glm::vec3(-4072.70185407, 2240.93658269, 300));
	m_positions_v.emplace_back(-4172.70185407, 2440.93658269, 300);
	m_positions_v.emplace_back(-4272.70185407, 2640.93658269, 300);
	m_positions_v.emplace_back(-4372.70185407, 2740.93658269, 300);
}

void Room::Init(RakNet::RakPeerInterface*& c_peerInterface_ptr)
{
    m_peerInterface_ptr = c_peerInterface_ptr;
}

Room::~Room()
{
    delete m_entityManager;
    delete m_physicsManager;
    delete m_checkPointManager;
    delete m_eventManager;
    delete m_mapManager;
	delete m_positionManager;
	delete m_genericData;
	delete m_powerUpManager;

    std::cout<<"Room erased correctly"<<std::endl;
}

void Room::RegisterClient(RakNet::Packet* c_packet_ptr)
{	
	if(m_numclients_i<4)
	{
		//SendPreviousClients();
		std::cout<<"Registering client..."<<std::endl;
		ClientID client_identification;
		client_identification.sys_address = c_packet_ptr->systemAddress;

		Entity* clientPlayer = m_entityManager->CreateEntity(std::to_string(m_numclients_i), m_positions_v[(int)m_clients.size()]);
		
		client_identification.serverID = clientPlayer->GetId();
		client_identification.ready = false;
		m_clients.emplace_back(client_identification);
		
		std::cout<<"[SUCCESS] - Client registered"<<std::endl;

		std::cout<<"[SUCCESS] - Game entity registered"<<std::endl;

		std::cout<<"-------------------------------------"<<std::endl;
		std::cout<<"Entity identification: "<<clientPlayer->GetName()<<std::endl;
		std::cout<<"Entity id: "			<<clientPlayer->GetId()  <<std::endl;
		std::cout<<"-------------------------------------"<<std::endl;

		m_numclients_i++;
	}
}

void Room::IsEveryOneReady()
{
    bool start = true;
    if(m_clients.size() > 0)
    {
        for(ClientID& client: m_clients)
        {
			if(!client.ready)
            	start = false;
        }

        if(start)
        {
			g_roomMutex_mtx.lock();
            BroadcastInitGame();
			g_roomMutex_mtx.unlock();
        }
    }
}

void Room::IsEveryOneInGame()
{
	bool start = true;
	if(m_clients.size()>0)
	{
		for(ClientID& client: m_clients)
		{
			start = client.ready;
		}

		if(start)
		{
			g_roomMutex_mtx.lock();
			StartGame();
			g_roomMutex_mtx.unlock();
		}
	}
}

void Room::Update(float delta)
{
    if(m_roomState_en == RoomStates::LOBBY_STATE)
    {
        IsEveryOneReady();
    }
	else if(m_roomState_en == RoomStates::WAITCONNECTIONS_STATE)
	{
		IsEveryOneInGame();
	}else{
		if(m_clock->GetElapsedTime() >= 30.0f){
			SendSyncPacket();
			m_clock->Restart();
		}
	}
}

void Room::SendSyncPacket()
{
	JoinRoomInformation empty;
	empty.typeId = (RakNet::MessageID)ID_SYNC;

	for(ClientID& client : m_clients)
	{
		m_peerInterface_ptr->Send(reinterpret_cast<char*>(&empty), sizeof(empty), HIGH_PRIORITY, RELIABLE_ORDERED,0, client.sys_address, false);
	}
}

void Room::BroadcastSync(RakNet::Packet* c_packet_ptr)
{
	EntityUpdatePacket* receivedData = (EntityUpdatePacket*)c_packet_ptr->data;
	assert(c_packet_ptr->length == sizeof(EntityUpdatePacket));
	if (c_packet_ptr->length != sizeof(EntityUpdatePacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	for(ClientID client: m_clients)
	{
		if(client.serverID != receivedData->entityID)
			m_peerInterface_ptr->Send(reinterpret_cast<char*>(receivedData), sizeof(*receivedData), HIGH_PRIORITY, RELIABLE_ORDERED,0, client.sys_address, false);
	}
}

void Room::SetClientReady(RakNet::Packet* c_packet_ptr)
{
	std::cout<<"Registering readyness"<<std::endl;
    for(ClientID& client : m_clients)
    {
        if(client.sys_address == c_packet_ptr->systemAddress)
        {
            std::cout<<"[ROOM "<< m_RoomID_i <<"] "<<"Client " << client.serverID << "is ready"<<std::endl;
            client.ready = true;
        }
    }

}

void Room::StartGame()
{
    for(ClientID& client: m_clients)
    {
        SendLoadMapOrder(client.sys_address);

		/*
		*	Pick up a client
		*	Pick up all vector's clients 
		*	
		*	Check if they are the same client
		*	IF Client is the same -> Send packet with CREATE_PLAYER
		*	ELSE				  -> Send packet with CREATE_EXTERNAL_ENTITY
		*/
        for(ClientID& client2: m_clients)
        {
            if(client2.serverID != client.serverID)
            {
                std::cout<<"Crea external"<<std::endl;
                PlayerData ext_client_data;
		        Entity* ext_client_entity = m_entityManager->GetEntity(client2.serverID);
		        ext_client_data.typeId = (RakNet::MessageID)ID_CREATE_EXTERMAL_ENTITY;
		        ext_client_data.entityID = client2.serverID;
		        ext_client_data.m_position = ext_client_entity->getPos();
				ext_client_data.howmanyplayers = m_clients.size();

		        m_peerInterface_ptr->Send(reinterpret_cast<char*>(&ext_client_data), sizeof(ext_client_data), HIGH_PRIORITY, RELIABLE_ORDERED, 0, client.sys_address,false);	
		        std::cout<<"[SENT] External entity with id - "<<client2.serverID<<std::endl;
            }
            else
            {
                std::cout<<"Crea Player"<<std::endl;
                PlayerData ext_client_data;
		        Entity* ext_client_entity = m_entityManager->GetEntity(client2.serverID);
		        ext_client_data.typeId = (RakNet::MessageID)ID_CREATE_PLAYER;
		        ext_client_data.entityID = client2.serverID;
		        ext_client_data.m_position = ext_client_entity->getPos();
				ext_client_data.howmanyplayers = m_clients.size();

		        m_peerInterface_ptr->Send(reinterpret_cast<char*>(&ext_client_data), sizeof(ext_client_data), HIGH_PRIORITY, RELIABLE_ORDERED, 0, client.sys_address,false);	
		        std::cout<<"[SENT] Player entity with id - "<<client2.serverID<<std::endl;
            }
        }
    }

    m_roomState_en = RoomStates::GAME_STATE;
}


void Room::SendPowerupOrder(RakNet::Packet* c_packet_ptr)
{
	JoinRoomInformation powerups;
	powerups.typeId = (RakNet::MessageID)ID_LOADPOWERUPS;
	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&powerups), sizeof(powerups), HIGH_PRIORITY, RELIABLE_ORDERED, 0, c_packet_ptr->systemAddress,false);	
}


void Room::SendLoadMapOrder(RakNet::SystemAddress client_address)
{
	MapLoadInformation mapInfo;
	mapInfo.typeId = (RakNet::MessageID)ID_LOAD_MAP;
	mapInfo.mapId = 0;
	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&mapInfo),sizeof(mapInfo),HIGH_PRIORITY,RELIABLE_ORDERED,0,client_address,false);
}

/**
 * @brief Quick packet resend
 * 
 * @param c_packet_ptr 
 */
void Room::UpdatePlayerState(RakNet::Packet* c_packet_ptr)
{
	InputDataPacket* receivedData = (InputDataPacket*)c_packet_ptr->data;
	assert(c_packet_ptr->length == sizeof(InputDataPacket));
	if (c_packet_ptr->length != sizeof(InputDataPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}
	receivedData->typeId = (RakNet::MessageID)ID_UPDATE_ENTITY;

	for(ClientID client: m_clients)
	{
		if(client.serverID != receivedData->entityID)
			m_peerInterface_ptr->Send(reinterpret_cast<char*>(receivedData), sizeof(*receivedData), HIGH_PRIORITY, RELIABLE_ORDERED,0, client.sys_address, false);
	}
}

void Room::BroadcastInitGame()
{
	std::cout<<"Broadcasting game readiness"<<std::endl;
	
	for(ClientID& client : m_clients)
	{
		client.ready = false;
		JoinRoomInformation roomInfo;
    	roomInfo.typeId = (RakNet::MessageID)ID_STARTGAME;
		roomInfo.roomID = m_RoomID_i;
		m_peerInterface_ptr->Send(reinterpret_cast<char*>(&roomInfo),sizeof(roomInfo),HIGH_PRIORITY,RELIABLE_ORDERED,0,client.sys_address,false);
	}

	std::cout<<"Broadcasted"<<std::endl;
	m_roomState_en = RoomStates::WAITCONNECTIONS_STATE;
}

void Room::DisconnectPlayer(RakNet::Packet* c_packet_ptr)
{

	DisconnectionPacket* receivedData = (DisconnectionPacket*)c_packet_ptr->data;
	assert(c_packet_ptr->length == sizeof(DisconnectionPacket));
	if (c_packet_ptr->length != sizeof(DisconnectionPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	for(ClientID client: m_clients)
	{
		if(client.serverID != receivedData->entityID)
			m_peerInterface_ptr->Send(reinterpret_cast<char*>(receivedData), sizeof(*receivedData), HIGH_PRIORITY, RELIABLE_ORDERED,0, client.sys_address, false);
	}
}

void Room::ResendPowerUp(RakNet::Packet* c_packet_ptr){
	
	SendPUPacket* receivedData = (SendPUPacket*)c_packet_ptr->data;
	assert(c_packet_ptr->length == sizeof(SendPUPacket));
	if (c_packet_ptr->length != sizeof(SendPUPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data";
		return;
	}

	for(ClientID client: m_clients)
	{
		if(client.serverID != receivedData->entityID)
			m_peerInterface_ptr->Send(reinterpret_cast<char*>(receivedData), sizeof(*receivedData), HIGH_PRIORITY, RELIABLE_ORDERED,0, client.sys_address, false);
	}
}