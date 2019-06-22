#include <GUIManager.hpp>
#include <RenderManager.h>
#include <GenericData.hpp>

GUIManager::GUIManager()
{
}

GUIManager::~GUIManager()
{
    ClearList();
}

void GUIManager::Init(GenericData *& c_genericData)
{
    m_entityManager_ptr = c_genericData->g_entityManager_ptr;
    m_renderManager_ptr = c_genericData->g_renderManager_ptr;
}
