#include <PUReflectorComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>


PUReflectorComponent::PUReflectorComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_REFLECTOR, TYPE_VALUE_REFLECTOR, l_genericData_ptr)
{
    m_sprites_vec.reserve(3);
    m_sprites_vec.emplace_back("mirror0_disabled.png");
    m_sprites_vec.emplace_back("mirror1_ready.png");
    m_sprites_vec.emplace_back("mirror2_being_used.png");
};

PUReflectorComponent::~PUReflectorComponent()
{
    delete m_clock_ptr;
    m_clock_ptr = nullptr;
    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUReflectorComponent::SetActive(bool value)
{ 
    m_active = value;

    if(m_active)
        ChangeSprite(BEING_USED); 
    else
    {
        ChangeSprite(DISABLED);
    }
};

void PUReflectorComponent::UpdateSprite()
{
    if(m_sprite < 2)
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

void PUReflectorComponent::ActivatePowerUp()
{
    if(CheckIfPowerUpReady(true))
    {
        if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
        {
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 5.0);
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
            m_soundManager_ptr->Play(m_drone, "PowerUP");
        }   
        //Activate the powerup component
        SetActive(true);
        m_powerUpManager_ptr->m_activeReflectors_v.emplace_back(this);
        ////std::cout << "#########REFLECTOR ACTIVATED#########\n" << std::endl;

        //Initialize Clock
        m_clock_ptr->Init();

    } 
    else
    {
        ////std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
    }
}