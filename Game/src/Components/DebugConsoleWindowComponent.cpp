#include <DebugConsoleWindowComponent.hpp>
#include <IrrIMGUI.h>

/**
 * @brief Construct a new Debug Console Window Component:: Debug Console Window Component object
 * 
 * @param id 
 * @param id_ent 
 * @param c_X 
 * @param c_Y 
 * @param c_W 
 * @param c_H 
 */
DebugConsoleWindowComponent::DebugConsoleWindowComponent(const std::string& id, 
                                                        int id_ent, 
                                                        int c_X, 
                                                        int c_Y, 
                                                        int c_W, 
                                                        int c_H) : IComponent(id, id_ent)
{

    m_WindowCaption_str = id;
    m_posX_i = c_X;
    m_posY_i = c_Y;
    m_width_i = c_W;
    m_height_i = c_H;
};

/**
 * @brief Destroy the Debug Console Window Component:: Debug Console Window Component object
 * 
 */
DebugConsoleWindowComponent::~DebugConsoleWindowComponent()
{

}

/**
 * @brief 
 * 
 * @param text 
 */
void DebugConsoleWindowComponent::AddTextDataString(std::string text) 
{
    m_TextData_str.push_back(text);
};

/**
 * @brief 
 * 
 */
void DebugConsoleWindowComponent::RenderWindow()
{
    ImGui::SetNextWindowSize(ImVec2(m_width_i, m_height_i));
    ImGui::SetNextWindowPos(ImVec2(m_posX_i, m_posY_i));
    ImGui::Begin(m_WindowCaption_str.c_str());

    for(const std::string& text : m_TextData_str)
    {
        ImGui::Text(text.c_str());
    }

    ImGui::End();
};

/**
 * @brief 
 * 
 */
void DebugConsoleWindowComponent::ClearWindow()
{
    m_TextData_str.clear();
}