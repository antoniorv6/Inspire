#pragma once
#include <State.hpp>
#include <RakPeerInterface.h>
#include <NatPunchthroughClient.h>
#include <OrionClientPredictor.hpp>
#include <vector>
#include <CommonData.hpp>
#include <iostream>

class RenderManager;
class EntityManager;
class Entity;
class CameraManager;
class InputManager;
class PhysicsManager;
class Clock;
class GenericData;
class MapManager;
class GUIManager;
class PowerUpManager;
class EventManager;
class PositionManager;
class CheckpointManager;
class SoundManager;

class OnlineRaceState : public State
{
    public:
        OnlineRaceState(RakNet::RakPeerInterface*& c_peerInterface, RakNet::NatPunchthroughClient*& c_clientPunch, int c_roomJoined, 
                        RakNet::SystemAddress c_serverAddress, GenericData *&l_gameData_ptr, int l_selected_PowerUp);
                        
        ~OnlineRaceState();

        void Init();
        void Update(float&);
        void Draw();
        void ChangeState();
        void DrawStateGUI();
        void CloseState();

        void LoadLevel();

        void ProccessPackets();

        void CreateMainPlayer();
        void CreateExternalEnt();
        
        void CompareInputVectors();

        void InputBroadcast();

        void UpdateEntity();
        
        void InterpolateDrones(float &delta);

        void LoadPowerUps();

        void SendSyncPacket();

        void ProcessSyncPacket();
        
        void DisconnectClient();
        void SetVariablesDisconnection(unsigned int l_entityId_n);

        void Countdown(float& delta);

        void UpdateGUI();

        void EndOfTheRace(float& delta);

        void GetPowerUpString(TypePU* type, std::string* name);

        void SendPURequest();
        void ProcessPowerUps();

    private:
        RakNet::RakPeerInterface* m_peerInterface_ptr = nullptr;
        RakNet::SocketDescriptor  m_socketDescriptor_sock;
        RakNet::Packet*           m_PacketReceiver_ptr = nullptr;
        RakNet::NatPunchthroughClient* m_clientPunch_ptr = nullptr;

        RakNet::NatPunchthroughClient* m_punchClient_ptr = nullptr;

        RakNet::SystemAddress     m_serverAddress;
        
        RenderManager*            m_RenderManager_ptr     = nullptr;
        EventManager*             m_eventManager_ptr      = nullptr;
        EntityManager*            m_EntityManager_ptr     = nullptr; 
        CameraManager*            m_CameraManager_ptr     = nullptr;
        InputManager*             m_InputManager_ptr      = nullptr;
        PhysicsManager*           m_PhysicsManager_ptr    = nullptr;
        Entity*                   m_MainCharacter_ptr     = nullptr; 
        MapManager*               m_MapManager_ptr        = nullptr;
        GenericData*              m_gameData_ptr          = nullptr;
        GUIManager*               m_guiManager_ptr        = nullptr;
        PowerUpManager*           m_powerUpManager_ptr    = nullptr;
        PositionManager*          m_positionManager_ptr   = nullptr;
        CheckpointManager*        m_checkpointManager_ptr = nullptr;
        SoundManager*             m_soundManager_ptr      = nullptr;

        bool* isPlay              = nullptr;
        bool isCountdown          = true;
        int  actualCountDown;
        int  lastCountDown;
        bool powerUpReady;

        Clock*                    m_clock = nullptr;                        
        std::vector<bool>         m_inputsToSend;
        std::vector<bool>         m_oldInputs;

        int m_roomJoined_i = 0;
        bool m_isinLobby = false;
        

        const float m_loopTime = 1.0f/30.0f; //update runs 30 times every second

        std::vector<Entity*> m_PlayableCharacters_vec;

        //Power up selected
        TypePU              m_playerPU1;
        TypePU              m_playerPU2;
        std::string         m_playerPU1_str = "";
        std::string         m_playerPU2_str = "";
        uint8_t             m_send_n = 0;        //To know how many players have created their power up.
};