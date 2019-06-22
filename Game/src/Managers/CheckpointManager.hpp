#pragma once

#include <CheckpointComponent.hpp>
#include <PositionManager.hpp>
#include "IComponent.h"
#include "EntityManager.h"
#include "Entity.h"

#include <vector>
#include <iostream>

class PositionManager;
class AIManager;
class GenericData;

class CheckpointManager{
    public:
        CheckpointManager();
        ~CheckpointManager();
        void Init(GenericData *& c_genericData_ptr);
        void CreateComponent(Entity*, int, float, bool);
        void ClearList();
        CheckpointComponent* GetComponent(int pos){ return components[pos]; };
        int GetNumCheckpoints() { return components.size(); };

    private:
        std::vector<CheckpointComponent*> components;
        EntityManager* m_entityManager_ptr     = nullptr;
        PositionManager* m_positionManager_ptr = nullptr;

};