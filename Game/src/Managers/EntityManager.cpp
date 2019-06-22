#include "EntityManager.h"

#include <iostream>

EntityManager::EntityManager(){
    entities.reserve(200);
}

Entity* EntityManager::CreateEntity(const std::string& name, glm::vec3 pos){
    int id = entities.size();
    Entity* entity = new Entity(name, pos, id);
    entities.push_back(entity);
    return entity;
}

IComponent* EntityManager::GetComponent(int EntityID, const std::string& ComponentID){

    std::list<IComponent*> components = GetEntity(EntityID)->GetComponentList();
    
    for(auto const& j : components){
    
        if(j->GetComponent() == ComponentID){
    
            return j;
    
        }
    
    }


    return nullptr;
}

void EntityManager::AddComponent(int EntityID, IComponent* Component){
    GetEntity(EntityID)->AddComponent(Component);
}

EntityManager::~EntityManager()
{
    ClearList();
}

void EntityManager::ClearList()
{
    for(unsigned int i = 0; i < entities.size(); i++)
     {
        delete entities[i];
        entities[i] = nullptr;
     }
     entities.clear();
}

Entity* EntityManager::GetEntityByName(const std::string& EntityName) const {

    for (auto const& i : entities) {
        if(i->GetName() == EntityName){
            return i;
        }
    }

    return nullptr;
}

Entity* EntityManager::GetEntity(int id) const {

    if(entities[id]!=nullptr){
        return entities[id];
    }
    return nullptr;
}

