#pragma once

#include <glm.hpp>
#include <PowerUpComponent.hpp>

class PUFreezingComponent : public PowerUpComponent{
    public:
        PUFreezingComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr);
        virtual ~PUFreezingComponent();
        float GetRadius() const { return m_RADIUS; };

        virtual void ActivatePowerUp();
        virtual void SetActive(bool value);
        virtual void UpdateSprite();
        
    private:
        /*POWER UP DATA*/
        const float m_RADIUS = 1000.0f;        
};