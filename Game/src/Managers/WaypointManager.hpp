#pragma once
#include <glm.hpp>
#include <vector>
#include "WaypointComponent.hpp"

#include "PhysicsManager.h"
#include "EntityManager.h"

class RenderManager;
class GenericData;
class WaypointManager{
    public:
        WaypointManager();
        ~WaypointManager();

        void Init(GenericData *&c_genericData);
        void Update();
        void ClearList();
        void CreateWaypointComponent(Entity* l_entity_o);
        void CreatePath();
        void CreateWayPoint(const std::string& l_index_s, const glm::vec3& l_position_v3);
        
        const std::vector<WaypointComponent*>& GetWaypointsVector() const;

    private:
        std::vector<WaypointComponent*> m_waypointsVector_vo;
        EntityManager* m_entityManager_ptr = nullptr;
        RenderManager* m_renderManager_ptr = nullptr;
};