#pragma once

#include <list>
#include <string>
#include <glm.hpp>
#include <IComponent.h>

class Entity {
friend class PhysicsManager;

public:
    struct EntityData{
        glm::vec3 pos;
        glm::vec3 rot;
    };


    Entity(){
        id = -1;
    };
    Entity(std::string e_name, glm::vec3 position, int e_id)
    {
        name = e_name;
        id = e_id;
        m_NowED.pos=position;
        m_AfterED.pos=position;
        m_NowED.rot = glm::vec3(0,0,0);
        m_AfterED.rot = glm::vec3(0,0,0);
    };
    ~Entity(){
    };

    void AddComponent(IComponent* Component){ components.push_back(Component); };
    std::list<IComponent*> GetComponentList(){ return components; };
    std::string GetName(){ return name; };
    int GetId(){ return id; };
    
    glm::vec3 getPos(){ return m_NowED.pos; };
    glm::vec3* getPosPtr(){ return &m_NowED.pos; };
    void setPos(glm::vec3 position){ m_NowED.pos = position; };

    glm::vec3 getRot(){ return m_NowED.rot; };
    glm::vec3* getRotPtr(){ return &m_NowED.rot; };
    void setRot(glm::vec3 rotation){ m_NowED.rot = rotation; };
    void setRotZ(float rotZ){ m_NowED.rot.z = rotZ; };

    EntityData* getBeforeED(){ return &m_BeforeED; };
    void setBeforeED(EntityData data){ m_BeforeED=data; };
    void setBeforeED(glm::vec3 pos, glm::vec3 rot){ m_BeforeED.pos=pos;  m_BeforeED.rot=rot;};

    EntityData* getAfterED(){ return &m_AfterED; };
    void setAfterED(EntityData data){ m_AfterED=data; };
    void setAfterED(glm::vec3 pos, glm::vec3 rot){ m_AfterED.pos=pos;  m_AfterED.rot=rot;};
    
private:
    int id;
    std::string name;
    std::list<IComponent*> components;

    /*TRANSFORMATION DATA OF THE ENTITY*/
    EntityData m_NowED;
    EntityData m_BeforeED;
    EntityData m_AfterED;

};