/**
 * @file WaypointManager.cpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2018-11-22
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "WaypointManager.hpp"
#include "RenderManager.h"
#include <GenericData.hpp>

WaypointManager::WaypointManager(){
    
}


/**
 * @brief Destroy the Waypoint Manager:: Waypoint Manager object
 * 
 */
WaypointManager::~WaypointManager()
{
     ClearList();
}

void WaypointManager::ClearList()
{
    for(unsigned int i = 0; i < m_waypointsVector_vo.size(); i++)
     {
        delete m_waypointsVector_vo[i];
        m_waypointsVector_vo[i] = nullptr;
     }
    
    m_waypointsVector_vo.clear();

}

/**
 * WaypointManager::CreateWaypointComponent
 * 
 * @brief 
 * 
 * @param l_entity_o 
 */
void WaypointManager::CreateWaypointComponent(Entity* l_entity_o)
{
    WaypointComponent* waypoint = new WaypointComponent("Waypoint", l_entity_o->GetId(), l_entity_o->getPos());
    m_entityManager_ptr->AddComponent(l_entity_o->GetId(), (IComponent*) waypoint);
    m_waypointsVector_vo.push_back(waypoint);
}

/**
 * WaypointManager::Init
 * 
 * @brief
 * 
 */
void WaypointManager::Init(GenericData *&c_genericData)
{
    m_entityManager_ptr = c_genericData->g_entityManager_ptr;
    m_renderManager_ptr = c_genericData->g_renderManager_ptr;
    //Generate the waypoint fields
    //CreatePath();

}

/**
 * WaypointManager::CreatePath
 * 
 * @brief 
 * 
 */
void WaypointManager::CreatePath(){

    /** Para crear aqui el path, podemos hacer uso del event manager 
    *   y enviar un mensaje al Path Planning para que lo cree.
    */
   
    // CreateWayPoint("Waypoint0", glm::vec3(250, 0, -10));
    // CreateWayPoint("Waypoint1", glm::vec3(500, 0, -10));
    // CreateWayPoint("Waypoin2", glm::vec3(500, 700, -10));
    // CreateWayPoint("Waypoint3", glm::vec3(0, 800, -10));
    // CreateWayPoint("Waypoint4", glm::vec3(0, 0, -10));

    // CreateWayPoint("Waypoint0", glm::vec3(250, 0, 0));
    // CreateWayPoint("Waypoint1", glm::vec3(500, 0, 0));
    // CreateWayPoint("Waypoin2", glm::vec3(500, 500, 0));
    // CreateWayPoint("Waypoint3", glm::vec3(0, 500, 0));
    // CreateWayPoint("Waypoint4", glm::vec3(0, 0, 0));

}

/**
 * WaypointManager::CreateWaypoint
 * 
 * @brief 
 * 
 * @param l_index_s 
 * @param l_position_v3 
 */
void WaypointManager::CreateWayPoint(const std::string& l_index_s, const glm::vec3& l_position_v3)
{
    Entity* waypoint= m_entityManager_ptr->CreateEntity(l_index_s, l_position_v3);
    m_renderManager_ptr->CreateComponent(waypoint, "Waypoint", "");
    this->CreateWaypointComponent(waypoint);
    m_renderManager_ptr->AddNode(waypoint);
    m_renderManager_ptr->GetRenderFacade()->ScaleSceneNode(waypoint->GetId(), 0.3);
}

/**
 * WaypointManager::GetWaypointsVector 
 * 
 * @brief 
 * 
 * @return std::vector<Entity*> 
 */
const std::vector<WaypointComponent*>& WaypointManager::GetWaypointsVector() const
{
    return m_waypointsVector_vo;
}