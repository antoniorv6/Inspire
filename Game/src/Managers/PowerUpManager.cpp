#include <PowerUpManager.hpp>
#include <GenericData.hpp>
#include <SoundManager.h>
#include <HUD.hpp>

#include <Event.h>
#include <Profiler.h>

void UsePowerUp(Event event);


PowerUpManager::PowerUpManager()
{
    m_powerUps_v.reserve(8);
    m_playerPU_v.reserve(2);

    m_activeShields_v.reserve(4); //list of the activated shieldss
    m_activeCyberattack_v.reserve(4); // list of the activated cyberattacks
    m_activeReflectors_v.reserve(4); // list of the activated reflectors 
}

PowerUpManager::~PowerUpManager()
{
    ClearList();
}

void PowerUpManager::Init(GenericData *&c_gameData)
{
    m_entityManager_ptr = c_gameData->g_entityManager_ptr;
    m_renderManager_ptr = c_gameData->g_renderManager_ptr;
    m_physicsManager_ptr = c_gameData->g_physicsManager_ptr;
    m_eventManager_ptr  = c_gameData->g_eventManager_ptr;
    m_positionManager_ptr= c_gameData->g_positionManager_ptr;
    m_eventManager_ptr->addListener(EventManager::Listener{UsePowerUp}, EventType::Power_Up);
    m_eventData_ptr = c_gameData;
    m_soundManager_ptr = c_gameData->g_soundManager_ptr;

    m_numShieldBB = 3;
}

void PowerUpManager::InitHUD(HUD*& c_HUD_ptr)
{
    m_HUD_ptr = c_HUD_ptr;
    SetHudPowerUpSprites();
}

void PowerUpManager::Update()
{
    //PROFILE_SCOPED();
    UpdateActiveShields();
    UpdateActiveCyberattacks();
    UpdateActiveReflectors();
    CheckPowerUpSprites();
}

//=================CREATE POWER UP COMPONENT=======================

