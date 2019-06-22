#pragma once

#include <string>
#include <vector>
#include <IComponent.h>


class DebugConsoleWindowComponent : public IComponent
{
    public:
         DebugConsoleWindowComponent(const std::string& id, int id_ent, int c_X, int c_Y, int c_W, int c_H);
        ~DebugConsoleWindowComponent();

        void AddTextDataString(std::string text);
        void RenderWindow();
        void ClearWindow();

    private:
        std::string m_WindowCaption_str;
        std::vector<std::string> m_TextData_str;
        int m_posX_i = 0;
        int m_posY_i = 0;
        int m_width_i = 0;
        int m_height_i = 0;
};