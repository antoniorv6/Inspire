#include <DebugGUIManager.hpp>
#include <DebugConsoleWindowComponent.hpp>
#include <EntityManager.h>
#include <Entity.h>
#include <GenericData.hpp>

/**
 * @brief Construct a new Debug G U I Manager:: Debug G U I Manager object
 * 
 */
DebugGUIManager::DebugGUIManager()
{
    m_GUIDevice_ptr = nullptr;
}

/**
 * @brief Destroy the Debug G U I Manager:: Debug G U I Manager object
 * 
 */
DebugGUIManager::~DebugGUIManager()
{
    ClearList();
}

/**
 * @brief 
 * 
 * @param c_genericData 
 */
void DebugGUIManager::Init(GenericData *& c_genericData)
{
    m_entityManager_ptr = c_genericData->g_entityManager_ptr;
}

/**
 * @brief 
 * 
 */
void DebugGUIManager::ClearList()
{
    for(auto component : m_DConsoleComponents_vec)
    {
        delete component;
        component = nullptr;
    }

    m_DConsoleComponents_vec.clear();
}
 
/**
 * @brief 
 * 
 */
const void DebugGUIManager::initPointerValue(IrrIMGUI::IIMGUIHandle *&c_GUIDevice)
{
    m_GUIDevice_ptr = c_GUIDevice;
}

/**
 * @brief 
 * 
 * @param entity 
 * @param x 
 * @param y 
 * @param w 
 * @param h 
 */
void DebugGUIManager::CreateComponent(Entity* entity, int x, int y, int w, int h)
{
    DebugConsoleWindowComponent* dwcomponent = new DebugConsoleWindowComponent("DebugGUI", entity->GetId(), x , y, w, h);
    m_entityManager_ptr->AddComponent(entity->GetId(), dwcomponent);
    m_DConsoleComponents_vec.push_back(dwcomponent);
}

/**
 * @brief 
 * 
 * @param entity 
 * @param c_text 
 */
void DebugGUIManager::AddTextToInterface(Entity* entity, const std::string& c_text)
{
    DebugConsoleWindowComponent* debugWinComp = ((DebugConsoleWindowComponent*)m_entityManager_ptr->GetComponent(entity->GetId(), "DebugGUI"));
    debugWinComp->AddTextDataString(c_text);
}

/**
 * @brief 
 * 
 */
void DebugGUIManager::RenderDebugWindows()
{
    for(const auto& component : m_DConsoleComponents_vec)
    {
        component->RenderWindow();
    }

    ClearComponentWindows();
}

/**
 * @brief 
 * 
 */
void DebugGUIManager::ClearComponentWindows()
{
    for(const auto& component : m_DConsoleComponents_vec)
    {
        component->ClearWindow();
    }
}
