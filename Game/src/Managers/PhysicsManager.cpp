#include <PhysicsManager.h>
#include <OrionClientPredictor.hpp>
#include <PowerUpManager.hpp>
#include <Profiler.h>
#include <CommonData.hpp>
#include <SoundManager.h>
#include <AIManager.hpp>

void NitroCharge(Event);

PhysicsManager::PhysicsManager(){
    
    //Reserve vectors sizes
    RB_CloseComponents.reserve(104);
    m_TransComp_list.reserve(4);
    m_RBComp_list.reserve(200);
    m_RBCompDrones_list.reserve(4);
    m_RBNitroZones_v3.reserve(20);
}

void PhysicsManager::Init(GenericData *& c_genericData)
{
    m_entityManager_ptr     = c_genericData->g_entityManager_ptr;
    m_eventManager_ptr      = c_genericData->g_eventManager_ptr;
    m_renderManager_ptr     = c_genericData->g_renderManager_ptr;
    m_powerUpManager_ptr    = c_genericData->g_powerUpManager_ptr;
    m_checkpointManager_ptr = c_genericData->g_checkPointManager_ptr;
    m_positionManager_ptr   = c_genericData->g_positionManager_ptr;
    m_soundManager_ptr      = c_genericData->g_soundManager_ptr;
    m_aiManager_ptr         = c_genericData->g_aiManager_ptr;
    m_renderUtilities_ptr   = RenderUtilities::getInstance();

    m_gameData_ptr = c_genericData;

    //Add Listeners
    m_eventManager_ptr->addListener(EventManager::Listener{NitroCharge}, EventType::Nitro_Charge);
}

/**
 * @brief Destroy the Physics Manager:: Physics Manager object
 * 
 */
PhysicsManager::~PhysicsManager()
{
    ClearList();
}

void PhysicsManager::ClearList()
{
    for(unsigned int i = 0; i < m_TransComp_list.size(); i++)
     {
        delete m_TransComp_list[i]->physicsDataPtr->frozen_clk;
        m_TransComp_list[i]->physicsDataPtr->frozen_clk = nullptr;;

        delete m_TransComp_list[i]->physicsDataPtr->electShock_clk;
        m_TransComp_list[i]->physicsDataPtr->electShock_clk = nullptr;

        delete m_TransComp_list[i];
        m_TransComp_list[i] = nullptr;
    }

    for(unsigned int i = 0; i < m_RBComp_list.size(); i++)
    {
        delete m_RBComp_list[i];
        m_RBComp_list[i] = nullptr;
    }
    
    m_TransComp_list.clear();
    m_RBComp_list.clear();
    m_RBCompDrones_list.clear();
    m_RBNitroZones_v3.clear();
    RB_CloseComponents.clear();

    delete octree;
    octree = nullptr;

    InOut = 0;
}

void PhysicsManager::CreateComponent(Entity* entity){
    TransformComponent* physics = new TransformComponent(&entity->m_NowED.pos, "Physics", entity->id);
    m_TransComp_list.push_back(physics);
    m_entityManager_ptr->AddComponent(entity->id, (IComponent*) physics);
}

void PhysicsManager::CreateRigidBodySphere(Entity* entity, float radius, float mass){
    RigidBodyComponent* rigidbody = new RigidBodyComponent(&entity->m_NowED.pos, "RigidBody", radius, mass, entity->id);
    m_RBComp_list.push_back(rigidbody);
    m_entityManager_ptr->AddComponent(entity->id, (IComponent*) rigidbody);

    if(debugBB)
        m_renderManager_ptr->DebugBSphere(entity->id, entity->m_NowED.pos, radius);
}

void PhysicsManager::CreatePowerUpSphere(Entity* entity, float radius, float mass){
    RigidBodyComponent* rigidbody = new RigidBodyComponent(&entity->m_NowED.pos, "RigidBody", radius, mass, entity->id);
    m_entityManager_ptr->AddComponent(entity->id, (IComponent*) rigidbody);
    m_renderManager_ptr->DebugBSphere(entity->id, entity->m_NowED.pos, radius);
}

