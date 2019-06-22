#pragma once
#include <StatesMachine.hpp>
#include <RaceState.hpp>
#include <State.hpp>

#include <Clock.hpp>

class GenericData;
class SoundManager;
class RenderManager;
class EntityManager; 
class InputManager;
class AIManager;
class CheckpointManager;
class CameraManager;
class GUIManager;
class DebugGUIManager;
class PositionManager;
class WaypointManager;
class LapManager;

class Game {
    public:
        Game(int l_graphicEngine_n);

    private:
        void Run();
        StatesMachine* m_statesMachine_ptr;

        int m_graphicEngine_n;
        /*delta time*/
        float totalTime=0;
        float deltaTime=0;

        GenericData       *  m_gameData_ptr;
        RenderManager     *  m_renderManager_ptr;
        EntityManager     *  m_entityManager_ptr;
        InputManager      *  m_inputManager_ptr;
        SoundManager      *  m_soundManager_ptr;
        PhysicsManager    *  m_physicManager_ptr;
        PowerUpManager    *  m_powerUpManager_ptr;
        AIManager         *  m_AIManager_ptr;
        CheckpointManager *  m_checkPointManager_ptr;
        MapManager        *  m_mapManager_ptr;
        EventManager      *  m_eventManager_ptr;
        CameraManager     *  m_cameraManager_ptr;
        GUIManager        *  m_guiManager_ptr;
        DebugGUIManager   *  m_debugGuiManager_ptr;
        PositionManager   *  m_positionManager_ptr;
        WaypointManager   *  m_waypointManager_ptr;
        LapManager        *  m_lapManager_ptr;
        Clock* clock      = nullptr;
};