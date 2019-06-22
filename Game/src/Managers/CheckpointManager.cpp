#include <CheckpointManager.hpp>
#include <AIManager.hpp>
#include <GenericData.hpp>

CheckpointManager::CheckpointManager()
{
   
}

void CheckpointManager::Init(GenericData *& c_genericData_ptr)
{
    m_entityManager_ptr     = c_genericData_ptr->g_entityManager_ptr;
    m_positionManager_ptr   = c_genericData_ptr->g_positionManager_ptr;
}

void CheckpointManager::CreateComponent(Entity* entity, int num, float vlimit, bool zone)
{
    CheckpointComponent* checkpoint = new CheckpointComponent("Checkpoint", entity->GetId(), num, vlimit, zone);
    components.push_back(checkpoint);
    m_entityManager_ptr->AddComponent(entity->GetId(), (IComponent*) checkpoint);
    m_positionManager_ptr->SetNumCheckpoints(components.size());
}

CheckpointManager::~CheckpointManager()
{
    ClearList();
}

void CheckpointManager::ClearList()
{
    for(unsigned int i = 0; i < components.size(); i++)
     {
        delete components[i];
        components[i] = nullptr;
     }
    
    components.clear();
}