bool PowerUpManager::CreatePowerUp(TypePU type, Entity* drone)
{   
    //Check if the drone has physics component
    TransformComponent* physics = (TransformComponent*) m_entityManager_ptr->GetComponent(drone->GetId(), "Physics");

    if(physics != nullptr){

        //Create PowerUp's Entity
        Entity* l_PowerUp = m_entityManager_ptr->CreateEntity("PowerUp",drone->getPos());

        //Create PowerUp Component
        PowerUpComponent* l_PUComponent = nullptr;

        switch(type)
        {
            case shield:        l_PUComponent = new PUShieldComponent(drone->getPosPtr(), "PUShield", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;

            case magnet:        l_PUComponent = new PUMagnetComponent(drone->getPosPtr(), "PUMagnet", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;
            
            case freezing:      l_PUComponent = new PUFreezingComponent(drone->getPosPtr(), "PUFreezing", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;
            
            case thief:         l_PUComponent = new PUThiefComponent(drone->getPosPtr(), "PUThief", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;

            case reflector:     l_PUComponent = new PUReflectorComponent(drone->getPosPtr(), "PUReflector", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;
            
            case cyberattack:   l_PUComponent = new PUCyberattackComponent(drone->getPosPtr(), "PUCyberattack", l_PowerUp->GetId(), drone, m_eventData_ptr);
            break;
        }

        m_powerUps_v.emplace_back(l_PUComponent);

        //Add PowerUpComponent to PowerUpEntity 
        m_entityManager_ptr->AddComponent(l_PowerUp->GetId(), l_PUComponent);  
        
        //===========BILLBOARDS========

        //If Drone is the player
        if(drone->GetId() == 0)
        {
            if(billboard == -1 && (type == magnet || type == thief))
            {
                //CREATE RIVAL BILLBOARD 
                glm::vec3 init_pos = {0,0,-100000};
                m_renderManager_ptr->GetRenderFacade()->CreateBillboard(0, "bottom_arrow.png", 35.0f, 50.0f, init_pos);
            }
            m_playerPU_v.emplace_back(l_PUComponent);
        }

        //If type is shield
        if(type == shield && drone->GetId() != 0)
        {
            //CREATE SHIELD BILLBOARD 
            m_numShieldBB++;
            glm::vec3 init_pos = {0,0,-100000};
            m_entityShieldBB_v.emplace_back(drone);
            m_renderManager_ptr->GetRenderFacade()->CreateBillboard(m_numShieldBB, "shield_billboard.png", 42.0f, 50.0f, init_pos);
        }

        //===========================

        //Add PowerUpComponent to Drone's PhysicsComponent
        if(!physics->SetPowerUp(l_PUComponent, type)){

            //std::cout << "ERROR: THIS DRONE ALREADY HAS 2 POWER UPS." << std::endl;

            return false;
        }
            
        return true;
    }
    else{

        //std::cout << "ERROR: IMPOSIBLE TO CREATE A POWER UP IF THE DRONE HAS NOT PHYSICS COMPONENT." << std::endl;

        return false;
    }

}

//===================CREATE POWER UP EVENT======================

void PowerUpManager::CreatePUEvent(int l_droneId_n, int l_numberPowerUp_n)
{
    Event powerup{"Power_Up"+std::to_string(l_numberPowerUp_n), Power_Up, m_entityManager_ptr->GetEntity(l_droneId_n), m_eventData_ptr};    
    m_eventManager_ptr->addEvent(powerup);
}

//=============READ POWER UP EVENT AND ACTIVATE IT==============

void UsePowerUp(Event event)
{
    //Get EventData
    int l_droneId_n = event.entity_pt->GetId();
    int l_numberPowerUp_n = (int)event.id_str[8] - 48;
    
    //Get the PowerUpComponent that the Drone has assigned to the Pressed Key
    PowerUpData* l_powerUp = ((TransformComponent*)event.m_eventData_ptr->g_entityManager_ptr->GetComponent(l_droneId_n,"Physics"))->GetPowerUp(l_numberPowerUp_n);
    PowerUpComponent* l_PUComponent_ptr = (PowerUpComponent*) l_powerUp->m_powerUp_ptr;

    //Check if the PU is hacked
    if(!l_PUComponent_ptr->GetHacked())
    {
        l_PUComponent_ptr->ActivatePowerUp();
    }
    else
    {
        //std::cout << "ERROR: YOUR DRONE IS HACKED, YOU CANT USE YOUR POWER-UP." << std::endl;
    }
    
}


//===========================SHIELD===========================

bool PowerUpManager::CheckIfRivalHasShield(Entity* l_rival_drone_ptr)
{
    for(int i=0; i < (int)m_activeShields_v.size(); i++)
    {
        if(m_activeShields_v[i]->GetDrone() == l_rival_drone_ptr)
        {
            //Decrease shield lifes
            m_activeShields_v[i]->DecLifes();

            if(l_rival_drone_ptr->GetId() == 0)
            {
                m_renderManager_ptr->ActivatePPEffect("shield");
                m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "PowerUP", 0.0f);
                m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "ThrowReceive", 1.0f);
                m_soundManager_ptr->Play(l_rival_drone_ptr, "PowerUP");
            }

            return true;
        }
    }
    return false;
}

bool PowerUpManager::CheckIfRivalHasShield(int l_drone_id)
{
    for(int i=0; i < (int)m_activeShields_v.size(); i++)
    {
        if(m_activeShields_v[i]->GetDrone()->GetId() == l_drone_id)
        {
            return true;
        }
    }
    return false;
}


//===========================REFLECTOR===========================

bool PowerUpManager::CheckIfRivalHasReflector(Entity* l_rival_drone_ptr)
{
    for(int i=0; i < (int)m_activeReflectors_v.size(); i++)
    {
        if(m_activeReflectors_v[i]->GetDrone() == l_rival_drone_ptr)
        {
            //Erase from the Activated vector
            m_activeReflectors_v[i]->SetActive(false);
            m_activeReflectors_v.erase(m_activeReflectors_v.begin()+i);
            //std::cout << "EL DRONE " << l_rival_drone_ptr->GetName() << " TENÍA REFLECTOR\n\n";

            if(l_rival_drone_ptr->GetId() == 0)
            {
                m_renderManager_ptr->ActivatePPEffect("mirror");
                m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "PowerUP", 5.0f);
                m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "ThrowReceive", 1.0f);
                m_soundManager_ptr->Play(l_rival_drone_ptr, "PowerUP");
            }

            return true;
        }
    }
    return false;
}

//=============POWER UPS ATTACK VALUE FOR DIFFUSE LOGIC AI============

int PowerUpManager::GetDroneAttackValue(int l_entityId_n)
{
    TransformComponent* physics = (TransformComponent*)  m_entityManager_ptr->GetComponent(l_entityId_n, "Physics");

    if(physics != nullptr)
    {
        int att1 = ((PowerUpComponent*) physics->GetPowerUp(0)->m_powerUp_ptr)->GetAttackValue();
        int att2 = ((PowerUpComponent*) physics->GetPowerUp(1)->m_powerUp_ptr)->GetAttackValue();

        return ((att1 + att2) / 2);
    }

    return -1;
}

//=============POWER UP TYPE VALUE FOR DIFFUSE LOGIC AI==============

int PowerUpManager::GetPowerUpTypeValue(int l_entityId_n, int l_numPU)
{
    TransformComponent* physics = (TransformComponent*)  m_entityManager_ptr->GetComponent(l_entityId_n, "Physics");

    if(physics != nullptr)
    {
        return ((PowerUpComponent*) physics->GetPowerUp(l_numPU)->m_powerUp_ptr)->GetTypeValue();
    }

    return -1;
}

//============GET POWER UP SPRITE==================
int PowerUpManager::GetSprite(int i)
{
    return m_playerPU_v[i]->GetSprite();
}

//============SET POWER UP SPRITE==================
void PowerUpManager::SetSprite(int texture, PowerUpComponent* component)
{
    bool left;

    if(component == m_playerPU_v[0])
    {
        left = true;
    }
    else if(component == m_playerPU_v[1])
    {
        left = false;
    }

    m_HUD_ptr->SetPowerUpTexture(texture, left);
}
//============CHECK POWER UP SPRITES==================
void PowerUpManager::CheckPowerUpSprites()
{
    m_playerPU_v[0]->UpdateSprite();
    m_playerPU_v[1]->UpdateSprite();
}
//============SET POWER UP SPRITES===================
void PowerUpManager::SetHudPowerUpSprites()
{
    std::vector<std::string> sprites_left = m_playerPU_v[0]->GetSpritesVector();
    m_HUD_ptr->AddPowerupHUDElement(1100, 650, sprites_left, true);

    std::vector<std::string> sprites_right = m_playerPU_v[1]->GetSpritesVector();
    m_HUD_ptr->AddPowerupHUDElement(1200, 650, sprites_right, false);
}

//==========================================================================
//==========================================================================
//                  UPDATE POWER UP FUNCTIONS 
//==========================================================================
//==========================================================================

void PowerUpManager::UpdateActiveShields()
{

    for(int i=0; i < (int)m_activeShields_v.size(); i++){
        
        //Check if the lifetime od the shield is over to decrease a life
        if(m_activeShields_v[i]->GetClock()->GetElapsedTime()  > m_activeShields_v[i]->GetTime())
        {
            m_activeShields_v[i]->DecLifes();
            m_activeShields_v[i]->GetClock()->Restart();
        }

        //Check if drone is still activated and if the lifetime of the shield is over
        if(!m_activeShields_v[i]->GetActive()){
            
            //Erase from the Activated vector
            m_activeShields_v[i]->SetActive(false);
            m_activeShields_v.erase(m_activeShields_v.begin()+i);
            //Move Debug Sphere to 0,0,0
            //m_renderManager_ptr->TranslateBSphere(m_activeShields_v[i]->GetEntityId(),glm::vec3{0,0,0});

        }

    }
}

void PowerUpManager::UpdateActiveCyberattacks()
{

    for(int i=0; i < (int)m_activeCyberattack_v.size(); i++){

        //Check if the lifetime of the shield is over
        if(m_activeCyberattack_v[i]->GetClock()->GetElapsedTime() > m_activeCyberattack_v[i]->GetTime()){
            
            //Clear the vector of hacked PowerUps
            m_activeCyberattack_v[i]->ClearVector();

            //Erase from the Activated vector
            m_activeCyberattack_v[i]->SetActive(false);
            m_activeCyberattack_v.erase(m_activeCyberattack_v.begin()+i);

        }
        
    }
}

void PowerUpManager::UpdateActiveReflectors()
{

    for(int i=0; i < (int)m_activeReflectors_v.size(); i++){

        //Check if the lifetime of the shield is over
        if(m_activeReflectors_v[i]->GetClock()->GetElapsedTime() > m_activeReflectors_v[i]->GetTime()){
            
            //Erase from the Activated vector
            m_activeReflectors_v[i]->SetActive(false);
            m_activeReflectors_v.erase(m_activeReflectors_v.begin()+i);

        }
        
    }
}

void PowerUpManager::UpdateBillboardsPU()
{
    glm::vec3 rival_pos = {0,0,-100000};

    //RIVAL BILLBOARD
    if(billboard == 1)
    {
        rival_pos = m_billboard_rival->getPos();
        rival_pos.z += 80.0f;
        m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(0, rival_pos);
    }
    else if(billboard == 0)
    {
        m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(0, rival_pos);
    }

    //SHIELDS BILLBOARDS
    for(unsigned int i=0; i < m_entityShieldBB_v.size(); i++)
    {
        glm::vec3 drone_pos = {0,0,-100000};

        if(CheckIfRivalHasShield(m_entityShieldBB_v[i]->GetId()))
        {
            //if rival has shield, arrow disabled
            if(m_entityShieldBB_v[i] == m_billboard_rival)
            {
                m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(0, drone_pos);  
            }
            drone_pos = m_entityShieldBB_v[i]->getPos();
            drone_pos.z += 80.0f;
            m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(i+4, drone_pos);
        }
        else
        {
            m_renderManager_ptr->GetRenderFacade()->TranslateBillboard(i+4, drone_pos);
        }
    }
}




//==========================================================================
//==========================================================================
//                     CLOCKS FUNCTIONS
//==========================================================================
//==========================================================================

void PowerUpManager::PauseClocks()
{
    for(int i=0; i < (int)m_activeShields_v.size(); i++)
        m_activeShields_v[i]->GetClock()->Pause();

    for(int i=0; i < (int)m_activeCyberattack_v.size(); i++)
        m_activeCyberattack_v[i]->GetClock()->Pause();

    for(int i=0; i < (int)m_activeReflectors_v.size(); i++)
        m_activeReflectors_v[i]->GetClock()->Pause();
}

void PowerUpManager::PlayClocks()
{
    for(int i=0; i < (int)m_activeShields_v.size(); i++)
        m_activeShields_v[i]->GetClock()->Play();

    for(int i=0; i < (int)m_activeCyberattack_v.size(); i++)
        m_activeCyberattack_v[i]->GetClock()->Play();

    for(int i=0; i < (int)m_activeReflectors_v.size(); i++)
        m_activeReflectors_v[i]->GetClock()->Play();
}

//==========================================================================
//==========================================================================
//                     CLEAR DATA FUNCTIONS
//==========================================================================
//==========================================================================

void PowerUpManager::ClearList()
{
    for(unsigned int i = 0; i < m_powerUps_v.size(); i++)
    {
        delete m_powerUps_v[i];
        m_powerUps_v[i] = nullptr;
    }
    m_powerUps_v.clear();
    m_activeShields_v.clear();
    m_activeCyberattack_v.clear();
    m_activeReflectors_v.clear();
    m_playerPU_v.clear();

    billboard = -1;
    m_numShieldBB = 3;
    m_entityShieldBB_v.clear();
    
}






