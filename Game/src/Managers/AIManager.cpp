/**
 * @file AIManager.cpp
 * 
 * @brief 
 * @version 0.1
 * @date 2018-11-26
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "AIManager.hpp"
#include <WaypointManager.hpp>
#include <PowerUpManager.hpp>
#include <MapManager.hpp>
#include <Profiler.h>

void LODBehavior(AIDecisionComponent* l_component, AIManager* manager);
void DecisionBehavior(AIDecisionComponent* l_component, AIManager* manager);

/**
 * 
 * @brief 
 * 
 */
AIManager::AIManager()
{
    
}


/**
 * @brief Destroy the AIManager::AIManager object
 * 
 */
AIManager::~AIManager()
{
    ClearList();
}

void AIManager::Init(GenericData *&c_genericData)
{
    m_entityManager_ptr   = c_genericData->g_entityManager_ptr; 
    m_physicsManager_ptr  = c_genericData->g_physicsManager_ptr;
    m_waypointManager_ptr = c_genericData->g_waypointManager_ptr;
    m_powerUpManager_ptr  = c_genericData->g_powerUpManager_ptr;
    m_mapManager_ptr      = c_genericData->g_mapManager_ptr;

    m_genericData=c_genericData;
}

void AIManager::InitData()
{
    m_behavior_clk = new Clock();
    m_scheduling_clk = new Clock();

    m_behavior_clk->Init();
    m_scheduling_clk->Init();

    m_timeDecision_d = 0.00015;
    m_timeLOD_d = 0.000005;
    
}

void AIManager::ClearList()
{
     for(unsigned int i = 0; i < m_DecisionComponentsVector_vo.size(); i++)
     {
        delete m_DecisionComponentsVector_vo[i];
        m_DecisionComponentsVector_vo[i] = nullptr;
     }

    m_DecisionComponentsVector_vo.clear();
    
    ClearAIQueue();
    
    if(m_behavior_clk && m_scheduling_clk)
    {
        delete m_behavior_clk;
        m_behavior_clk = nullptr;
        delete m_scheduling_clk;
        m_scheduling_clk = nullptr;
    }

    WriteValues();
}



void AIManager::CreateDecisionComponent(Entity* entity, float l_force, int personality){
    AIDecisionComponent* dec = new AIDecisionComponent("AIDecision", entity->GetId(), m_waypointManager_ptr->GetWaypointsVector(), l_force, personality, m_genericData);
    m_DecisionComponentsVector_vo.emplace_back(dec);
    m_entityManager_ptr->AddComponent(entity->GetId(), dec);
}

/**
 * 
 * 
 * @brief 
 * 
 */
void AIManager::Update()
{
    m_expectedTime_d = 0.0;

    for(unsigned int i = 0; i < m_DecisionComponentsVector_vo.size(); i++)
    {
        if(m_DecisionComponentsVector_vo[i])
        {
            if(m_scheduling_b)
            {
                //Create AI Behavior Event
                AIBehaviorEvent event;
                event.component = m_DecisionComponentsVector_vo[i];
                event.function  = DeterminateBehavior(m_DecisionComponentsVector_vo[i]->GetEntityId());

                //Add to queue
                m_AIBehavior_queue.push(event);

                //Increment expected time with value for this behavior
                m_expectedTime_d += m_timeDecision_d;
            }
            else
            {
                m_DecisionComponentsVector_vo[i]->Update();
            }
        }
    }
}

Behavior AIManager::DeterminateBehavior(int l_entityID_n)
{
    PositionComponent* l_AI_Position_ptr = (PositionComponent*) m_entityManager_ptr->GetComponent(l_entityID_n, "Position");
    int l_AI_checkpoint_n = l_AI_Position_ptr->GetCheckpoint();
    int l_AI_lap_n        = l_AI_Position_ptr->GetLap();

    //Calculate the distance between IA and Player
    int l_difCheck_n    = *m_player_checkpoint_ptr - l_AI_checkpoint_n;

    //Check if both are in the same lap
    int l_difLap_n      = *m_player_lap_ptr - l_AI_lap_n;
    if(l_difLap_n != 0)
    {
        if(l_difLap_n > 0)
        {
            l_difCheck_n = (*m_player_checkpoint_ptr - (l_difLap_n * m_num_checkpoint_n)) - l_AI_checkpoint_n;
        }
        else
        {
            l_difCheck_n = *m_player_checkpoint_ptr - (l_AI_checkpoint_n - (-l_difLap_n * m_num_checkpoint_n));
        }
    }

    //std::cout << "CheckP: " << *m_player_checkpoint_ptr << " AI " << l_entityID_n << ": " << l_AI_checkpoint_n << std::endl;  

    if( l_difCheck_n > m_BHNDdistanceLOD || l_difCheck_n < -m_FWDdistanceLOD)
    {
        ((RigidBodyComponent*)m_entityManager_ptr->GetComponent(l_entityID_n, "RigidBody"))->setCollision(false);
        return LODBehavior;
    }

    ((RigidBodyComponent*)m_entityManager_ptr->GetComponent(l_entityID_n, "RigidBody"))->setCollision(true);
    return DecisionBehavior;
}

