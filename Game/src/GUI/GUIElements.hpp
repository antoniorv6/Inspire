#include <string>
#include <Event.h>

class GUIButton
{
    public:
        GUIButton(int c_xPosition, int c_yPosition, int c_width, int c_height, EventType c_eventToTrigger_e, int actionTrigger, const std::string& c_textureNoHover, const std::string& c_textureHover, int c_id)
        {
            m_xPosition = c_xPosition - 0.5*c_width;
            m_yPosition = c_yPosition - 0.5*c_height;
            m_width     = c_width;
            m_height    = c_height;
            m_eventToTrigger_ev = c_eventToTrigger_e;
            m_noHoverSprite_str = c_textureNoHover;
            m_HoverSprite_str   = c_textureHover;
            m_actionTrigger     = actionTrigger;
            m_isHovered_b = false;
            m_ID = c_id;
        };

        ~GUIButton(){};

        void MakeResponsiveAdaptation(int c_oldHeight, int c_oldWidth, int c_newHeight, int c_newWidth)
        {
            m_xPosition = (m_xPosition * c_newWidth)/c_oldWidth;
            m_yPosition = (m_yPosition * c_newHeight)/c_oldHeight;
            m_width = (m_width * c_newWidth)/c_oldWidth;
            m_height = (m_height * c_newHeight)/c_oldHeight;
        };

        std::string GetNoHoverSprite()
        {
            return m_noHoverSprite_str;
        };

        std::string GetHoverSprite()
        {
           return m_HoverSprite_str;
        };

        bool IsButtonHovered(int c_cursorX, int c_cursorY)
        {
            return ((c_cursorX > m_xPosition && c_cursorX < m_xPosition + m_width) && (c_cursorY > m_yPosition && c_cursorY < m_yPosition + m_height));
        };

        int GetID()
        {
            return m_ID;
        }

        void SetTextures(const std::string& c_textureHover, const std::string& c_textureNoHover)
        {
            m_noHoverSprite_str = c_textureNoHover;
            m_HoverSprite_str   = c_textureHover;
        }
        
        void SetHover(bool c_hoverValue){  m_isHovered_b = c_hoverValue; };
        bool WasButtonHovered(){ return m_isHovered_b; };

        int GetAction() {return m_actionTrigger;};

        EventType GetEvent() { return m_eventToTrigger_ev;};
        
    private:
        int m_xPosition, m_yPosition, m_width, m_height, m_ID, m_actionTrigger;
        std::string m_noHoverSprite_str, m_HoverSprite_str;
        EventType m_eventToTrigger_ev;
        bool m_isHovered_b;
};

class LoadingBar
{
    public:
        LoadingBar(int c_xPosition, int c_yPosition, int c_width, int c_height, int id)
        {
            m_id = id;
            m_maxWidth = c_width;
            m_maxHeight = c_height;
        }
        float AddProgress(float newProgress)
        {
            m_actualProgress += newProgress;
            float l_newWidth = (m_actualProgress * m_maxWidth) / 100.0;
            return l_newWidth;
        }
        int GetHeight() { return m_maxHeight;}

    private:
        float m_actualwidth = 0, m_actualProgress = 0.0f;
        int m_maxWidth, m_maxHeight, m_id;

};