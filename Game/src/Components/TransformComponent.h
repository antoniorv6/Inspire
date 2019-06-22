#pragma once

#include <glm.hpp>
#include <PICASSO.hpp>
#include <IComponent.h>

class PhysicsManager;
class Event;

struct PowerUpData{
    IComponent* m_powerUp_ptr;
    TypePU m_typePowerUp;
};

class TransformComponent : public IComponent{
    friend void NitroCharge(Event nitro);
    friend class PhysicsManager;
    public:
        TransformComponent(glm::vec3* c_position, const std::string& id, int id_ent) : IComponent(id, id_ent){
            physicsDataPtr = &physicsData;
            position = c_position;
        }
        ~TransformComponent(){};
        PICASSO::PhysicsVariables* GetPhysicsData(){ return physicsDataPtr; };
        bool SetPowerUp(IComponent* component, TypePU typePowerUp) 
        { 
            if(m_powerUp_v.size() < 2){
                m_powerUp_v.emplace_back(PowerUpData {component, typePowerUp});
                return true;
            }
            else    
                return false;
        };
        PowerUpData* GetPowerUp(int l_numberPowerUp_n){ return &m_powerUp_v[l_numberPowerUp_n]; }; 
        bool CheckPowerUp(int l_numberPowerUp_n, TypePU l_typePU)
        {
            if(((int)m_powerUp_v.size()) > l_numberPowerUp_n)
                return m_powerUp_v[l_numberPowerUp_n].m_typePowerUp == l_typePU;
            else
                return false; 
        };
        
    private:
        PICASSO::PhysicsVariables physicsData;
        PICASSO::PhysicsVariables* physicsDataPtr = nullptr;
        glm::vec3* position = nullptr;

        /*PowerUps*/
        std::vector <PowerUpData> m_powerUp_v;
};