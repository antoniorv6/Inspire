#include <PUFreezingComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>

PUFreezingComponent::PUFreezingComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_FREEZING, TYPE_VALUE_FREEZING, l_genericData_ptr)
{
    m_sprites_vec.reserve(5);
    m_sprites_vec.emplace_back("freezer0_disabled.png");
    m_sprites_vec.emplace_back("freezer1_ready.png");
    m_sprites_vec.emplace_back("freezer2_one.png");
    m_sprites_vec.emplace_back("freezer3_two.png");
    m_sprites_vec.emplace_back("freezer4_three.png");
}

PUFreezingComponent::~PUFreezingComponent()
{
    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUFreezingComponent::SetActive(bool value)
{ 
    m_active = value;

    if(!m_active)
    {
        ChangeSprite(DISABLED);
    }
};

void PUFreezingComponent::UpdateSprite()
{
    if(CheckIfPowerUpReady(false))
    {
        int nearby_rivals = m_physicsManager_ptr->CalculateNearRivals(m_drone->GetId(), m_RADIUS);
        ChangeSprite((PowerUpSprites)(nearby_rivals + 1));
    }
    else
    {
        ChangeSprite(DISABLED);
    }
}


void PUFreezingComponent::ActivatePowerUp()
{
    //Check if the drone can use the power up
    Entity* l_powerUpEntity_ptr = m_entityManager_ptr->GetEntity(GetEntityId());

    if(CheckIfPowerUpReady(true))
    {
        if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
        {
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 3.0);
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
            m_soundManager_ptr->Play(m_drone, "PowerUP");
        }   
        //Activate the powerup component
        SetActive(true);
        //std::cout << "#########FREEZING ACTIVATED#########\n" << std::endl;

        //Create the Sphere BoundingBox of the Power Up
        l_powerUpEntity_ptr->setPos(m_drone->getPos());
        m_physicsManager_ptr->CreatePowerUpSphere(l_powerUpEntity_ptr, m_RADIUS, 0); 
    
        //Calculate Collision between Expansive Wave and Rivals Drones
        m_physicsManager_ptr->CalculatePowerUpCollision(l_powerUpEntity_ptr, m_drone);

        //Deactivate the powerup component
        SetActive(false);
    } 
    else
    {
        ////std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
    }
}