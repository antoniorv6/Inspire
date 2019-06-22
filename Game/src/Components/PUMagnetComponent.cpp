#include <PUMagnetComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>

PUMagnetComponent::PUMagnetComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_MAGNET, TYPE_VALUE_MAGNET, l_genericData_ptr)
{
    m_sprites_vec.reserve(3);
    m_sprites_vec.emplace_back("magnet0_disabled.png");
    m_sprites_vec.emplace_back("magnet1_ready.png");
    m_sprites_vec.emplace_back("magnet2_effective.png");
};

PUMagnetComponent::~PUMagnetComponent()
{
    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUMagnetComponent::SetActive(bool value)
{ 
    m_active = value;

    if(!m_active)
    {
        ChangeSprite(DISABLED);
    }
};

void PUMagnetComponent::UpdateSprite()
{
    if(CheckIfPowerUpReady(false))
    {
        UpdateRival();
        
        if(CheckIfCloseEnough(m_rival_drone, m_position_drone_ptr->GetPosition(), m_DISTANCE))
        {
            UpdateBillboard(1);
            m_powerUpManager_ptr->m_billboard_rival = m_rival_drone;
            ChangeSprite(EFFECTIVE);
        }
        else
        {
            UpdateBillboard(0);
            ChangeSprite(READY);
        }
    }
    else
    {
        UpdateBillboard(0);
        ChangeSprite(DISABLED);
    }
}

void PUMagnetComponent::UpdateRival() 
{ 
    int next_position = m_positionManager_ptr->CalculateNextPosition(m_position_drone_ptr);
    m_rival_drone = m_positionManager_ptr->GetDrone(next_position);
};

void PUMagnetComponent::ActivatePowerUp()
{

    if(CheckIfPowerUpReady(true))
    {   
        UpdateRival();

        if(CheckIfCloseEnough(m_rival_drone, m_position_drone_ptr->GetPosition(), m_DISTANCE))
        {
            if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
            {
                m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 1.0);
                m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
                m_soundManager_ptr->Play(m_drone, "PowerUP");
            }   

            //Activate the powerup component
            SetActive(true);
            //std::cout << "#########MAGNET ACTIVATED#########\n" << std::endl;

            //Magnetize the rival
            Magnetize();
            
            //Deactivate the magnet
            SetActive(false);
        } 
        else
        {
            //std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
        }
    
    }
}


void PUMagnetComponent::Magnetize()
{       
    PICASSO::PhysicsVariables* l_affected_drone_data = ((TransformComponent*) m_entityManager_ptr->GetComponent(m_rival_drone->GetId() , "Physics"))->GetPhysicsData();
    Entity* affected_drone = m_rival_drone;

    //Calculate Magnet Force Direction
    glm::vec3 magnet_force = glm::normalize(m_drone->getPos() - m_rival_drone->getPos()) * m_drone_data->MaxVelocityNITRO;    

    //Check if the rival drone has reflector
    if(m_powerUpManager_ptr->CheckIfRivalHasReflector(affected_drone))
    {
        //Repulsion Effect
        l_affected_drone_data = m_drone_data;
        affected_drone = m_drone;
    }
    
    if(!m_powerUpManager_ptr->CheckIfRivalHasShield(affected_drone))
    {
        //Magnet Effect
        l_affected_drone_data->v_col_velocity = magnet_force;
        PICASSO::SetCollisionData(l_affected_drone_data, 1);

        if(affected_drone->GetId() == 0)
        {
            m_renderManager_ptr->ActivatePPEffect("magnet");
            m_soundManager_ptr->SetEventVariableValue(affected_drone, "PowerUP", "PowerUP", 1.0f);
            m_soundManager_ptr->SetEventVariableValue(affected_drone, "PowerUP", "ThrowReceive", 1.0f);
            m_soundManager_ptr->Play(affected_drone, "PowerUP");
        }
    }
}