void PhysicsManager::CreateRigidBodyRectangle(Entity* entity, float width, float height, float depth, float mass, float rotx, float roty, float rotz, bool nitro, bool checkpoint, bool electroshock){
    RigidBodyComponent* rigidbody = new RigidBodyComponent(&entity->m_NowED.pos, "RigidBody", width, height, depth, mass, rotx, roty, rotz, entity->id, nitro, checkpoint, electroshock);
    m_RBComp_list.push_back(rigidbody);
    m_entityManager_ptr->AddComponent(entity->id, (IComponent*) rigidbody);

    if(debugBB)
        m_renderManager_ptr->AddDebugCube(entity->id, entity->m_NowED.pos, glm::vec3(rotx, roty, rotz), glm::vec3(width, depth, height));
    
    if(nitro)
        m_RBNitroZones_v3.emplace_back(rigidbody);
}

void PhysicsManager::CreateOctree(){
    if(octree != nullptr){
        octree->clear();
    }
    octree = new Octree(0, Octree::Box{70000.0f, glm::vec3(-70000/2, -70000/2, -70000/3)});

    for(int i=0; i < (int)m_RBComp_list.size(); i++){
        if(m_entityManager_ptr->GetComponent(m_RBComp_list[i]->GetEntityId(), "Physics") == nullptr && !m_RBComp_list[i]->checkpoint_zone){
            ////std::cout << "INSERTAMOS UN OBJETO EN EL OCTREE "<< i << std::endl;
            octree->insert(m_RBComp_list[i]);
        }
    }

}

void PhysicsManager::CreateDronesRBList(){
    
    for(int i=0; i < (int)m_TransComp_list.size(); i++){
        RigidBodyComponent* RBcomponent = (RigidBodyComponent*)m_entityManager_ptr->GetComponent(m_TransComp_list[i]->GetEntityId(), "RigidBody");
        m_RBCompDrones_list.emplace_back(RBcomponent);
    }
}

void PhysicsManager::Update(float delta, const float loopTime){

    //PROFILE_SCOPED();

    if(!onlineMatch)
    {
        delta /= (1.0f/30.0f);
    }
    //std::cout << delta << std::endl;

    for(int i=0; i < (int)m_TransComp_list.size(); i++){
        
        if(m_TransComp_list[i] != nullptr){

            Entity* entity = m_entityManager_ptr->GetEntity(m_TransComp_list[i]->GetEntityId());
            PICASSO::PhysicsVariables* data = m_TransComp_list[i]->physicsDataPtr;

            float value = UpdatePerspective(data,entity);
            UpdateShader(data,entity,value);

            if(!onlineMatch)
            {
                entity->setBeforeED(*entity->getAfterED());//New data is now -> old data
                entity->setPos(entity->getAfterED()->pos);//Recover calculated position last physicis manager update (modified previously by interpolation)
                entity->setRot(entity->getAfterED()->rot);//Recover calculated rotation last physicis manager update (modified previously by interpolation)
            }
            //UPDATE MOVEMENT
            PICASSO::UpdateMovement(data, delta, entity->id);   

            //UPDATE COLLISIONS
            IComponent* IComponent = m_entityManager_ptr->GetComponent(entity->id, "RigidBody");
            if(IComponent != nullptr){

                RigidBodyComponent* l_droneRB_ptr = (RigidBodyComponent*) IComponent;
                PositionComponent* l_dronePOS_ptr = (PositionComponent*) m_entityManager_ptr->GetComponent(entity->id, "Position");

                if(l_droneRB_ptr->m_collision_b)
                {
                    /*GET CLOSE OBJECTS*/
                    RB_CloseComponents.clear();
                    octree->getCloseObjects(&RB_CloseComponents, l_droneRB_ptr);
                    //std::cout << "Todos: " << octree->getAllCompPTR()->size() << " Cercanos: " << RB_CloseComponents.size() << std::endl;
                    UpdateCollision(data, l_droneRB_ptr, l_dronePOS_ptr, (entity->m_NowED.pos + data->v_velocity) , entity, delta);
                }
                else
                {
                    CollisionWithNextCheckpoint(entity, l_droneRB_ptr, l_dronePOS_ptr, data);
                }
                
            }

            //UPDATE POSITION
            UpdateVerticalPosition(data, entity);
            
            if(onlineMatch){
                entity->setPos(data->currentPosition);
                entity->setRot(data->v_rotation);
            }else{
                entity->setAfterED(data->currentPosition, data->v_rotation);//Set new data 
            }
            
        }
    }
}

