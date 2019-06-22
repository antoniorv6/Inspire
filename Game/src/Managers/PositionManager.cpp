
#include <PositionManager.hpp>
#include <PhysicsManager.h>
#include <SoundManager.h>
#include <CheckpointManager.hpp>
#include <AIManager.hpp>
#include <GenericData.hpp>
#include <RenderManager.h>
#include <functional>
#include <HUD.hpp>

PositionManager::PositionManager(){
}

 PositionManager::~PositionManager()
 {
    ClearList();
    m_positionComponent_v.clear();
 }

void PositionManager::Init(GenericData *& c_genericData)
{
    m_entityManager_ptr     = c_genericData->g_entityManager_ptr;
    m_physicsManager_ptr    = c_genericData->g_physicsManager_ptr;
    m_checkpointManager_ptr = c_genericData->g_checkPointManager_ptr;
    m_debugGUIManager_ptr   = c_genericData->g_debugGuiManager_ptr;
    m_soundManager_ptr      = c_genericData->g_soundManager_ptr;
    m_aiManager_ptr         = c_genericData->g_aiManager_ptr;
    m_renderManager_ptr     = c_genericData->g_renderManager_ptr;

    m_genericData_ptr       = c_genericData;

    m_positionComponent_v.reserve(4);
    m_finalResults_v.reserve(4);
    m_finalResults_v.clear();
    m_playerPosition_i = 4;
}

void PositionManager::InitHUD(HUD*& c_HUD_ptr)
{   
    m_HUD_ptr = c_HUD_ptr;
    m_HUD_ptr->AddLapHUDElement(120, 105, 150, 150, 0);
    m_HUD_ptr->AddPositionHUDElement(120, 600, 150, 150, 3);
}


void PositionManager::CreateComponent(Entity* entity)
{
    PositionComponent* position = new PositionComponent("Position", entity->GetId(), m_genericData_ptr);
    m_positionComponent_v.push_back(position);
    m_entityManager_ptr->AddComponent(entity->GetId(), (IComponent*) position);

    if(entity->GetId() == 0)
    {
        m_player_ptr = entity;
        m_aiManager_ptr->SetPlayerCheckpoint(position->GetCheckpointPTR());
        m_aiManager_ptr->SetPlayerLap(position->GetLapPTR());
    }
    else
    {
        //CREATE BILLBOARD 
        m_numBillboards++;
        glm::vec3 init_pos = {0,0,-100000};
        m_entityBillboard_v.emplace_back(entity);
        std::string texture = entity->GetName() + ".png";
        m_renderManager_ptr->GetRenderFacade()->CreateBillboard(m_numBillboards, texture, 100.0f, 50.0f, init_pos);
    }
    
}

Entity* PositionManager::GetDrone(int position)
{
    return m_entityManager_ptr->GetEntity(m_positionComponent_v[position-1]->GetEntityId());
}

bool SortPositions(PositionComponent* l_pos, PositionComponent* r_pos, PositionManager *& c_manager)
{
    if(l_pos->checkpoint != r_pos->checkpoint)
        return l_pos->checkpoint > r_pos->checkpoint;   
    else
    {
        //CALCULATE DISTANCE TO THE NEXT CHECKPOINT
        if(!l_pos->dist_calc)
            c_manager->DistanceToNextCheckpoint(l_pos);
        if(!r_pos->dist_calc)
            c_manager->DistanceToNextCheckpoint(r_pos);

        return l_pos->distance < r_pos->distance;
    }
}

void PositionManager::DistanceToNextCheckpoint(PositionComponent* component)
{
    Entity* drone = m_entityManager_ptr->GetEntity(component->GetEntityId());

    int id = m_checkpointManager_ptr->GetComponent(component->next_checkpoint)->GetEntityId();
    
    component->distance = m_physicsManager_ptr->DistanceToNextCheckpoint((RigidBodyComponent*)m_entityManager_ptr->GetComponent(id, "RigidBody"), drone);

    component->dist_calc = true;
} 

void PositionManager::Update()
{
    using namespace std::placeholders;
    std::sort(m_positionComponent_v.begin(),m_positionComponent_v.end(), std::bind(SortPositions, _1, _2, this));
    for(int i=0; i < (int)m_positionComponent_v.size(); i++)
    {   
        m_positionComponent_v[i]->position = i+1;

        if(m_positionComponent_v.size() > 1)
        {
            int next_position = CalculateNextPosition(m_positionComponent_v[i]);

            m_positionComponent_v[i]->nearby_rivals = m_physicsManager_ptr->CalculateNearRivals(m_positionComponent_v[i]->GetEntityId(), 1300.0f);
            m_positionComponent_v[i]->distance_next_rival = glm::distance(m_positionComponent_v[i]->m_physicsData_ptr->currentPosition, GetDrone(next_position)->getPos());
            m_positionComponent_v[i]->race_progress = m_positionComponent_v[i]->checkpoint / (m_numCheckpoints_n * m_positionComponent_v[i]->NUM_LAPS);
        }
        
        m_positionComponent_v[i]->dist_calc = false;
        
        //Player Update
        if(m_positionComponent_v[i]->GetEntityId() == m_player_ptr->GetId())
        {
            if(m_playerPosition_i != m_positionComponent_v[i]->position)
            {
                if(m_positionComponent_v[i]->lap <= m_positionComponent_v[i]->NUM_LAPS)
                {
                    m_HUD_ptr->UpdatePosition(m_positionComponent_v[i]->position - 1);
                    m_playerPosition_i = m_positionComponent_v[i]->position;
                }
            }
        }
    }
}

