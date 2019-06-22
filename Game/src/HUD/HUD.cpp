#include <HUD.hpp>
#include <HudElement.hpp>
#include <RenderManager.h>
#include <GenericData.hpp>

/**
 * @brief Construct a new HUD::HUD object
 * 
 * @param c_genericData 
 */
HUD::HUD(GenericData *&c_genericData)
{
    m_renderManager = c_genericData->g_renderManager_ptr;
    m_HUDElements_v.reserve(7);
    m_LapHUD_ptr = nullptr;
}

/**
 * @brief Destroy the HUD::HUD object
 * 
 */
HUD::~HUD()
{
    delete m_positionHUD_ptr;
    m_positionHUD_ptr = nullptr;
    delete m_LapHUD_ptr;
    m_LapHUD_ptr = nullptr;
    delete m_nitroHUD_ptr;
    m_nitroHUD_ptr = nullptr;
    
    if(m_countDown_ptr != nullptr)
    { 
        delete m_countDown_ptr;
        m_countDown_ptr = nullptr; 
    }
    
    delete m_rightPU_ptr;
    delete m_leftPU_ptr; 
    delete m_nitroBar_ptr; 

    m_rightPU_ptr = nullptr;
    m_leftPU_ptr = nullptr;
    m_nitroBar_ptr = nullptr;

    for(auto & hudElement : m_HUDElements_v)
    {
        hudElement = nullptr;
    }
    m_HUDElements_v.clear();
}

/**
 * @brief 
 * 
 * @param c_HUDElementTexture 
 * @param c_xPos_i 
 * @param c_yPos_i 
 * @param c_width 
 * @param c_height 
 */
void HUD::AddHUDElement(const std::string& c_HUDElementTexture, int c_xPos_i, int c_yPos_i, int c_width, int c_height)
{
    HudElement* l_HUDelement = new HudElement(c_xPos_i, c_yPos_i);
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size(), c_xPos_i, c_yPos_i, 
                                                        c_HUDElementTexture, c_width, c_height, true);
    m_HUDElements_v.emplace_back(l_HUDelement);
}

/**
 * @brief 
 * 
 * @param c_xPos_i 
 * @param c_yPos_i 
 * @param c_width 
 * @param c_height 
 * @param c_actualLap 
 */
void HUD::AddLapHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height, int c_actualLap)
{
    m_LapHUD_ptr = new LAPHudElement(c_xPos_i, c_yPos_i, c_actualLap, m_HUDElements_v.size() + 1);
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size() + 1, c_xPos_i, c_yPos_i, m_LapHUD_ptr->GetTexture(), c_width, c_height, true);
}

/**
 * @brief 
 * 
 * @param c_xPos_i 
 * @param c_yPos_i 
 * @param c_width 
 * @param c_height 
 * @param c_actualPosition 
 */
void HUD::AddPositionHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height, int c_actualPosition)
{
    m_positionHUD_ptr = new PositionHUDElement(c_xPos_i, c_yPos_i, c_actualPosition, m_HUDElements_v.size() + 2);
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size() + 2, c_xPos_i, c_yPos_i, m_positionHUD_ptr->GetTexture(), c_width, c_height, true);   
}

/**
 * @brief 
 * 
 * @param c_xPos_i 
 * @param c_yPos_i 
 * @param c_width 
 * @param c_height 
 */
void HUD::AddNitroHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height)
{
    m_nitroHUD_ptr = new NitroHUDElement(c_xPos_i, c_yPos_i, c_width, c_height, 500, m_HUDElements_v.size());
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size(), c_xPos_i, c_yPos_i, m_nitroHUD_ptr->GetTexture(), c_width, c_height, true);

    m_HUDElements_v.emplace_back(m_nitroHUD_ptr);

    m_nitroBar_ptr = new NitroBarHUDElement(c_xPos_i, c_yPos_i, m_HUDElements_v.size());
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size(), c_xPos_i + 4, c_yPos_i, m_nitroBar_ptr->GetNormalBar(), 575, 130, true);

    m_HUDElements_v.emplace_back(m_nitroBar_ptr);
}

/**
 * @brief 
 * 
 * @param c_currentLap_i 
 */
void HUD::UpdateLAP(int c_currentLap_i)
{
    m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_LapHUD_ptr->GetID(), m_LapHUD_ptr->UpdateTexture(c_currentLap_i), true);
}

/**
 * @brief 
 * 
 * @param c_currentPosition_i 
 */