void PhysicsManager::UpdateVerticalPosition(PICASSO::PhysicsVariables* data, Entity* entity)
{
    data->currentPosition = entity->m_NowED.pos + data->v_velocity;
    
    //Limit vertical movement. 
    if(data->currentPosition.z > data->verticalLimit)
    {
        float difference = data->currentPosition.z - data->verticalLimit;

        if(difference < 10.0f){ data->currentPosition.z = data->verticalLimit; }
        else { data->currentPosition.z -= difference/5.0f; }
        
        data->v_velocity.z = 0;
        data->AxisZ = 0;
    }
}

void PhysicsManager::MakeMovementPrediction(ClientPredictor& c_predictor, double c_now, int c_playerId)
{
    Entity* entity = m_entityManager_ptr->GetEntity(c_playerId);
    TransformComponent* component = (TransformComponent*)m_entityManager_ptr->GetComponent(c_playerId, "Physics");
    PICASSO::PhysicsVariables* data = component->physicsDataPtr;
        
    PICASSO::UpdateMovement(data, 1.0, entity->id);   

    data->currentPosition = entity->m_NowED.pos + data->v_velocity;
    EventRegister l_prediction;
    l_prediction.m_lastPosition = data->currentPosition;
    l_prediction.m_lastRotation = data->v_rotation;
    l_prediction.m_lastVvelocity = data->v_velocity;
    l_prediction.m_rotSpin      = data->RotSPIN;
    l_prediction.m_timeStamp    = c_now;
    c_predictor.RegisterEvent(true, l_prediction);
}

void PhysicsManager::InterpolatePositions(float delta, const float loopTime){

    for(int i=0; i < (int)m_TransComp_list.size(); i++){

        Entity* entity = m_entityManager_ptr->GetEntity(((IComponent*)m_TransComp_list[i])->GetEntityId());

        Entity::EntityData* BeforeED = entity->getBeforeED();
        Entity::EntityData* AfterED = entity->getAfterED();

        entity->setPos(BeforeED->pos + (delta * (AfterED->pos - BeforeED->pos)) / loopTime);
        entity->setRot(BeforeED->rot + (delta * (AfterED->rot - BeforeED->rot)) / loopTime);

        
        float dif = AfterED->rot.z - BeforeED->rot.z;
        if(glm::abs(dif) >100){
            
            if(glm::abs(AfterED->rot.z) > 300)
                AfterED->rot.z-= copysign(360,BeforeED->rot.z);
            else if(glm::abs(BeforeED->rot.z) > 300)
                BeforeED->rot.z-= copysign(360,BeforeED->rot.z);
                
        dif = AfterED->rot.z - BeforeED->rot.z;
        entity->setRotZ(BeforeED->rot.z + (delta * (dif)) / loopTime);      
        }
    }

}

void PhysicsManager::SetOnlineMatch(bool isOnlineMatch)
{
    onlineMatch = isOnlineMatch;
}