void PositionManager::OnlineUpdate()
{
    using namespace std::placeholders;
    std::sort(m_positionComponent_v.begin(),m_positionComponent_v.end(), std::bind(SortPositions, _1, _2, this));

    for(int i=0; i < (int)m_positionComponent_v.size(); i++)
    {   
        m_positionComponent_v[i]->position = i+1;
        m_positionComponent_v[i]->dist_calc = false;

        //int next_position = CalculateNextPosition(m_positionComponent_v[i]);
        //m_positionComponent_v[i]->nearby_rivals = m_physicsManager_ptr->CalculateNearRivals(m_positionComponent_v[i]->GetEntityId());
        //m_positionComponent_v[i]->distance_next_rival = glm::distance(m_positionComponent_v[i]->m_physicsData_ptr->currentPosition, GetDrone(next_position)->getPos());
        //m_positionComponent_v[i]->race_progress = m_positionComponent_v[i]->checkpoint / (m_numCheckpoints_n * m_positionComponent_v[i]->NUM_LAPS);

    }
}

void PositionManager::ClearList()
{
    for(unsigned int i = 0; i < m_positionComponent_v.size(); i++)
    {
        delete m_positionComponent_v[i];
        m_positionComponent_v[i] = nullptr;
    }
    m_positionComponent_v.clear();
    
    m_entityBillboard_v.clear();
    m_numBillboards = 0;
}

void PositionManager::BillboardsUpdate()
{
    for(unsigned int i=0; i < m_entityBillboard_v.size(); i++)
    {
        glm::vec3 drone_pos = m_entityBillboard_v[i]->getPos();
        drone_pos.z += 35.0f;
        m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(i+1, drone_pos);
    }
}

void PositionManager::AddCheckpoint(int id_entity){
    PositionComponent* l_position_ptr = ((PositionComponent*)m_entityManager_ptr->GetComponent(id_entity,"Position"));


    if(l_position_ptr->next_checkpoint == 0 && l_position_ptr->checkpoint > 0)
    {
        AddLap(id_entity, l_position_ptr);
    }

    l_position_ptr->AddCheckpoint(m_numCheckpoints_n);


    // if(id_entity == m_player_ptr->GetId())
    // {
    //     //std::cout << "Checkpoints Passed: " << l_position_ptr->checkpoint << "  Next Checkpoint: " << l_position_ptr->next_checkpoint << "\n";
    // }
}

void PositionManager::AddLap(int id_entity, PositionComponent* l_position_ptr)
{
    l_position_ptr->lap++;
    
    // FINAL OF THE RACE
    if(l_position_ptr->lap == l_position_ptr->NUM_LAPS+1)
    {    
        Add2FinalResults(m_entityManager_ptr->GetEntity(id_entity)->GetName());
     
        //FINAL DE LA CARRERA
        if(id_entity == m_player_ptr->GetId() || m_finalResults_v.size() > 2)
        {
            m_soundManager_ptr->SetEventVariableValue(m_player_ptr,"Music", "Lap", l_position_ptr->lap - 1);
            EndGame();
        }

    }

    // NEXT LAP
    else
    {
        if(id_entity == m_player_ptr->GetId())
        {
            //std::cout << "LAP---> " << l_position_ptr->lap << std::endl;
            m_soundManager_ptr->SetEventVariableValue(m_player_ptr,"Music", "Lap", l_position_ptr->lap - 1);
            m_HUD_ptr->UpdateLAP(l_position_ptr->lap-1);
        }
    }
         
}

void PositionManager::Add2FinalResults(std::string name)
{
    bool already = false;
    for(unsigned int i=0; i<m_finalResults_v.size(); i++)
    {
        if(m_finalResults_v[i] == name)
        {
            already = true;
            break;
        }
    }

    if(!already)
    {
        m_finalResults_v.emplace_back(name);
        //std::cout << m_finalResults_v.size() << "º: " << name << std::endl;
    }
}

void PositionManager::EndGame()
{
    while(m_finalResults_v.size() < m_positionComponent_v.size())
    {
        for(unsigned int i=0; i<m_positionComponent_v.size(); i++)
        {
            if(m_positionComponent_v[i]->lap < m_positionComponent_v[i]->NUM_LAPS+1)
            {
                Add2FinalResults(m_entityManager_ptr->GetEntity(m_positionComponent_v[i]->GetEntityId())->GetName());
            }
        }
    }

    isPlay = false;
    PICASSO::PhysicsVariables* data = ((TransformComponent*) m_entityManager_ptr->GetComponent(m_player_ptr->GetId(), "Physics"))->GetPhysicsData();
    PICASSO::Keys2False(data);
}

int PositionManager::CalculateNextPosition(PositionComponent* l_component)
{
    int next_position = l_component->position - 1;
    if(l_component->position == 1)
        next_position = l_component->position + 1;

    return next_position;
}

PositionComponent* PositionManager::GetPlayerPosition()
{
    return (PositionComponent*)m_entityManager_ptr->GetComponent(m_player_ptr->GetId(), "Position");
}

void PositionManager::SetNumCheckpoints(int value)
{ 
    m_numCheckpoints_n = value; 
    m_aiManager_ptr->SetNumCheckpoint(value);
}