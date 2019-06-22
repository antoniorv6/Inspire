#include <PUThiefComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>

PUThiefComponent::PUThiefComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_THIEF, TYPE_VALUE_THIEF, l_genericData_ptr)
{
    m_sprites_vec.reserve(3);
    m_sprites_vec.emplace_back("thief0_disabled.png");
    m_sprites_vec.emplace_back("thief1_ready.png");
    m_sprites_vec.emplace_back("thief2_effective.png");
};

PUThiefComponent::~PUThiefComponent()
{
    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUThiefComponent::SetActive(bool value)
{ 
    m_active = value;

    if(!m_active)
    {
        ChangeSprite(DISABLED);
    }
};

void PUThiefComponent::UpdateSprite()
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

void PUThiefComponent::UpdateRival() 
{ 
    int next_position = m_positionManager_ptr->CalculateNextPosition(m_position_drone_ptr);
    m_rival_drone = m_positionManager_ptr->GetDrone(next_position);
};

void PUThiefComponent::ActivatePowerUp()
{
    if(CheckIfPowerUpReady(true))
    {
        UpdateRival();

        if(CheckIfCloseEnough(m_rival_drone, m_position_drone_ptr->GetPosition(), m_DISTANCE))
        {   
            if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
            {
                m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 4.0);
                m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
                m_soundManager_ptr->Play(m_drone, "PowerUP");
            }   
            //Activate the powerup component
            SetActive(true);
            //std::cout << "#########THIEF ACTIVATED#########\n" << std::endl;

            //Steal the energy
            StealEnergy();

            //Deactivate the powerup component
            SetActive(false);

        } 
        else
        {
            //std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
        }
    
    }
}


void PUThiefComponent::StealEnergy()
{
    //Apply the energy steal
    PICASSO::PhysicsVariables* l_rival_drone_data = ((TransformComponent*) m_entityManager_ptr->GetComponent(m_rival_drone->GetId(), "Physics"))->GetPhysicsData();
    
    //Check if the rival drone has reflector
    if(m_powerUpManager_ptr->CheckIfRivalHasReflector(m_rival_drone))
    {
        l_rival_drone_data->thiefNITRO = 50;
    }
    
    else if(!m_powerUpManager_ptr->CheckIfRivalHasShield(m_rival_drone))
    {
        m_drone_data->thiefNITRO = l_rival_drone_data->chargeNITRO;
        l_rival_drone_data->thiefNITRO = -l_rival_drone_data->chargeNITRO;
    }

    m_drone_data->thief = true;
    l_rival_drone_data->thief = true;

    if(m_rival_drone->GetId() == 0)
    {
        m_renderManager_ptr->ActivatePPEffect("thief");
        m_soundManager_ptr->SetEventVariableValue(m_rival_drone, "PowerUP", "PowerUP", 4.0f);
        m_soundManager_ptr->SetEventVariableValue(m_rival_drone, "PowerUP", "ThrowReceive", 1.0f);
        m_soundManager_ptr->Play(m_rival_drone, "PowerUP");
    }

    if(m_drone->GetId() == 0)
    {
        m_soundManager_ptr->SetEventVariableValue(m_rival_drone, "PowerUP", "PowerUP", 4.0f);
        m_soundManager_ptr->SetEventVariableValue(m_rival_drone, "PowerUP", "ThrowReceive", 0.0f);
        m_soundManager_ptr->Play(m_rival_drone, "PowerUP");
    }
    
}