void AIManager::Scheduling(float frameTime)
{
    double l_timeSpent_d = 0.0;
    //calculate time that we can spent in this frame
    // ==> expected time to process all queue's behavior / (time between updates / time for 1 frame)
    double l_timeCalculated_d = m_expectedTime_d / (loopTime / frameTime); 

    while(!m_AIBehavior_queue.empty())
    {
        //Restart Time
        m_scheduling_clk->Restart();

        if(l_timeSpent_d < l_timeCalculated_d)
        {
            //Get First event in the queue
            AIBehaviorEvent event = m_AIBehavior_queue.front();
            //Restart behavior clock
            m_behavior_clk->Restart();
            //Call the event's function
            event.function(event.component, this);

            //Delete behavior from the queue
            m_AIBehavior_queue.pop();
            //Add time
            l_timeSpent_d += m_scheduling_clk->GetElapsedTime();
        }
        else
        {
            break;
        }  
    }
    //WriteValues(frameTime, l_timeSpent_d, l_timeCalculated_d);
}

void LODBehavior(AIDecisionComponent* l_component, AIManager* manager)
{
    //PROFILE_SCOPED();

    //Update AI
    l_component->LODUpdate();

    //Update average time 
    manager->UpdateAverageTime(&manager->m_timeLOD_d, &manager->m_numLOD_n);

    //Add cont
    manager->m_numLOD_n++;
}
void DecisionBehavior(AIDecisionComponent* l_component, AIManager* manager)
{
    //PROFILE_SCOPED();

    //Update AI
    l_component->Update();

    //Update average time 
    manager->UpdateAverageTime(&manager->m_timeDecision_d, &manager->m_numDecision_n);

    //Add cont
    manager->m_numDecision_n++;
}

void AIManager::UpdateAverageTime(double* time, int* num)
{
    //Calculate average time 4 this behavior type
    *time = (*num > 1000) ? *time : (*time * (*num) + m_behavior_clk->GetElapsedTime()) / (*num + 1);
}

void AIManager::RestartAIClock(int id)
{
    for(unsigned int i = 0; i < m_DecisionComponentsVector_vo.size(); i++)
    {

        if(m_DecisionComponentsVector_vo[i]->GetEntityId() == id)
        {
            m_DecisionComponentsVector_vo[i]->RestartClock();
            return;
        }

    }
}


void AIManager::WriteValues(float frameTime, double l_timeSpent_d, double l_timeCalculated_d)
{
    // system("clear");
    // std::cout << "===================="                                         << std::endl;
    // std::cout << "Behavior Queue size after: "  << m_AIBehavior_queue.size()    << std::endl;
    // std::cout << "Scheduling Expected Time: "   << m_expectedTime_d             << std::endl;
    // std::cout << "Time btween updates: "        << loopTime                     << std::endl;
    // std::cout << "Time for 1 frame: "           << frameTime                    << std::endl;
    // std::cout << "Frames btween updates: "      << loopTime / frameTime         << std::endl;
    // std::cout << "Time Calculated: "            << l_timeCalculated_d           << std::endl;
    // std::cout << "Time Spent: "                 << l_timeSpent_d                << std::endl;
    // std::cout << "===================="                                         << std::endl;
    // std::cout << "Time Decision: "              << m_timeDecision_d             << std::endl;
    // std::cout << "Num Decision: "               << m_numDecision_n             << std::endl;
    // std::cout << "Time LOD: "                   << m_timeLOD_d                  << std::endl;
    // std::cout << "Num LOD: "                    << m_numLOD_n                  << std::endl;
    // std::cout << "===================="                                         << std::endl;
}

void AIManager::WriteValues()
{
    // system("clear");
    // std::cout << "===================="                                         << std::endl;
    // std::cout << "Time Decision: "              << m_timeDecision_d             << std::endl;
    // std::cout << "Num Decision: "               << m_numDecision_n             << std::endl;
    // std::cout << "Time LOD: "                   << m_timeLOD_d                  << std::endl;
    // std::cout << "Num LOD: "                    << m_numLOD_n                  << std::endl;
    // std::cout << "===================="                                         << std::endl;
}
