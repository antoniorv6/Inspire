#pragma once
#include <RakPeerInterface.h>
#include <NatPunchthroughServer.h>
#include <OrionEnums.hpp>
#include <vector>

struct ClientID;
class  OrionHTTPRequester;
class  Clock;

class Room;
struct ServerCallBack;
class OrionServer 
{
    public:
        OrionServer(unsigned int c_maxConnections, unsigned int c_serverPort);
        ~OrionServer();

        void Init();
        void ProccessPackets();
        void RegisterClient();
        void SendLoadMapOrder();

        void UpdatePlayerInputs();

        void Update();

        void BroadcastDataToClients();
        void UpdateBetweenClients();

        void ProccessRoomJoin();
        void ProccesPlayerReadyStatus();

        void CreateRoom();
        void DisconnectFromRoom();

        void OrderPowerUps();

        void SyncPlayers();

        void SendPowerUpType();

    private:
        RakNet::RakPeerInterface* m_peerInterface_ptr;
        RakNet::SocketDescriptor  m_socketDescriptor_sock;
        RakNet::Packet*           m_serverPacketReceiver_ptr;
        RakNet::NatPunchthroughServer* m_natPunch_ptr;

        OrionHTTPRequester* m_HTTPRequester_ptr;

        unsigned int m_MAXCONNECTIONS_i, m_SERVERPORT_i;
        unsigned int numclients;
        
        std::vector<ClientID> m_clients;

        std::vector<Room*> m_rooms_vec;

        Clock * m_tickClock;
        int m_tickTime; //Expressed in miliseconds - 24 = 60 ticks/s

        void SendPreviousClients();
};