bool PhysicsManager::UpdateCollision(PICASSO::PhysicsVariables* data, RigidBodyComponent* l_RBdrone, PositionComponent* l_dronePOS_ptr, glm::vec3 l_dronePosition, Entity* l_drone_ptr, float delta){

    bool collision = false;

    if(l_drone_ptr->GetId() == 0)
        recharging = false;

    if(m_checkpointManager_ptr->GetNumCheckpoints() > 0)
        CollisionWithNextCheckpoint(l_drone_ptr, l_RBdrone, l_dronePOS_ptr, data);

    //============================================================
    //   STATIC COLLISION: COLLISION BETWEEN DRONE AND OBSTACLE
    //============================================================

    for(int i=0; i < (int)RB_CloseComponents.size(); i++){

        Entity* l_obstacle_ptr = m_entityManager_ptr->GetEntity(RB_CloseComponents[i]->GetEntityId());            

        glm::vec3 l_obstaclePosition = l_obstacle_ptr->m_NowED.pos;

        glm::vec3 closestpoint = PICASSO::ClosestPoint( &RB_CloseComponents[i]->r_bounding.axis, &RB_CloseComponents[i]->r_bounding.size, &l_obstaclePosition, &l_dronePosition);
        collision = PICASSO::CheckSphereOBBCollision(&l_dronePosition, &closestpoint, l_RBdrone->s_bounding.radius);
                
        if (collision){

            //============================================================
            //      COLLISION WITH NITRO ZONE --> RECHARGE NITRO
            //============================================================

            if(RB_CloseComponents[i]->nitro_zone)
            {
                Event nitro{"Nitro_Charge",Nitro_Charge,l_drone_ptr, m_gameData_ptr};
                m_eventManager_ptr->addEvent(nitro);

                if(l_drone_ptr->GetId() == 0)
                {
                    recharging = true;
                }
            }

            //============================================================
            //    COLLISION WITH OBSTACLE --> CALCULATE COLLISION FORCE
            //============================================================

            else
            {

                PICASSO::CalculateSphereOBBCollision(&RB_CloseComponents[i]->r_bounding.axis, &RB_CloseComponents[i]->r_bounding.size, &RB_CloseComponents[i]->r_bounding.faces,
                                                     &l_obstaclePosition, &l_drone_ptr->m_NowED.pos, data, l_RBdrone->mass, RB_CloseComponents[i]->mass);
                PICASSO::SetCollisionData(data, delta);
                
                if(RB_CloseComponents[i]->electroshock_effect)
                    SetElectricShock(data, l_drone_ptr->id);

                if(l_drone_ptr->GetId() == 0)
                {
                    m_soundManager_ptr->Play(l_drone_ptr, "Collisions");
                }
                else
                {
                    m_aiManager_ptr->RestartAIClock(l_drone_ptr->GetId());
                }

            }
        }
            
    }

    //============================================================
    //          KINETIC COLLISION: COLLISION BETWEEN DRONES
    //============================================================

    for(unsigned int i=0; i < m_RBCompDrones_list.size() ; i++)
    {

        Entity* l_rival_drone_ptr = m_entityManager_ptr->GetEntity(((IComponent*)m_RBCompDrones_list[i])->GetEntityId());
        TransformComponent* l_TCobstacle =  (TransformComponent*) m_entityManager_ptr->GetComponent(l_rival_drone_ptr->id, "Physics");

        if( m_RBCompDrones_list[i]->m_collision_b && l_RBdrone != m_RBCompDrones_list[i])
        {
            PICASSO::PhysicsVariables* data2 = l_TCobstacle->physicsDataPtr;
            glm::vec3 l_rivalPosition = l_rival_drone_ptr->m_NowED.pos + data2->v_velocity;

            collision = PICASSO::CheckSpheresCollision(&l_dronePosition, l_RBdrone->s_bounding.radius , &l_rivalPosition , m_RBCompDrones_list[i]->s_bounding.radius);
            if (collision){
                                        
                PICASSO::CalculateSpheresCollision(&data->v_velocity,data->v_col_velocity, &l_drone_ptr->m_NowED.pos, l_RBdrone->mass, &data2->v_velocity, data2->v_col_velocity, &l_rival_drone_ptr->m_NowED.pos, m_RBCompDrones_list[i]->mass);
                PICASSO::SetCollisionData(data, delta);
                PICASSO::SetCollisionData(data2, delta);
                
                if(l_drone_ptr->GetId() == 0 || l_rival_drone_ptr->GetId() == 0)
                {
                    m_soundManager_ptr->Play(l_drone_ptr, "Collisions");
                }
            }
        }
            
    }

    return collision;
}

