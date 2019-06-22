#pragma once

#include <glm.hpp>
#include <PowerUpComponent.hpp>


class PUReflectorComponent : public PowerUpComponent{
    public:
        PUReflectorComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr);
        virtual ~PUReflectorComponent();
        float GetTime()   const { return m_TIME; };
        Clock* GetClock() const { return m_clock_ptr; };

        virtual void ActivatePowerUp();
        virtual void SetActive(bool value);
        virtual void UpdateSprite();
        
    private:

        /*POWER UP DATA*/
        const float m_TIME = 11; //seconds of shields life
        Clock* m_clock_ptr = new Clock();
        
};