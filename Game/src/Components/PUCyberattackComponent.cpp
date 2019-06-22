#include <PUCyberattackComponent.hpp>
#include <EntityManager.h>
#include <PhysicsManager.h>
#include <PowerUpManager.hpp>
#include <SoundManager.h>

PUCyberattackComponent::PUCyberattackComponent(glm::vec3* c_position, const std::string& id, int id_ent, Entity* d,  GenericData *&l_genericData_ptr) 
: PowerUpComponent(c_position, id, id_ent, d, ATTACK_VALUE_CYBERATTACK , TYPE_VALUE_CYBERATTACK, l_genericData_ptr)
{
    m_hackedPU_v.reserve(6);
    m_sprites_vec.reserve(3);
    m_sprites_vec.emplace_back("cyberattack0_disabled.png");
    m_sprites_vec.emplace_back("cyberattack1_ready.png");
    m_sprites_vec.emplace_back("cyberattack2_being_used.png");
}

PUCyberattackComponent::~PUCyberattackComponent()
{
    ClearVector();
    delete m_clock_ptr;
    m_clock_ptr = nullptr;

    std::vector<std::string> empty;
    m_sprites_vec.swap(empty);
};

void PUCyberattackComponent::AddHackedPU(PowerUpComponent* component)
{ 
    m_hackedPU_v.emplace_back(component); 
};

void PUCyberattackComponent::ClearVector()
{ 
    for(int i=0; i < (int)m_hackedPU_v.size(); i++)
    { 
        m_hackedPU_v[i]->SetHacked(false); 
        if(m_hackedPU_v[i]->GetDrone()->GetId() == 0)
        {
            //DISABLED GLITCH EFFECT
            m_renderManager_ptr->ActivatePPEffect("defaultPP");
        }
    }
    m_hackedPU_v.clear(); 
};

void PUCyberattackComponent::SetActive(bool value)
{ 
    m_active = value;

    if(m_active)
        ChangeSprite(BEING_USED); 
    else
    {
        ChangeSprite(DISABLED);
    }
};

void PUCyberattackComponent::UpdateSprite()
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

void PUCyberattackComponent::ActivatePowerUp()
{
    if(CheckIfPowerUpReady(true))
    {
        if(m_entityManager_ptr->GetComponent(m_drone->GetId(), "PowerUP"))
        {
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "PowerUP", 2.0);
            m_soundManager_ptr->SetEventVariableValue(m_drone, "PowerUP", "ThrowReceive", 0.0);
            m_soundManager_ptr->Play(m_drone, "PowerUP");
        }              
        //Activate the powerup component
        SetActive(true);
        m_powerUpManager_ptr->m_activeCyberattack_v.emplace_back(this);
        //std::cout << "#########CYBERATTACK ACTIVATED#########\n" << std::endl;

        //Initialize Clock
        m_clock_ptr->Init();

        //============================
        //APPLY THE CYBERATTACK EFFECT
        //============================
        DisableRivalsPowerUps();

    } 
    else
    {
        //std::cout << "ERROR: YOU NEED MORE ENERGY TO USE A POWER-UP" << std::endl;
    }
}


void PUCyberattackComponent::DisableRivalsPowerUps(){

    //check if you are the last drone 
    if(m_position_drone_ptr->GetPosition() != m_positionManager_ptr->GetLastPosition()){
        
        int next_pos = m_position_drone_ptr->GetPosition() + 1;
        Entity* l_rival_drone_ptr = nullptr;
        TransformComponent* l_rival_component_ptr = nullptr;
        PowerUpComponent* l_PU_component_ptr = nullptr;

        //hack until the last drone 
        while(next_pos <= m_positionManager_ptr->GetLastPosition())
        {
            l_rival_drone_ptr = m_positionManager_ptr->GetDrone(next_pos);

            //check if this rival has reflector
            if(m_powerUpManager_ptr->CheckIfRivalHasReflector(l_rival_drone_ptr)){ l_rival_drone_ptr = m_entityManager_ptr->GetEntity(m_position_drone_ptr->GetEntityId());}

            //check if this rival has shield
            if(!m_powerUpManager_ptr->CheckIfRivalHasShield(l_rival_drone_ptr)){

                l_rival_component_ptr = (TransformComponent*)m_entityManager_ptr->GetComponent(l_rival_drone_ptr->GetId(), "Physics");

                //hack both power ups for every rival drone
                for(unsigned int i=0; i<=1; i++){
                    l_PU_component_ptr = (PowerUpComponent*)l_rival_component_ptr->GetPowerUp(i)->m_powerUp_ptr;
                    if(!l_PU_component_ptr->GetHacked())
                    {
                        if(l_rival_drone_ptr->GetId() == 0)
                        {
                            //SET GLITCH SHADER
                            m_renderManager_ptr->ActivatePPEffect("glitch");
                            m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "PowerUP", 2.0f);
                            m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "ThrowReceive", 1.0f);
                            m_soundManager_ptr->Play(l_rival_drone_ptr, "PowerUP");
                        }
                        l_PU_component_ptr->SetHacked(true);
                        AddHackedPU(l_PU_component_ptr);
                    }
                }

            }
            //inc next position
            next_pos++;
        }
        
    }
    else
    {
        //std::cout << "ERROR: YOU ARE THE LAST ONE" << std::endl;
    }
}
