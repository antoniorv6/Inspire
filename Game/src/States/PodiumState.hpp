#include <State.hpp>
#include <StatesMachine.hpp>


class RenderManager;
class EntityManager;
class InputManager;
class SoundManager;
class CameraManager;
class Entity;
class GenericData;
class GUI;
class EventManager;
class PositionManager;

class PodiumState : public State
{
    public:
        PodiumState(GenericData* c_gameData, std::string& c_playerName);
        ~PodiumState();
        void Init();
        void Update(float&);
        void Draw();
        void ChangeState();
        void ChangetoOnline();
        void CheckForStateChange();
        void DrawStateGUI(){};
        void CloseState();
        void CreateAnimations();
        void PlayVladimirVictory();
        void PlayVladimirAplausos();
        void PlayVladimirLoose();
        void PlayKarenVictory();
        void PlayKarenAplausos();
        void PlayKarenLoose();
        void PlayJackVictory();
        void PlayJackAplausos();
        void PlayJackLoose();
        void PlayLyannaVictory();
        void PlayLyannaAplausos();
        void PlayLyannaLoose();
    
    private:
        int                 m_playerPosition = 0;
        int                 m_characterFirst = 0;

        Entity*             vladimir = nullptr;
        Entity*             karen    = nullptr;
        Entity*             jack     = nullptr;
        Entity*             lyanna   = nullptr;
        Entity*             podium   = nullptr;

        GUI*                m_GUI_ptr = nullptr;

        RenderManager*      render             = nullptr;
        EntityManager*      entityManager      = nullptr;
        InputManager*       inputManager       = nullptr; 
        SoundManager*       soundManager       = nullptr;
        CameraManager*      cameraManager      = nullptr;
        Entity*             m_demoCube         = nullptr;
        EventManager*       m_eventManager_ptr = nullptr;
        PositionManager*    positionManager    = nullptr;
        GenericData*       m_gameData          = nullptr;

        std::vector<std::string>* m_finalResults_v;
        std::string m_playerCharacterName = "";
};

