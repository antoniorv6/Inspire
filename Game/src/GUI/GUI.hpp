#include <vector>
#include <string>
#include <Event.h>

struct GenericData;
class RenderManager;
class EventManager;
class InputManager;
class SoundManager;
class LoadingBar;

class GUIButton;

class GUI
{
    public:
        GUI(GenericData *& c_genericData);
        ~GUI();

        void Update();
        void CheckResponsive();
        void UpdateMouse();
        void CheckInteractions();


        int AddGUIButton(int c_xPosition, int c_yPosition, int c_width, int c_height, EventType c_eventToTrigger_e, int c_actionTrigger, const std::string& c_textureNoHover, const std::string& c_textureHover);
        void ChangeButtonTextures(int c_ButtonID, const std::string& c_textureNoHover, const std::string& c_textureHover);
        void SetBackground(const std::string& c_textureForBG);
        void SetBackground(const std::string& c_textureForBG, int width, int height);
        void CleanGUI();

        void AddLoadingBar(int c_xPosition, int c_yPosition, int c_width, int c_height);
        void AddProgressToLoadingBar(float c_newProgress);
        

    private:
        std::vector<GUIButton*> m_GUIButtons_v;
        RenderManager* m_renderManager_ptr     = nullptr;
        EventManager*  m_eventManager_ptr      = nullptr;
        InputManager*  m_inputManager_ptr      = nullptr;
        SoundManager*  m_soundManager_ptr      = nullptr;

        LoadingBar*    m_loadingBar = nullptr;

        int m_actualScreenWidth = 0, m_actualScreenHeight = 0;
        bool m_mouseClicked = true;
        int m_mouseX = 0;
        int m_mouseY = 0;
        int m_bgSet = 0;
};
