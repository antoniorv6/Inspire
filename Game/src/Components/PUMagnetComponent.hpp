#pragma once

#include <glm.hpp>
#include <PowerUpComponent.hpp>


class PUMagnetComponent : public PowerUpComponent{
    public:
        PUMagnetComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr);
        virtual ~PUMagnetComponent();

        float GetDistance() const { return m_DISTANCE; };
        
        void UpdateRival();
        Entity* GetRival()  const { return m_rival_drone; };

        virtual void ActivatePowerUp();
        virtual void SetActive(bool value);
        virtual void UpdateSprite();
        void Magnetize();
        
    private:
        /*POWER UP DATA*/
        const float m_DISTANCE = 1700.0f;
        Entity* m_rival_drone = nullptr; 
};