void PhysicsManager::CollisionWithNextCheckpoint(Entity* l_drone_ptr, RigidBodyComponent* l_RBdrone, PositionComponent* l_dronePOS_ptr, PICASSO::PhysicsVariables* l_data_drone)
{
    //====== Get drone data =========
    int l_next_checkpoint_m = l_dronePOS_ptr->GetNextCheckpoint();    
    glm::vec3 l_dronePosition_v3 = l_drone_ptr->m_NowED.pos;

    //====== Get next checkpoint data ======
    int l_entityID_checkpoint_n = m_checkpointManager_ptr->GetComponent(l_next_checkpoint_m)->GetEntityId();
    RigidBodyComponent* l_RBcheckpoint = ((RigidBodyComponent*) m_entityManager_ptr->GetComponent(l_entityID_checkpoint_n, "RigidBody"));
    Entity* l_checkpoint_ptr = m_entityManager_ptr->GetEntity(l_entityID_checkpoint_n);
    glm::vec3 l_checkpointPosition_v3 = l_checkpoint_ptr->m_NowED.pos;
    
    //Check if there's collision between them
    glm::vec3 closestpoint = PICASSO::ClosestPoint( &l_RBcheckpoint->r_bounding.axis, &l_RBcheckpoint->r_bounding.size, &l_checkpointPosition_v3, &l_dronePosition_v3);
    bool collision = PICASSO::CheckSphereOBBCollision(&l_dronePosition_v3, &closestpoint, l_RBdrone->s_bounding.radius);
    
    //If there's collision 
    if (collision) 
    {
        //add checkpoint
        m_positionManager_ptr->AddCheckpoint(l_drone_ptr->id);

        //Set New Vertical Limit
        l_data_drone->verticalLimit = m_checkpointManager_ptr->GetComponent(l_next_checkpoint_m)->GetVerticalLimit();

        if(m_checkpointManager_ptr->GetComponent(l_next_checkpoint_m)->GetChangeZone() && l_drone_ptr->GetId() == 0)
        {
            InOut = !InOut;
            m_soundManager_ptr->SetEventVariableValue(l_drone_ptr, "Drone", "InOut", InOut);
        }
    }
}

void PhysicsManager::CalculatePowerUpCollision(Entity* l_powerUpEntity_ptr, Entity* l_drone_ptr)
{
    RigidBodyComponent* l_PowerUp_RBcomponent = ((RigidBodyComponent*)m_entityManager_ptr->GetComponent(l_powerUpEntity_ptr->id, "RigidBody"));
    RigidBodyComponent* l_Drone_RBcomponent = ((RigidBodyComponent*)m_entityManager_ptr->GetComponent(l_drone_ptr->id, "RigidBody"));

    //std::cout << "Tira freezing " << l_drone_ptr->name << "\n";
    
    for(int i=0; i < (int)m_RBCompDrones_list.size(); i++){
        
        if(m_RBCompDrones_list[i] != l_Drone_RBcomponent){
            Entity* l_drone_entity_ptr = m_entityManager_ptr->GetEntity(m_RBCompDrones_list[i]->GetEntityId());

            bool collision = PICASSO::CheckSpheresCollision(&l_powerUpEntity_ptr->m_NowED.pos, l_PowerUp_RBcomponent->s_bounding.radius , &l_drone_entity_ptr->m_NowED.pos , m_RBCompDrones_list[i]->s_bounding.radius);
            
            if (collision)
            {
                if(m_powerUpManager_ptr->CheckIfRivalHasReflector(m_entityManager_ptr->GetEntity(m_RBCompDrones_list[i]->GetEntityId())))
                    SetFreezing(((TransformComponent*)m_entityManager_ptr->GetComponent(l_drone_ptr->id, "Physics"))->physicsDataPtr, l_drone_ptr->id);         
                else
                    SetFreezing(((TransformComponent*)m_entityManager_ptr->GetComponent(m_RBCompDrones_list[i]->GetEntityId(), "Physics"))->physicsDataPtr, m_RBCompDrones_list[i]->GetEntityId());    
            }
            
        }       
    }
}

void PhysicsManager::SetElectricShock(PICASSO::PhysicsVariables* data, int l_entityID_n)
{
    
    Entity* l_rival_drone_ptr = m_entityManager_ptr->GetEntity(l_entityID_n);

    PICASSO::SetElectricShock(data, m_powerUpManager_ptr->CheckIfRivalHasShield(l_rival_drone_ptr));
}


