#include <WaypointComponent.hpp>

/**
 * @brief Construct a new Waypoint Component:: Waypoint Component object
 * 
 * @param l_componentId_s 
 * @param l_entityId_s 
 * @param l_position_v3 
 */
WaypointComponent::WaypointComponent(const std::string& l_componentId_s, int l_entityId_s, const glm::vec3& l_position_v3)
: IComponent(l_componentId_s, l_entityId_s)
{
    m_position_v3 = l_position_v3;
}

/**
 * @brief Destroy the Waypoint Component:: Waypoint Component object
 * 
 */
WaypointComponent::~WaypointComponent()
{
    
}

