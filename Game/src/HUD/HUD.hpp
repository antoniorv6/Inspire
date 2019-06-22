#include <vector>
#include <glm.hpp>
#include <string>

class HudElement;
class LAPHudElement;
class GenericData;
class RenderManager;
class PositionHUDElement;
class NitroHUDElement;
class PowerUpHUDElement;
class NitroBarHUDElement;
class CountDownHUDElement;

class HUD 
{
    public:
        HUD(GenericData *&c_genericData);
        ~HUD();
        void AddHUDElement(const std::string& c_HUDElementTexture, int c_xPos_i, int c_yPos_i, int c_width, int c_height);
        void AddLapHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height, int c_actualLap);
        void AddPositionHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height, int c_actualPosition);
        void AddNitroHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height);
        
        void AddCountDownHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height);
        void UpdateCountDownHUD(int c_countDown);

        void AddPowerupHUDElement(int c_xPos_i, int c_yPos_i, std::vector<std::string> c_textures, bool c_leftOrRight);
        void SetPowerUpTexture(int c_teturePos, bool leftorRight);
        
        void UpdateLAP(int c_currentLap_i);
        void UpdatePosition(int c_currentPosition_i);
        void UpdateNitro(float c_newNitroCharge);

    private:
        RenderManager* m_renderManager            = nullptr;
        std::vector<HudElement*> m_HUDElements_v;
        
        LAPHudElement * m_LapHUD_ptr             = nullptr;
        PositionHUDElement * m_positionHUD_ptr   = nullptr;
        NitroHUDElement *    m_nitroHUD_ptr      = nullptr;
        PowerUpHUDElement*   m_leftPU_ptr        = nullptr;
        PowerUpHUDElement*   m_rightPU_ptr       = nullptr;   
        NitroBarHUDElement*  m_nitroBar_ptr      = nullptr;
        CountDownHUDElement* m_countDown_ptr     = nullptr;
        
        float m_oldNitroCharge = 0;

        int m_lastColorToUseNitro = 0;

        bool m_wasNitroFull = false;

};