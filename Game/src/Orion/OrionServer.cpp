#include <OrionServer.hpp>
#include <Room.hpp>
#include <BitStream.h>
#include <iostream>

#include <OrionStructs.hpp>
#include <OrionHTTPRequester.hpp>

#include <RakSleep.h>

#include <Clock.hpp>

#include <thread>

struct ServerCallBack
{
    RakNet::MessageID m_id;
    void (OrionServer::*CallbackFunc)();
};

ServerCallBack m_callbacks [] = 
{
		{ID_JOIN_ROOM, 		&OrionServer::ProccessRoomJoin        },
		{ID_PLAYER_INPUT, 	&OrionServer::UpdatePlayerInputs      },
		{ID_CLIENTREADY, 	&OrionServer::ProccesPlayerReadyStatus},
		{ID_CREATEROOM,		&OrionServer::CreateRoom			  },
		{ID_DISCONNECTION,  &OrionServer::DisconnectFromRoom      },
		{ID_CHARACTERSCREATED, &OrionServer::OrderPowerUps		  },
		{ID_SYNCENTITY,     &OrionServer::SyncPlayers             },
		{ID_GETPOWERUP,     &OrionServer::SendPowerUpType         },
		{	   0	  ,		0		}
};

OrionServer::OrionServer(unsigned int c_maxConnections, unsigned int c_serverPort):m_MAXCONNECTIONS_i(c_maxConnections), m_SERVERPORT_i(c_serverPort)
{
	std::cout<<" ---------------------------------------"<<std::endl;
	std::cout<< " ██████╗ ██████╗ ██╗ ██████╗ ███╗   ██╗"<<std::endl;
	std::cout<< "██╔═══██╗██╔══██╗██║██╔═══██╗████╗  ██║"<<std::endl;
	std::cout<< "██║   ██║██████╔╝██║██║   ██║██╔██╗ ██║"<<std::endl;
    std::cout<< "██║   ██║██╔══██╗██║██║   ██║██║╚██╗██║"<<std::endl;
    std::cout<< "╚██████╔╝██║  ██║██║╚██████╔╝██║ ╚████║"<<std::endl;
 	std::cout<< "╚═════╝ ╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝" <<std::endl;
	std::cout<<" ---------------------------------------"<<std::endl;

	numclients = 0;
	m_tickTime = 34;
	m_tickClock = new Clock();
	m_tickClock->Init();
}

OrionServer::~OrionServer()
{
	std::cout<<"Shutting down the Server..."<<std::endl;
	for(auto& room : m_rooms_vec)
	{
		m_HTTPRequester_ptr->DeleteRoom((int)room->GetRoomID());
		delete room;
		room = nullptr;
	}
	m_rooms_vec.clear();

    m_peerInterface_ptr->Shutdown(100,0);
	RakNet::RakPeerInterface::DestroyInstance(m_peerInterface_ptr);

	delete m_natPunch_ptr;
	
	delete m_HTTPRequester_ptr;

	delete m_tickClock;

    std::cout<<"[SUCCESS] - Server shutted down correclty"<<std::endl;
}

void OrionServer::Init()
{
	m_natPunch_ptr = RakNet::NatPunchthroughServer::GetInstance(); //TODO Check if it is useful

	//Get raknet peer interface instance to procceed
    m_peerInterface_ptr = RakNet::RakPeerInterface::GetInstance();

	m_peerInterface_ptr->AttachPlugin(m_natPunch_ptr);
	
	//Set up the server socket
	//TODO -> Try to create a little script to autodetect the server IP and use it in strcpy function
	m_socketDescriptor_sock = RakNet::SocketDescriptor(m_SERVERPORT_i, 0); // 0 = 127.1.1 - Localhost
	//strcpy(m_socketDescriptor_sock.hostAddress, "192.168.1.39");
    
	//Start the port listening with our socket
	m_peerInterface_ptr->Startup(m_MAXCONNECTIONS_i,&m_socketDescriptor_sock,1);
    m_peerInterface_ptr->SetMaximumIncomingConnections(m_MAXCONNECTIONS_i);

	//Init the HTTP Requester
	m_HTTPRequester_ptr = new OrionHTTPRequester("zenongames.herokuapp.com");

    std::cout<<"[SUCCESS] - Server initialized correctly"<<std::endl;
    std::cout<<"The server GUID is ->"<<m_peerInterface_ptr->GetMyGUID().ToString()<<std::endl;
    std::cout<<"The server IP is   ->"<<m_peerInterface_ptr->GetSystemAddressFromGuid(m_peerInterface_ptr->GetMyGUID()).ToString()<<std::endl;
}

void OrionServer::Update()
{
	m_tickClock->Restart(); //TickClock is now 0

	ProccessPackets();
	
	std::vector<std::thread> m_threads;
	m_threads.reserve(m_rooms_vec.size());
	
	/*	
		Initialization of the rooms threads. It calls the Update method of the rooms that we have to handle
	*/
	for(auto& room : m_rooms_vec)
	{
		m_threads.emplace_back(std::thread(&Room::Update, room, 0.7));
	}

	//We wait till all threads have ended the function
	for(auto& threadToJoin : m_threads)
	{
		threadToJoin.join();
	}

	//All rooms have updated at this point

	double l_elapsedTime = m_tickClock->GetElapsedTime() * 1000.0;
	
	if(l_elapsedTime<m_tickTime)
	{
		RakSleep(m_tickTime - l_elapsedTime);
	}

}

