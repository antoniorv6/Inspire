#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Entity.h"
#include "IComponent.h"

class EntityManager{
    public:
        EntityManager();
        ~EntityManager();

        void ClearList();
        void AddComponent(int EntityID, IComponent* Component);
        IComponent* GetComponent(int EntityID, const std::string& ComponentID);
        Entity* CreateEntity(const std::string& name, glm::vec3 pos);
        void DeleteEntity(int id);
        Entity* GetEntity(int id) const;
        Entity* GetEntityByName(const std::string& EntityName) const;
        unsigned int GetNumEntities() const { return entities.size(); };

        std::vector<Entity*> entities;
};