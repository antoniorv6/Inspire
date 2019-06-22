#include <PowerUpComponent.hpp>
#include <GenericData.hpp>
#include <PositionComponent.hpp>
#include <PowerUpManager.hpp>

PowerUpComponent::PowerUpComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d, const int attack_value, const int type_value,  GenericData *&l_genericData_ptr)
: IComponent(id, id_ent)
{
    m_position = c_position;
    m_drone = d;
    m_ATTACK_VALUE = attack_value;
    m_TYPE_VALUE = type_value;
    m_entityManager_ptr     = l_genericData_ptr->g_entityManager_ptr;
    m_physicsManager_ptr    = l_genericData_ptr->g_physicsManager_ptr;
    m_positionManager_ptr   = l_genericData_ptr->g_positionManager_ptr;
    m_powerUpManager_ptr    = l_genericData_ptr->g_powerUpManager_ptr;
    m_soundManager_ptr      = l_genericData_ptr->g_soundManager_ptr;
    m_renderManager_ptr     = l_genericData_ptr->g_renderManager_ptr;

    m_drone_data = ((TransformComponent*) m_entityManager_ptr->GetComponent(m_drone->GetId(), "Physics"))->GetPhysicsData(); 
    m_position_drone_ptr = (PositionComponent*)m_entityManager_ptr->GetComponent(m_drone->GetId(), "Position");
}

PowerUpComponent::~PowerUpComponent(){};

bool PowerUpComponent::CheckIfCloseEnough(Entity* l_rival_drone_ptr, int l_drone_position_n, float l_distance_f)
{
    if(l_drone_position_n != 1)
    {
        //Calculate Distence between The Drone and The Next Rival Drone
        float distance = glm::distance(m_drone->getPos(), l_rival_drone_ptr->getPos());
        
        //If you are enough close you can activate the power up
        if(distance <= l_distance_f)
        {
            return true;
        }
        else 
        {
            //std::cout << "ERROR: YOU ARE NOT CLOSE ENOUGH" << std::endl;
            return false;
        }
    }
    else 
    {
        //std::cout << "ERROR: YOU ARE THE FIRST" << std::endl;
        return false;
    }
}

bool PowerUpComponent::CheckIfPowerUpReady(bool reset_nitro)
{
    //std::cout << "Carga de nitro: " << data->chargeNITRO << std::endl;
    if(m_drone_data->chargeNITRO == m_drone_data->MaxChargeNITRO)
    {
        if(reset_nitro)
        {
            m_drone_data->chargeNITRO = 0;
            return true;
        }

        else
        {
            return !m_hacked;
        }
    }
    else
    {
        return false;
    }
}

void PowerUpComponent::ChangeSprite(PowerUpSprites sprite)
{
    if(m_sprite != sprite && m_drone->GetId() == 0)
    {
        m_sprite = sprite;
        m_powerUpManager_ptr->SetSprite(m_sprite, this);
        //std::cout << GetComponent() << " sprite: " << m_sprite << std::endl;
    }   
}

void PowerUpComponent::UpdateBillboard(int value)
{
    m_powerUpManager_ptr->billboard = value;
}