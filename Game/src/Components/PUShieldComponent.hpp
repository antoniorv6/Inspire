#pragma once

#include <glm.hpp>
#include <PowerUpComponent.hpp>


class PUShieldComponent : public PowerUpComponent{
    public:
        PUShieldComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr);
        virtual ~PUShieldComponent();

        int GetLifes()    const { return m_lifes_n; };
        float GetRadius() const { return m_RADIUS; };
        float GetTime()   const { return m_TIME; };
        Clock* GetClock() const { return m_clock_ptr; };

        void DecLifes();
        void ResetLifes(){ m_lifes_n = 3; };

        virtual void ActivatePowerUp();
        virtual void SetActive(bool value);
        virtual void UpdateSprite();
        
    private:

        /*POWER UP DATA*/
        int m_lifes_n = 0;
        const float m_RADIUS = 6.0f;
        const float m_TIME = 6.0f; //seconds of shields life
        int m_shader_cont = -1;
        Clock* m_clock_ptr = new Clock();
        
};