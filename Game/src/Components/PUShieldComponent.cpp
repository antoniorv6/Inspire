#include <PUShieldComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>

PUShieldComponent::PUShieldComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_SHIELD, TYPE_VALUE_SHIELD, l_genericData_ptr)
{
    m_sprites_vec.reserve(5);
    m_sprites_vec.emplace_back("shield0_disabled.png");
    m_sprites_vec.emplace_back("shield1_ready.png");
    m_sprites_vec.emplace_back("shield2_one.png");
    m_sprites_vec.emplace_back("shield3_two.png");
    m_sprites_vec.emplace_back("shield4_three.png");
}

PUShieldComponent::~PUShieldComponent()
{
    delete m_clock_ptr;
    m_clock_ptr = nullptr;
    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUShieldComponent::SetActive(bool value)
{ 
    m_active = value;

    if(!m_active)
    {
        ChangeSprite(DISABLED);
    }
};

void PUShieldComponent::UpdateSprite()
{
    if(m_active)
    {
        ChangeSprite((PowerUpSprites)(m_lifes_n + 1));
    }
    else
    {
        if(CheckIfPowerUpReady(false))
        {
            ChangeSprite(READY);
        }
        else
        {
            ChangeSprite(DISABLED);
        }
    }
}

void PUShieldComponent::ActivatePowerUp()
{
    if(CheckIfPowerUpReady(true))
    {
        if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
        {
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 0.0);
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
            m_soundManager_ptr->Play(m_drone, "PowerUP");
        }
        //Activate the powerup component
        SetActive(true);
        m_powerUpManager_ptr->m_activeShields_v.emplace_back(this);
        //std::cout << "########SHIELD ACTIVATED#########\n" << std::endl;
        ResetLifes();

        //Initialize Clock
        m_clock_ptr->Init();

    } 
    else
    {
        //std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
    }
}


void PUShieldComponent::DecLifes()
{ 
    if(m_active)
        m_lifes_n--;
    
    //std::cout << "vidas escudo: " << GetLifes() << std::endl;

    if(m_lifes_n == 0)
        SetActive(false);
};