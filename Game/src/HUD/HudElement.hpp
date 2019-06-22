#include <string>
#include <vector>

class HudElement
{
    public:
        HudElement(int c_xPosition_i, int c_yPosition_i)
        {
            m_xPosition_i = c_xPosition_i;
            m_yPosition_i = c_yPosition_i;
        }
        virtual ~HudElement(){};
    private:
        int m_xPosition_i = 0, m_yPosition_i = 0;
};

class LAPHudElement: public HudElement
{
    public:
        LAPHudElement(int c_xPosition_i, int c_yPosition_i, int c_actualLap_i, int c_ID):HudElement(c_xPosition_i, c_yPosition_i)
        {
            m_actualLap_i = c_actualLap_i;
            m_texturesToHandle.reserve(3);
            m_ID_i = c_ID;
            m_texturesToHandle.emplace_back("1.png");
            m_texturesToHandle.emplace_back("2.png");
            m_texturesToHandle.emplace_back("3.png");
        }
        virtual ~LAPHudElement(){};
        std::string UpdateTexture(int c_actualLap)
        {
            if(c_actualLap != m_actualLap_i && c_actualLap < (int)m_texturesToHandle.size())
            {
                m_actualLap_i = c_actualLap;
                return m_texturesToHandle[c_actualLap];
            }
            return NULL;
        }
        std::string GetTexture() {return m_texturesToHandle[m_actualLap_i];};
        int GetID() { return m_ID_i; };
    private:
        int m_actualLap_i = 0, m_ID_i = 0;
        std::vector <std::string> m_texturesToHandle;
};

class PositionHUDElement: public HudElement
{
    public:
        PositionHUDElement(int c_xPosition_i, int c_yPosition_i, int c_actualPosition_i, int c_ID):HudElement(c_xPosition_i, c_yPosition_i)
        {
            m_actualPosition_i = c_actualPosition_i;
            m_texturesToHandle.reserve(4);
            m_ID_i = c_ID;
            m_texturesToHandle.emplace_back("first.png");
            m_texturesToHandle.emplace_back("second.png");
            m_texturesToHandle.emplace_back("third.png");
            m_texturesToHandle.emplace_back("fourth.png");
        }
        std::string UpdateTexture(int c_actualPosition)
        {
            if(c_actualPosition != m_actualPosition_i && c_actualPosition < (int)m_texturesToHandle.size())
            {
                m_actualPosition_i = c_actualPosition;
                return m_texturesToHandle[c_actualPosition];
            }
            return "";
        }
        std::string GetTexture() {return m_texturesToHandle[m_actualPosition_i];};
        virtual ~PositionHUDElement(){};
        int GetID() {return m_ID_i;};
    private:
        int m_actualPosition_i = 0, m_ID_i = 0;
        std::vector <std::string> m_texturesToHandle;
};

class CountDownHUDElement: public HudElement
{
    public:
        CountDownHUDElement(int c_xPosition_i, int c_yPosition_i, int c_ID):HudElement(c_xPosition_i, c_yPosition_i)
        {
            m_id = c_ID;
            m_texturesToHandle.emplace_back("countdown_1.png");
            m_texturesToHandle.emplace_back("countdown_2.png");
            m_texturesToHandle.emplace_back("countdown_3.png");
        }
        std::string UpdateCountDownTexture(int c_countdown)
        {
            return m_texturesToHandle[c_countdown];
        }
        int GetID() {return m_id;};
    private:
        int m_actualCount, m_id;
        std::vector<std::string> m_texturesToHandle;
};

class NitroHUDElement: public HudElement
{
    public:
        NitroHUDElement(int c_positionX, int c_positionY, int c_barWidth, int c_barHeight, int c_maxbarWidth, int c_ID):HudElement(c_positionX, c_positionY)
        {
            m_width = c_barWidth;
            m_height = c_barHeight;
            m_maxWidth = c_maxbarWidth;
            m_spriteColors_v.reserve(4);
            m_spriteColors_v.emplace_back("textura_ia.png");
            m_spriteColors_v.emplace_back("mid_nitro.png");
            m_spriteColors_v.emplace_back("semi_full_bar.png");
            m_spriteColors_v.emplace_back("full_bar.png");
            m_colorToUse = 0;
            m_ID = c_ID;
        }
        virtual ~NitroHUDElement(){ }
        int CalculateNewWidth(float c_newNitroCharge)
        {
            m_width = (c_newNitroCharge * m_maxWidth)/100;
            m_colorToUse = CalculateColorToUse(c_newNitroCharge);
            return m_width;
        }
        int CalculateColorToUse(float c_newNitroCharge)
        {
            if(c_newNitroCharge == 100.0)
                return 3;
            if(c_newNitroCharge > 50.0)
                return 2;
            if(c_newNitroCharge > 25.0)
                return 1;
            
            return 0;
        }
        std::string GetTexture()
        {
            return m_spriteColors_v[m_colorToUse];
        }
        int GetHeight() {return m_height;}
        int GetID() {return m_ID;}
        int GetColorToUse() {return m_colorToUse;}
    private:
        int m_width = 0, m_height = 0, m_maxWidth = 0;
        std::vector<std::string>m_spriteColors_v;
        int m_colorToUse = 0;
        int m_ID = 0;
};

class NitroBarHUDElement: public HudElement
{
    public:
    NitroBarHUDElement(int c_xPosition_i, int c_yPosition_i, int c_ID):HudElement(c_xPosition_i, c_yPosition_i)
    {
        full_charge_bar = "energy_bar_charged.png";
        normalBar       = "energy_bar.png";
        id = c_ID;
    };
    virtual ~NitroBarHUDElement(){};
    std::string GetChargedBar()
    {
        return full_charge_bar;
    }
    std::string GetNormalBar()
    {
        return normalBar;
    };
    int GetID()
    {
        return id;
    }
    private:
        std::string full_charge_bar;
        std::string normalBar;
        int id = 0;

};

class PowerUpHUDElement: public HudElement
{
    public:
    PowerUpHUDElement(int c_xPosition_i, int c_yPosition_i, int c_ID, std::vector<std::string>&c_textures):HudElement(c_xPosition_i, c_yPosition_i)
    {
        m_textures = c_textures; 
        m_activeTexture = c_textures[0];
        m_ID = c_ID;
    };
    void SetActiveTexture(int c_position)
    {
        m_activeTexture = m_textures[c_position];
    }
    std::string GetActiveTexture()
    {
        return m_activeTexture;
    }
    virtual ~PowerUpHUDElement()
    {
        std::vector<std::string>empty;
        m_textures.swap(empty);
    };
    int GetID()
    {
        return m_ID;
    }
    private:
    std::string m_activeTexture = "";
    std::vector<std::string> m_textures;
    int m_ID = 0;
};