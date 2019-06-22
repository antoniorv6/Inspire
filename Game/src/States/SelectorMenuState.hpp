#include <State.hpp>
#include <StatesMachine.hpp>
#include <vector>
#include <string>

class RenderManager;
class EntityManager;
class InputManager;
class SoundManager;
class CameraManager;
class GUIManager;
class DebugGUIManager;
class Entity;
class GenericData;
class EventManager;
class GUI;
class Clock;
struct DroneData;

enum SelectorState
{
    SELECT_DRONE = 0,
    SELECT_POWERUPS = 1
};

class SelectorMenuState : public State
{
    public:
        SelectorMenuState(GenericData* c_gameData);
        ~SelectorMenuState();
        void Init();
        void Update(float&);
        void Draw();
        void ChangeState();
        void CheckForStateChange();
        void DrawStateGUI();
        
        void SelectPowerUp(int*, int);
        void SelectDrone(int);
        void MoveTarget();

        void RotateDrones();
        void UpdatePerspective();
        
        bool SavePlayerPU();
        
        void ShowPUMenu();
        void InitDroneToShow(Entity* c_entity, const std::string& c_mesh, const std::string& c_texture);

        virtual void CloseState();
        void ClearVectors();

        void ProcessGUIEvent(int c_eventType, int c_action); //Left = 1, Right = 0

        void MoveDrones(float& delta);
        void CheckCharacter(std::string c_characterName);
        void CharacterSelected(int c_selectedDrone);
     
    private:
        /*PowerUps Selector*/
        int m_selected_PowerUp;
        int m_selected_PowerUpLEFT = 0;
        int m_selected_PowerUpRIGHT = 1;
        int m_selectedDrone = 0;
        int m_powerUpLeftGUI = 0, m_PowerUpRightGUI = 0;
        bool deleteState = false;
        int m_perspective;

        GenericData* m_gameData     = nullptr;
        Clock* m_clickClock_ptr     = nullptr;

        RenderManager*      render            = nullptr;
        EntityManager*      entityManager     = nullptr;
        InputManager*       inputManager      = nullptr;
        SoundManager*       soundManager      = nullptr;
        CameraManager*      cameraManager     = nullptr;
        Entity*             m_demoCube        = nullptr; 
        EventManager*       m_eventManager    = nullptr;
        PhysicsManager*     m_physicsManager  = nullptr;
        GUI*                m_GUI_ptr         = nullptr;

        Entity*             vladimir;
        Entity*             karen;
        Entity*             jack;
        Entity*             lyanna;
        //Entity*             mansion;

        glm::vec3 m_desiredPosition = {0,0,0};
        glm::vec3 m_increment = {-1,-1,-1};

        std::vector<DroneData> m_dronesToChoose;
        std::vector<Entity*>   m_droneEntitites;
        std::vector<std::string> m_powerUpsTextures_v;

        SelectorState m_actualState;

        void ProcessGoBack();
        void SetDroneSelectionButtons();
        void GoToMainMenu();

        glm::vec3 m_billboardPos;
};

