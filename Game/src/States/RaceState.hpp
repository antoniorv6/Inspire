#pragma once
#include <State.hpp>
#include <Clock.hpp>
#include <glm.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <rapidxml.hpp>
#include <memory>
#include <vector>
#include <CommonData.hpp>

class RenderManager;
class EntityManager;
class EventManager;
class PhysicsManager;
class AIManager;
class InputManager;
class SoundManager;
class WaypointManager;
class CameraManager;
class CheckpointManager;
class GUIManager;
class PowerUpManager;
class Entity;
class StatesMachine;
class PositionManager;
class EventData;
class MapManager;
class GenericData;
class DebugGUIManager;
class HUD;


class RaceState: public State{

    public:
        RaceState(GenericData* c_gameData, HUD* c_HUD);
        ~RaceState();
        void Init();
        void Update(float&);
        void Scheduling();
        void InterpolateDrones(float&);
        void LoadLevel();
        void DeployAI();
        void DeployPlayer(Entity * player);
        void CreateAI(const std::string& l_id_s, const glm::vec3& l_position_v3, float l_radius_f, float l_mass_f, float l_force_f, const std::string l_texture_str);
        void IARandomValues(TypePU* PU1, TypePU* PU2, int* personality);
        void UpdateSound(Entity* player);
        void UpdateGUI(Entity* player);
        void Draw();
        void ChangeState();
        void EndGame();
        void CloseState();
        void Pause();
        void Resume();
        void DrawStateGUI();
        void Countdown(float&);
        void EndOfTheRace(float&);
        void PauseClocks();
        void PlayClocks();
        void GetPowerUpString(TypePU*, std::string*);
        void FPS(double frametime);

    private:
        RenderManager*      render              = nullptr;
        EntityManager*      entityManager       = nullptr;
        EventManager*       eventManager        = nullptr;
        PhysicsManager*     physicsManager      = nullptr;
        AIManager*          aiManager           = nullptr;
        InputManager*       inputManager        = nullptr;
        SoundManager*       soundManager        = nullptr;
        WaypointManager*    waypointManager     = nullptr;
        CameraManager*      cameraManager       = nullptr;
        CheckpointManager*  checkpointManager   = nullptr;
        PowerUpManager*     powerUpManager      = nullptr;
        PositionManager*    positionManager     = nullptr;
        GUIManager*         guiManager          = nullptr;
        StatesMachine*      statesMachine       = nullptr;
        Entity*             m_player            = nullptr;
        EventData*          m_eventData_ptr     = nullptr;
        MapManager*         m_mapManager        = nullptr;
        DebugGUIManager*    debugGUIManager     = nullptr;
        HUD*                m_HUD_ptr           = nullptr;
        
        GenericData*        m_gameData = nullptr;
        bool* isPlay        = nullptr;
        bool isCountdown    = true;
        bool isEndRace      = false;

        Clock*              m_clock = nullptr;

        double              m_FPStime_d = 0;
        int                 m_FPScounter_n=0;
        int                 m_FPS_n=0;

        TypePU              m_playerPU1;
        TypePU              m_playerPU2;
        std::string         m_playerPU1_str = "";
        std::string         m_playerPU2_str = "";

        std::vector<glm::vec3>* m_initialPositions_vec;
        std::vector<std::string> m_powerUpsTextures_v;

        int m_powerUpsSelected = 0;

        //Update's loop time
        const float loopTime = 1.0f/30.0f; //update runs 30 times every second

        int actualCountDown = 0;
	    int lastCountDown   = 0;
};