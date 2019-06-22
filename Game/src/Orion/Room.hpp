#include <RakPeerInterface.h>
#include <vector>
#include <glm.hpp>

enum RoomStates
{
    LOBBY_STATE = 0,
    WAITCONNECTIONS_STATE  = 1,
    GAME_STATE = 2
};

class PhysicsManager;
class EventManager;
class EntityManager;
class MapManager;
class CheckpointManager;
class PositionManager;
class LapManager;
class GenericData;
class PowerUpManager;
class ClientID;
class Clock;

class Room
{
    public:

        Room(unsigned int c_room_id);
        ~Room();

        void ReceivePacket(RakNet::Packet* c_packet_ptr);
        void Init(RakNet::RakPeerInterface*& c_peerInterface_ptr);
        
        void SetClientReady(RakNet::Packet* c_packet_ptr);
        void IsEveryOneReady();
        void IsEveryOneInGame();
        
        void StartGame();
        void BroadcastInitGame();

        void SendLoadMapOrder(RakNet::SystemAddress client_address);

        void UpdatePlayerState(RakNet::Packet* c_packet_ptr);
        void RegisterClient(RakNet::Packet* c_packet_ptr);

        void Update(float delta);

        void SendSyncPacket();
        void BroadcastSync(RakNet::Packet* c_packet_ptr);
        void SendPowerupOrder(RakNet::Packet*  c_packet_ptr);
        unsigned int GetRoomID(){ return m_RoomID_i; };

        void DisconnectPlayer(RakNet::Packet* c_packet_ptr);
        void ResendPowerUp(RakNet::Packet* c_packet_ptr);


    private:

    RakNet::RakPeerInterface* m_peerInterface_ptr;
    unsigned int m_RoomID_i;
    RoomStates m_roomState_en;
    unsigned int m_numclients_i;

    PhysicsManager* m_physicsManager;
    EventManager*   m_eventManager;
    EntityManager*  m_entityManager;
    MapManager*     m_mapManager;
    CheckpointManager* m_checkPointManager;
    PositionManager*   m_positionManager;
    LapManager*        m_lapManager;
    PowerUpManager*    m_powerUpManager;
    GenericData*  m_genericData;
    Clock*             m_clock;

    std::vector<glm::vec3> m_positions_v;


    std::vector<ClientID> m_clients;

};