void PhysicsManager::SetFreezing(PICASSO::PhysicsVariables* data, int l_entityID_n)
{
    Entity* l_rival_drone_ptr = m_entityManager_ptr->GetEntity(l_entityID_n);

    //std::cout << "Se congela: " << l_rival_drone_ptr->name << "\n";

    bool shield = m_powerUpManager_ptr->CheckIfRivalHasShield(l_rival_drone_ptr);

    if (!shield && l_rival_drone_ptr->GetId() == 0)
    {
        m_renderManager_ptr->ActivatePPEffect("freeze");
        m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "PowerUP", 3.0f);
        m_soundManager_ptr->SetEventVariableValue(l_rival_drone_ptr, "PowerUP", "ThrowReceive", 1.0f);
        m_soundManager_ptr->Play(l_rival_drone_ptr, "PowerUP");
    }
    
    PICASSO::SetFreezing(data, shield);
}

void PhysicsManager::UpdateKeys(float W, float A, float S, float D, float K, float I, float Space, int l_entityId_s)
{
    IComponent* physics = m_entityManager_ptr->GetComponent(l_entityId_s, "Physics");

    if(physics != nullptr)
    {
        PICASSO::PhysicsVariables* data = ((TransformComponent*) physics)->physicsDataPtr;

        PICASSO::UpdateW(W, data);
        PICASSO::UpdateA(A, data);
        PICASSO::UpdateS(S, data);
        PICASSO::UpdateD(D, data);
        PICASSO::UpdateK(K, data);
        PICASSO::UpdateI(I, data);
        PICASSO::UpdateSpace(Space, data);
    }
}

float PhysicsManager::DistanceToNextCheckpoint(RigidBodyComponent* RB_checkpoint, Entity* drone)
{
    Entity* checkpoint = m_entityManager_ptr->GetEntity(RB_checkpoint->GetEntityId());
    
    glm::vec3 closestpoint = PICASSO::ClosestPoint( &RB_checkpoint->r_bounding.axis, &RB_checkpoint->r_bounding.size, &checkpoint->m_NowED.pos, &drone->m_NowED.pos);
    return glm::distance(drone->m_NowED.pos,closestpoint);
}

int PhysicsManager::CalculateNearRivals(int l_entity_id, float l_radius_f)
{
    Entity* l_entity = m_entityManager_ptr->GetEntity(l_entity_id);
    int nearby_rivals = -1;

    for(int i=0; i < (int)m_RBCompDrones_list.size(); i++){

        Entity* l_drone_entity_ptr = m_entityManager_ptr->GetEntity(m_RBCompDrones_list[i]->GetEntityId());

        bool collision = PICASSO::CheckSpheresCollision(&l_entity->m_NowED.pos, l_radius_f, &l_drone_entity_ptr->m_NowED.pos , m_RBCompDrones_list[i]->s_bounding.radius);
        if (collision)
        {
            nearby_rivals++;
        }
    
    }

    return nearby_rivals;
}

void PhysicsManager::ResetNitro(PICASSO::PhysicsVariables* data)
{
    data->chargeNITRO = 0;
}

void NitroCharge(Event nitro)
{
    PICASSO::ChargeNitro(((TransformComponent*)nitro.m_eventData_ptr->g_entityManager_ptr->GetComponent(nitro.entity_pt->GetId(), "Physics"))->physicsDataPtr);
}

/**
 * @brief 
 * 
 * @return const Octree& 
 */
Octree* PhysicsManager::GetOctree()
{
    return octree;
}

/**
 * @brief: returns a vector with the Nitro Zones positions.
 * 
 * @return std::vector<glm::vec3> 
 */
std::map<int, glm::vec3> PhysicsManager::GetNitroZonesPositions()
{
    std::map<int, glm::vec3> l_nitroPositionsMap_m;

    for(size_t i = 0; i < m_RBNitroZones_v3.size(); i++)
    {
        l_nitroPositionsMap_m[m_RBNitroZones_v3[i]->GetEntityId()] = *m_RBNitroZones_v3[i]->getPosition();
    }
    return l_nitroPositionsMap_m;
}