void OrionServer::ProccessPackets()
{
	//We check for every single packet that has been received by the server at this moment
    for(m_serverPacketReceiver_ptr = m_peerInterface_ptr->Receive(); m_serverPacketReceiver_ptr; m_peerInterface_ptr->DeallocatePacket(m_serverPacketReceiver_ptr), m_serverPacketReceiver_ptr = m_peerInterface_ptr->Receive())
    {
		ServerCallBack* l_nextCall = m_callbacks;
		while(l_nextCall->m_id)
		{
			if(m_serverPacketReceiver_ptr->data[0] == l_nextCall->m_id)
			{
				(this->*l_nextCall->CallbackFunc)();
			}
			++l_nextCall;
		}
    }
}

void OrionServer::UpdatePlayerInputs()
{
	InputDataPacket* newInputs = (InputDataPacket*)m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(InputDataPacket));
	if (m_serverPacketReceiver_ptr->length != sizeof(InputDataPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}

	m_rooms_vec[newInputs->roomID]->UpdatePlayerState(m_serverPacketReceiver_ptr);
}

void OrionServer::ProccessRoomJoin()
{
	std::cout<<"One player wants to join the room"<<std::endl;
	JoinRoomInformation* roomInfo = (JoinRoomInformation*)m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(JoinRoomInformation));
	if (m_serverPacketReceiver_ptr->length != sizeof(JoinRoomInformation))
		{
			std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
			return;
		}
	//The packet is correct, so we have to send the info to the room (resend the packet)
	std::cout<<roomInfo->roomID<<std::endl;
	m_rooms_vec[roomInfo->roomID]->RegisterClient(m_serverPacketReceiver_ptr);
}

void OrionServer::ProccesPlayerReadyStatus()
{
	std::cout<<"One player says he is ready to play"<<std::endl;
				
	JoinRoomInformation* roomInfo = (JoinRoomInformation*)m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(JoinRoomInformation));
	if (m_serverPacketReceiver_ptr->length != sizeof(JoinRoomInformation))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}
	//The packet is correct, so we have to send the info to the room (resend the packet)
	m_rooms_vec[roomInfo->roomID]->SetClientReady(m_serverPacketReceiver_ptr);
}

void OrionServer::CreateRoom()
{
	std::cout<<"Creating a room..."<<std::endl;

	//We create a heap pointer with our new room and initialize it. Finally we store it into our vector
	Room* l_newRoom_ptr = new Room(m_rooms_vec.size());
	l_newRoom_ptr->Init(m_peerInterface_ptr);
	m_rooms_vec.emplace_back(l_newRoom_ptr);

	//We send the confirmation message to our client
	JoinRoomInformation l_packet;
	l_packet.typeId = (RakNet::MessageID)ID_ROOMCREATED;
	l_packet.roomID = l_newRoom_ptr->GetRoomID();
	m_peerInterface_ptr->Send(reinterpret_cast<char*>(&l_packet), sizeof(l_packet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverPacketReceiver_ptr->systemAddress, false);	
}

void OrionServer::DisconnectFromRoom()
{
	std::cout<<"Client sent a disconnection package"<<std::endl;
	DisconnectionPacket* l_disconnectionPacket_ptr = (DisconnectionPacket*) m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(DisconnectionPacket));

	if (m_serverPacketReceiver_ptr->length != sizeof(DisconnectionPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}

	m_rooms_vec[l_disconnectionPacket_ptr->roomID]->DisconnectPlayer(m_serverPacketReceiver_ptr);
}

void OrionServer::SyncPlayers()
{
	EntityUpdatePacket* entityUpdate = (EntityUpdatePacket*) m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(EntityUpdatePacket));

	if (m_serverPacketReceiver_ptr->length != sizeof(EntityUpdatePacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}

	m_rooms_vec[entityUpdate->roomID]->BroadcastSync(m_serverPacketReceiver_ptr);
}

void OrionServer::OrderPowerUps()
{
	JoinRoomInformation * emptyPacket = (JoinRoomInformation*) m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(JoinRoomInformation));

	if (m_serverPacketReceiver_ptr->length != sizeof(JoinRoomInformation))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}

	m_rooms_vec[emptyPacket->roomID]->SendPowerupOrder(m_serverPacketReceiver_ptr);
}

void OrionServer::SendPowerUpType()
{
	SendPUPacket * emptyPacket = (SendPUPacket*) m_serverPacketReceiver_ptr->data;
	assert(m_serverPacketReceiver_ptr->length == sizeof(SendPUPacket));

	if (m_serverPacketReceiver_ptr->length != sizeof(SendPUPacket))
	{
		std::cout<<"[ERROR] - Couldn't proccess the client's data"<<std::endl;
		return;
	}

	m_rooms_vec[emptyPacket->roomID]->ResendPowerUp(m_serverPacketReceiver_ptr);
}