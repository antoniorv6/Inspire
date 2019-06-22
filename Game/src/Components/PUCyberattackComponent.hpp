#pragma once

#include <glm.hpp>
#include <PowerUpComponent.hpp>

class PUCyberattackComponent : public PowerUpComponent{
    public:
        PUCyberattackComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr);
        virtual ~PUCyberattackComponent();
        
        float GetTime()   const { return m_EFFECT_TIME; };
        Clock* GetClock() const { return m_clock_ptr; };

        virtual void ActivatePowerUp();
        virtual void SetActive(bool value);
        virtual void UpdateSprite();

        void AddHackedPU(PowerUpComponent* component);
        void ClearVector();

        void DisableRivalsPowerUps();

    private:
        /*POWER UP DATA*/
        const float m_EFFECT_TIME = 8.0f; //seconds
        Clock* m_clock_ptr = new Clock();
        std::vector<PowerUpComponent*> m_hackedPU_v; //list of all power ups hacked by this cyberattack

};