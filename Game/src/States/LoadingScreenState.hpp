#include <State.hpp>
#include <vector>
#include <glm.hpp>
#include <string>
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
class GUI;

class LoadingScreenState:public State
{
    public:
        LoadingScreenState(GenericData* c_gameData, int l_playerPU, std::string& c_playerName, std::string& c_playerDrone, std::string& c_playerTexture, const std::string& l_droneName);
        ~LoadingScreenState(){};
        virtual void Init();
        virtual void Update(float&);
        virtual void Draw(){};
        virtual void ChangeState();

        void SetCharacterNames();

        void Load();

        void LoadPlayer();
        void LoadMap();
        void LoadAI();

        void CreateAI(const DroneData& c_droneData, const glm::vec3& l_position_v3, float l_radius_f, float l_mass_f, float l_force_f);
        void GetPowerUpString(TypePU* type, std::string* name);
        void IARandomValues(TypePU* PU1, TypePU* PU2, int* personality);

    
    private:
        RenderManager*      render            = nullptr;
        EntityManager*      entityManager     = nullptr;
        EventManager*       eventManager      = nullptr;
        PhysicsManager*     physicsManager    = nullptr;
        AIManager*          aiManager         = nullptr;
        InputManager*       inputManager      = nullptr;
        SoundManager*       soundManager      = nullptr;
        WaypointManager*    waypointManager   = nullptr;
        CameraManager*      cameraManager     = nullptr;
        CheckpointManager*  checkpointManager = nullptr;
        PowerUpManager*     powerUpManager    = nullptr;
        PositionManager*    positionManager   = nullptr;
        GUIManager*         guiManager        = nullptr;
        StatesMachine*      statesMachine     = nullptr;
        Entity*             m_player          = nullptr;
        EventData*          m_eventData_ptr   = nullptr;
        MapManager*         m_mapManager      = nullptr;
        DebugGUIManager*    debugGUIManager   = nullptr;
        HUD*                m_HUD_ptr         = nullptr;
        GUI*                m_GUI_ptr         = nullptr;

        std::vector<glm::vec3>* m_initialPositions_vec;
        std::vector<DroneData>  m_possibleDrones;
        std::vector<std::string> m_powerUpsTextures_v;

        DroneData           playerData;

        TypePU              m_playerPU1;
        TypePU              m_playerPU2;

        GenericData*        m_gameData = nullptr;

        int m_powerUpsSelected = 0;
};