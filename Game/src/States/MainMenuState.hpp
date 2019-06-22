#include <State.hpp>
#include <StatesMachine.hpp>


class RenderManager;
class EntityManager;
class InputManager;
class SoundManager;
class CameraManager;
class GUIManager;
class DebugGUIManager;
class Entity;
class GenericData;
class GUI;
class EventManager;

class MainMenuState : public State
{
    public:
        MainMenuState(GenericData* c_gameData);
        ~MainMenuState();
        void Init();
        void Update(float&);
        void Draw();
        void ChangeState();
        void ChangetoOnline();
        void CheckForStateChange();
        void DrawStateGUI();
        void CloseState();

        void ProcessGUIEvent(int c_eventType, int c_action);
    
    private:
        bool isPlay;
        bool deleteState = false;

        RenderManager*      render             = nullptr;
        EntityManager*      entityManager      = nullptr;
        InputManager*       inputManager       = nullptr;
        SoundManager*       soundManager       = nullptr;
        CameraManager*      cameraManager      = nullptr;
        GUIManager*         guiManager         = nullptr;
        Entity*             m_demoCube         = nullptr;
        EventManager*       m_eventManager_ptr = nullptr;

        GenericData*       m_gameData          = nullptr;
        GUI*               m_GUI_ptr           = nullptr;
};

