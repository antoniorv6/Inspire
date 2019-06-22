#include <GUI.hpp>
#include <RenderManager.h>
#include <EventManager.h>
#include <GUIElements.hpp>
#include <InputManager.hpp>
#include <SoundManager.h>

GUI::GUI(GenericData *& c_genericData)
{
    m_renderManager_ptr = c_genericData->g_renderManager_ptr;
    m_eventManager_ptr  = c_genericData->g_eventManager_ptr;
    m_inputManager_ptr  = c_genericData->g_inputManager_ptr;
    m_soundManager_ptr  = c_genericData->g_soundManager_ptr;
    
    m_actualScreenWidth = 1280;
    m_actualScreenHeight = 720;

    m_loadingBar = nullptr;
}

GUI::~GUI()
{
    CleanGUI();
}

void GUI::CleanGUI()
{
    if(m_loadingBar != nullptr)
    {
        m_renderManager_ptr->GetRenderFacade()->GUIEraseSprite(m_GUIButtons_v.capacity());
        delete m_loadingBar;
        m_loadingBar = nullptr;
    }
    else
    {
        m_renderManager_ptr->GetRenderFacade()->GUIEraseSprite(m_GUIButtons_v.size() + m_bgSet);    
    }
    
    for(unsigned int i = 0; i < m_GUIButtons_v.size(); i++)
    {
        if(m_GUIButtons_v[i])
        {
            delete m_GUIButtons_v[i];
            m_GUIButtons_v[i] = nullptr;
        }
    }
        
    m_GUIButtons_v.clear();
    m_actualScreenWidth = 1280;
    m_actualScreenHeight = 720;
}

int GUI::AddGUIButton(int c_xPosition, int c_yPosition, int c_width, int c_height, EventType c_eventToTrigger_e, int c_actionTrigger, const std::string& c_textureNoHover, const std::string& c_textureHover)
{
    GUIButton* l_newGUIButton = new GUIButton(c_xPosition, c_yPosition, c_width, c_height, c_eventToTrigger_e, c_actionTrigger, c_textureNoHover, c_textureHover, m_GUIButtons_v.size());
    m_renderManager_ptr->GetRenderFacade()->GUICreateSprite(m_GUIButtons_v.size(), c_xPosition, c_yPosition, c_textureNoHover, c_width, c_height, false);
    m_GUIButtons_v.push_back(l_newGUIButton);
    return l_newGUIButton->GetID();
}

void GUI::ChangeButtonTextures(int c_ButtonID, const std::string& c_textureNoHover, const std::string& c_textureHover)
{
    m_GUIButtons_v[c_ButtonID]->SetTextures(c_textureHover, c_textureNoHover);
    m_renderManager_ptr->GetRenderFacade()->GUIChangeSpriteTexture(c_ButtonID, c_textureNoHover, false);
}

void GUI::SetBackground(const std::string& c_textureForBG)
{
    m_renderManager_ptr->GetRenderFacade()->GUICreateSprite(m_GUIButtons_v.capacity(), 640, 360, c_textureForBG, 1280, 720, false);
    m_bgSet = 1;
}

void GUI::SetBackground(const std::string& c_textureForBG, int width, int height)
{
    m_renderManager_ptr->GetRenderFacade()->GUICreateSprite(m_GUIButtons_v.capacity(), 640, 360, c_textureForBG, width, height, false);
}

void GUI::Update()
{
    CheckResponsive();
    CheckInteractions();
}

void GUI::CheckResponsive()
{
    int l_newWidth = m_renderManager_ptr->GetRenderFacade()->GetScreenWidth();
    int l_newHeight = m_renderManager_ptr->GetRenderFacade()->GetScreenHeight();

    if(m_actualScreenWidth != l_newWidth || m_actualScreenHeight != l_newHeight)
    {
        //We have to make a responsive adaptation on all button bounding
        for(auto& button : m_GUIButtons_v)
        {
            button->MakeResponsiveAdaptation(m_actualScreenHeight, m_actualScreenWidth, l_newHeight, l_newWidth);
        }

        m_actualScreenHeight = l_newHeight;
        m_actualScreenWidth = l_newWidth;
    }
}

void GUI::UpdateMouse()
{
    if(m_mouseClicked)
    {
        m_inputManager_ptr->SetMouseClick(false);
    }
    
    m_mouseClicked = m_inputManager_ptr->IsMouseClicked();
    m_mouseX = m_inputManager_ptr->GetMousePositionX();
    m_mouseY = m_inputManager_ptr->GetMousePosityonY();
}

void GUI::CheckInteractions()
{
    UpdateMouse();
    
    for(unsigned int i = 0; i<m_GUIButtons_v.size(); i++)
    {
        GUIButton& l_buttonProcessed = *m_GUIButtons_v[i];
        
        if(l_buttonProcessed.IsButtonHovered(m_mouseX, m_mouseY))
        {
            //if it is being hovered, then we have to change the sprite
            if(!l_buttonProcessed.WasButtonHovered())
            {
                m_renderManager_ptr->GetRenderFacade()->GUIChangeSpriteTexture(i, l_buttonProcessed.GetHoverSprite(), false);
                l_buttonProcessed.SetHover(true);
                m_soundManager_ptr->SetEventVariableValue("MenuSounds", "Interaction", 0);
                m_soundManager_ptr->Play("MenuSounds");
            }
            if(m_mouseClicked)
            {
                EventType l_type = l_buttonProcessed.GetEvent();
                int       l_action = l_buttonProcessed.GetAction();
                if(l_type != EventType::Void_Event)
                {
                    //l_buttonProcessed.GetEvent()
                    Event eventToSend = {"EventGUI", l_type, nullptr, nullptr, l_action};
                    m_eventManager_ptr->addEvent(eventToSend);
                    m_soundManager_ptr->SetEventVariableValue("MenuSounds", "Interaction", 1);
                    m_soundManager_ptr->Play("MenuSounds");
                } 
            }

        }
        else
        {
            if(l_buttonProcessed.WasButtonHovered())
            {
                m_renderManager_ptr->GetRenderFacade()->GUIChangeSpriteTexture(i, l_buttonProcessed.GetNoHoverSprite(), false);
                l_buttonProcessed.SetHover(false);
            }
        }
        
    }
}

void GUI::AddLoadingBar(int c_xPosition, int c_yPosition, int c_width, int c_height)
{
    //First we create the bar
    m_GUIButtons_v.reserve(3);
    this->SetBackground("loading_screen_bg.png");

    m_renderManager_ptr->GetRenderFacade()->GUICreateSprite(0, c_xPosition, 570, "semi_full_bar.png", 0, 30, false);
    m_loadingBar = new LoadingBar(c_xPosition, c_yPosition, (c_width - (c_width/4)), 30, 0);

    m_renderManager_ptr->GetRenderFacade()->GUICreateSprite(1, c_xPosition, c_yPosition, "loading_bar.png", c_width, c_height, false);

}

void GUI::AddProgressToLoadingBar(float c_newProgress)
{
    float c_newWidth = m_loadingBar->AddProgress(c_newProgress);
    m_renderManager_ptr->GetRenderFacade()->GUIScaleSprite(0, c_newWidth, m_loadingBar->GetHeight(), false);
}
