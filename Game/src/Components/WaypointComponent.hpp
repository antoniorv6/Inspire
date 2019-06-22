#pragma once
#include <IComponent.h>
#include <glm.hpp>

class WaypointComponent : public IComponent
{
public:
    WaypointComponent(const std::string& l_componentId_s, int l_entityId_s, const glm::vec3& l_position_v3);
    ~WaypointComponent();

    glm::vec3 GetPosition() { return m_position_v3; };
private:
    glm::vec3 m_position_v3 = glm::vec3(0.0f);
};