/**
 * @brief: Returns a vector with all the Nitro Zones Ids.
 * 
 * @return std::vector<int> 
 */
std::vector<int> PhysicsManager::GetNitroZonesId()
{
    std::vector<int> l_nitroZonesIds_vn;

    l_nitroZonesIds_vn.reserve(m_RBNitroZones_v3.size());

    for(size_t i = 0; i < m_RBNitroZones_v3.size(); i++)
    {
        l_nitroZonesIds_vn.emplace_back(m_RBNitroZones_v3[i]->GetEntityId());
    }

    return l_nitroZonesIds_vn;
}

//==========================================================================
//==========================================================================
//                     SHADERS FUNCTIONS
//==========================================================================
//==========================================================================

float PhysicsManager::UpdatePerspective(PICASSO::PhysicsVariables* data, Entity* entity)
{
    //VALUE TO PASS TO NITRO SHADER 
    if(entity->GetId() == 0)
    {
        float extraVel = data->MaxVelocity - data->MaxVelocityStandar;
        float max_extraVel = data->MaxVelocityNITRO - data->MaxVelocityStandar;

        float value = extraVel / max_extraVel;
        float fov = 75.0f + (20.0f * value);

        m_renderManager_ptr->SetPerspectiveAngle(fov);

        return value;
    }

    return 0;
}
    
void PhysicsManager::UpdateShader(PICASSO::PhysicsVariables* data, Entity* entity, float value)
{    
    if(PICASSO::IsFrozen(data, true) == -1 && m_renderManager_ptr->GetEffectID() == "freeze")
    {
        //CHANGE TO REGULAR MESH
        m_renderManager_ptr->ActivatePPEffect("defaultPP");
        m_renderUtilities_ptr->ResetFreezeValues();
    }

    if(data->thief && data->thiefNITRO == 0)
    {
        data->thief = false;

        if(m_renderManager_ptr->GetEffectID() == "thief")
        {
            m_renderUtilities_ptr->ResetThiefValues();
            m_renderManager_ptr->ActivatePPEffect("defaultPP");
        }
    }

    if(entity->GetId() == 0)
    {
        if(recharging && (m_renderManager_ptr->GetEffectID() == "defaultPP" || m_renderManager_ptr->GetEffectID() == "nitro"))
        {
            m_renderManager_ptr->ActivatePPEffect("charging");
        }
        else if(!recharging && m_renderManager_ptr->GetEffectID() == "charging")
        {
            m_renderManager_ptr->ActivatePPEffect("defaultPP");
        }

        if(value > 0 && (m_renderManager_ptr->GetEffectID() == "defaultPP" || m_renderManager_ptr->GetEffectID() == "nitro"))
        {
            m_renderManager_ptr->ActivatePPEffect("nitro");
            m_renderUtilities_ptr->SetNitroValue(value);
        }
        else if(m_renderManager_ptr->GetEffectID() == "nitro")
        {
            m_renderManager_ptr->ActivatePPEffect("defaultPP");
            m_renderManager_ptr->SetPerspectiveAngle(75.0f);
        }
         
    }
}



//==========================================================================
//==========================================================================
//                     CLOCKS FUNCTIONS
//==========================================================================
//==========================================================================

void PhysicsManager::PauseClocks()
{
    for(int i=0; i < (int)m_TransComp_list.size(); i++)
    {
        PICASSO::PhysicsVariables* data = m_TransComp_list[i]->physicsDataPtr;

        if(data->electric_shock)
            data->electShock_clk->Pause();

        if(data->frozen)
            data->frozen_clk->Pause();
    }
}

void PhysicsManager::PlayClocks()
{
    for(int i=0; i < (int)m_TransComp_list.size(); i++)
    {
        PICASSO::PhysicsVariables* data = m_TransComp_list[i]->physicsDataPtr;

        if(data->electric_shock)
            data->electShock_clk->Play();
            
        if(data->frozen)
            data->frozen_clk->Play();
    }
}