void HUD::UpdatePosition(int c_currentPosition_i)
{
    std::string l_textureToHandle = m_positionHUD_ptr->UpdateTexture(c_currentPosition_i);
    if(l_textureToHandle.compare("") != 0)
        m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_positionHUD_ptr->GetID(), l_textureToHandle, true);
}

void HUD::UpdateNitro(float c_newNitroCharge)
{
    if(m_oldNitroCharge != c_newNitroCharge)
    {
        int l_widthToUpdate = m_nitroHUD_ptr->CalculateNewWidth(c_newNitroCharge);
        //std::cout<<"NitroValue: "<<c_newNitroCharge<<std::endl;
        //std::cout<<"RectangleWidth: "<<l_widthToUpdate<<std::endl;
        m_renderManager->GetRenderFacade()->GUIScaleSprite(m_nitroHUD_ptr->GetID(), l_widthToUpdate, m_nitroHUD_ptr->GetHeight(), true);
        
        if(m_lastColorToUseNitro != m_nitroHUD_ptr->GetColorToUse())
        {
            m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_nitroHUD_ptr->GetID(), m_nitroHUD_ptr->GetTexture(), true);
            m_lastColorToUseNitro = m_nitroHUD_ptr->GetColorToUse();
        }

        m_oldNitroCharge = c_newNitroCharge;

        if(m_oldNitroCharge == 100.0 && !m_wasNitroFull)
        {
            m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_nitroBar_ptr->GetID(), m_nitroBar_ptr->GetChargedBar(), true);
            m_wasNitroFull = true;
        }
        else if(m_wasNitroFull)
        {
            m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_nitroBar_ptr->GetID(), m_nitroBar_ptr->GetNormalBar(), true);
            m_wasNitroFull = false;            
        }
    }
}

void HUD::AddPowerupHUDElement(int c_xPos_i, int c_yPos_i, std::vector<std::string> c_textures, bool c_leftOrRight)
{
    if(c_leftOrRight)
    {
       m_leftPU_ptr = new PowerUpHUDElement(c_xPos_i, c_yPos_i, m_HUDElements_v.size(), c_textures);
       m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size(), c_xPos_i, c_yPos_i, m_leftPU_ptr->GetActiveTexture(), 100, 100, true);  
       m_HUDElements_v.emplace_back(m_leftPU_ptr);
    }
    else
    {
       m_rightPU_ptr = new PowerUpHUDElement(c_xPos_i, c_yPos_i, m_HUDElements_v.size(), c_textures);
       m_renderManager->GetRenderFacade()->GUICreateSprite(m_HUDElements_v.size(), c_xPos_i, c_yPos_i, m_rightPU_ptr->GetActiveTexture(), 100, 100, true);  
       m_HUDElements_v.emplace_back(m_rightPU_ptr);
    }   
}

void HUD::SetPowerUpTexture(int c_teturePos, bool leftorRight)
{
    if(leftorRight)
    {
        m_leftPU_ptr->SetActiveTexture(c_teturePos);
        m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_leftPU_ptr->GetID(), m_leftPU_ptr->GetActiveTexture(), true);
    }
    else
    {
        m_rightPU_ptr->SetActiveTexture(c_teturePos);
        m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_rightPU_ptr->GetID(), m_rightPU_ptr->GetActiveTexture(), true);
    }
    
}

void HUD::AddCountDownHUDElement(int c_xPos_i, int c_yPos_i, int c_width, int c_height)
{
    m_countDown_ptr = new CountDownHUDElement(c_xPos_i, c_yPos_i, m_HUDElements_v.size() + 3); 
    
    m_renderManager->GetRenderFacade()->GUICreateSprite(m_countDown_ptr->GetID(), c_xPos_i, c_yPos_i, m_countDown_ptr->UpdateCountDownTexture(2), c_width, c_height, true);
    m_renderManager->GetRenderFacade()->SetSpriteActiveStatus(m_countDown_ptr->GetID(), false, true);
    m_HUDElements_v.emplace_back(m_countDown_ptr);
}

void HUD::UpdateCountDownHUD(int c_countDown)
{
    if(c_countDown == -1)
        m_renderManager->GetRenderFacade()->SetSpriteActiveStatus(m_countDown_ptr->GetID(), false, true);
    else if(c_countDown == 2)
        m_renderManager->GetRenderFacade()->SetSpriteActiveStatus(m_countDown_ptr->GetID(), true, true);
    else
        m_renderManager->GetRenderFacade()->GUIChangeSpriteTexture(m_countDown_ptr->GetID(), m_countDown_ptr->UpdateCountDownTexture(c_countDown), true);

}
