#include <RakPeerInterface.h>
#include <NatPunchthroughClient.h>
#include <string>
#include <State.hpp>
#include <vector>

enum LobbyStates 
{
    LOGIN_STATE = 0,
    SELECT_ROOM_STATE = 1,
    IN_ROOM_STATE = 2
};

class EntityManager;
class RenderManager;
class CameraManager;
class InputManager;
class Entity;
class OrionHTTPRequester;
struct RoomData;
class Clock;
class GenericData;

class LobbyState : public State 
{
    public:

        LobbyState(GenericData *&c_gameData);
        ~LobbyState();

        void Init();
        void Update(float&);
        void Draw();
        void DrawStateGUI();
        void ChangeState();

        void ProccessPackets();
        void RegisterServer();

        void RegisterAndJoinRoom();
        void JoinRoom(unsigned int c_room_id);

        void SendReadyStatus();
        void ConnectToServer();

        void CalculatePing();
        void SendPing();

        void ReadNetworkData();

        void CloseState();

        void DrawPowerUpSelector();
        void SelectPowerUp(bool* l_PU, int* PowerUp, int type, char side);
        bool SavePlayerPU();

        void Login(std::string c_username_str, std::string c_password_str);

    private:
        RakNet::RakPeerInterface* m_peerInterface_ptr;
        RakNet::SocketDescriptor  m_socketDescriptor_sock;
        RakNet::NatPunchthroughClient* m_clientPunch_ptr;
        RakNet::Packet*           m_packetReceiver_ptr;

        RakNet::SystemAddress     m_serverAddress;

        EntityManager* m_EntityManager_ptr;
        CameraManager* m_CameraManager_ptr;
        RenderManager* m_renderManager_ptr;
        InputManager*  m_inputManager_ptr;
        Entity*        m_mainEntity_ptr;
        GenericData*   m_gameData_ptr;

        OrionHTTPRequester* m_HTTPRequester_ptr;

        LobbyStates m_actualState_en;

        bool m_isPlayerReady_b;
        int  m_roomJoined_i;

        std::vector<RoomData> m_roomsAvailable;

        Clock* m_clock;
        Clock* m_pingClock;

        bool   m_requestsent;

        const char* m_serverIP;
        unsigned int m_serverPort;

        std::string m_customRoomName_str;

        //Power up selector
        int m_selected_PowerUp;
        int m_selected_PowerUpLEFT = -1;
        int m_selected_PowerUpRIGHT = -1;
        bool m_powerUpsLEFT [6] = { false };
        bool m_powerUpsRIGHT [6] = { false };

};