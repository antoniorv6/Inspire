
/**
 * @file AIDecisionComponent.hpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <AIDecisionComponent.hpp>
#include <MapManager.hpp>
#include <PowerUpManager.hpp>
#include <RigidBodyComponent.h>
#include <DebugGUIManager.hpp>
#include <PhysicsManager.h>
#include <TransformComponent.h>
#include <PositionManager.hpp>
#include <StackFSM.hpp>
#include <Path.hpp>
#include <SteerLib.hpp>
#include <FStates.hpp>
#include <SensorsLib.hpp>
#include <Octree.h>
#include <iterator>

/**
 * @brief Construct a new AIDecisionComponent::AIDecisionComponent object
 * 
 * @param l_componentId_s 
 * @param l_entityId_s 
 * @param l_waypointsVector 
 * @param force 
 */
AIDecisionComponent::AIDecisionComponent(const std::string& l_componentId_s, int l_entityId_s, const std::vector<WaypointComponent*>& l_waypointsVector, float force,
                                        int personality, GenericData *&l_genericData_ptr) : IComponent(l_componentId_s, l_entityId_s)
{
    m_entityId_n            = l_entityId_s;
    m_entityManager_ptr     = l_genericData_ptr->g_entityManager_ptr;
    m_physicsManager_ptr    = l_genericData_ptr->g_physicsManager_ptr;
    m_powerUpManager_ptr    = l_genericData_ptr->g_powerUpManager_ptr;
    m_mapManager_ptr        = l_genericData_ptr->g_mapManager_ptr;
    m_debugManager_ptr      = l_genericData_ptr->g_debugGuiManager_ptr;
    m_positionManager_ptr   = l_genericData_ptr->g_positionManager_ptr;
    m_personality_n         = personality;
    InitVariables();
    InitFSM();
    InitPath(l_waypointsVector);
    InitSteerModule(force);
    InitSensor(m_currentPosition_v3, m_currentVelocity_v3, m_rigidRadius_f);
    InitFuzzyModules();

    m_notCharge_ptr = new Clock();
    m_notCharge_ptr->Init();
    
   //m_debugManager_ptr->CreateComponent(m_entityManager_ptr->GetEntityByName("Player"), 20, 250, 350, 380);

   //IComponent * component = m_entityManager_ptr->GetComponent(0, "Physics"); 
   //TransformComponent* tr = ((TransformComponent*) component);
   //
   //playerData = tr->GetPhysicsData();

   //m_debugManager_ptr->AddTextToInterface(m_entityManager_ptr->GetEntityByName("Player"), std::to_string(playerData->accFWRD));
}

/**
 * @brief Destroy the AIDecisionComponent::AIDecisionComponent object
 * 
 */
AIDecisionComponent::~AIDecisionComponent()
{
    delete m_sfsm_ptr;
    delete m_steerModule_ptr;
    delete m_obstacleSensor_ptr;
    delete m_path_ptr;
    delete m_notCharge_ptr;

    m_sfsm_ptr = nullptr;
    m_steerModule_ptr = nullptr;
    m_obstacleSensor_ptr = nullptr;
    m_path_ptr = nullptr;
    m_notCharge_ptr = nullptr;
}

/**
 * @brief: Get all the necessary data for optimizations and the number of calls.
 * 
 */
void AIDecisionComponent::InitVariables()
{
    IComponent * component = m_entityManager_ptr->GetComponent(m_entityId_n, "Physics"); 
    TransformComponent* tr = ((TransformComponent*) component);
    
    m_physicsData_ptr = tr->GetPhysicsData();
    m_octree_ptr = m_physicsManager_ptr->GetOctree();

    component = m_entityManager_ptr->GetComponent(m_entityId_n, "RigidBody");
    m_rigidBody_ptr = ((RigidBodyComponent*) component);

    component = m_entityManager_ptr->GetComponent(m_entityId_n, "Position");
    m_position_ptr = ((PositionComponent*) component);

    m_mass_f                  = m_rigidBody_ptr->getMass();
    m_maxVelocity_f           = &m_physicsData_ptr->MaxVelocity;
    m_maxVelocityStandar_f    = &m_physicsData_ptr->MaxVelocityStandar;
    m_rigidRadius_f           = m_rigidBody_ptr->getRadius();
    m_currentPosition_v3      = &m_physicsData_ptr->currentPosition;
    m_currentVelocity_v3      = &m_physicsData_ptr->v_velocity;
    m_currentAcceleration_v3  = &m_physicsData_ptr->v_acceleration;
    m_nitroCharge_f           = &m_physicsData_ptr->chargeNITRO;
    m_nitroZonesMap_m         = m_physicsManager_ptr->GetNitroZonesPositions();
    m_attackness_f            = m_powerUpManager_ptr->GetDroneAttackValue(m_entityId_n);
    m_racePosition_n          = m_position_ptr->GetRefPosition();
    m_racePositionPlayer_n    = m_positionManager_ptr->GetPlayerPosition()->GetRefPosition();
    m_raceProgress_f          = m_position_ptr->GetRaceProgress();
    m_distanceNextRival_f     = m_position_ptr->GetDistanceNextRival();
    //m_nitroWaypointsMap_m     = m_mapManager_ptr->GetNitroWaypoints();
    m_nearbyRivals_n          = m_position_ptr->GetNearbyRivals();
    NitroZonesMapping();
}
/**
 * @brief 
 * 
 */
void AIDecisionComponent::InitFSM()
{
    m_sfsm_ptr = new StackFSM();
    m_sfsm_ptr->PushState(&Seek);
}  

/**
 * @brief 
 * 
 * @param l_waypointsVector 
 */
void AIDecisionComponent::InitPath(const std::vector<WaypointComponent*>& l_waypointsVector)
{
    //MapWaypointsToVec3(l_waypointsVector);
    std::vector<glm::vec3> waypoints = m_mapManager_ptr->GetPath();
    m_path_ptr = new SPath(waypoints.size(), 500.0f, true, waypoints);
}

/**
 * @brief 
 * 
 * @param l_force_f 
 */
void AIDecisionComponent::InitSteerModule(float l_force_f)
{
    m_steerModule_ptr = new SteerModule(m_mass_f,
                                        m_maxVelocity_f,
                                        l_force_f,
                                        m_currentPosition_v3,
                                        m_currentVelocity_v3);
}

/**
 * @brief: Initialize all the Fuzzy modules used.
 * 
 */
void AIDecisionComponent::InitFuzzyModules()
{
    InitVelocityModule();
    InitEnergyStrategyModule();
    InitUseNitroModule();
    InitChargingNitroModule();
    InitUsePowerUpModule();
    InitCalculateTendencyModule();
    InitTendencyModule();
}

/**
 * @brief 
 * 
 * @param l_position_v3 
 * @param l_velocity_v3 
 * @param l_rigidRadius_f 
 */
void AIDecisionComponent::InitSensor(glm::vec3* l_position_v3, glm::vec3* l_velocity_v3, float l_rigidRadius_f)
{
    m_obstacleSensor_ptr = new ObstacleSensor(l_position_v3, l_velocity_v3, l_rigidRadius_f);
}

/**
 * @brief 
 * 
 * @param l_waypointsVector 
 */
void AIDecisionComponent::MapWaypointsToVec3(const std::vector<WaypointComponent*>& l_waypointsVector)
{
    m_waypoints_v3.reserve(l_waypointsVector.size());

    for(size_t i = 0; i < l_waypointsVector.size(); i++)
    {
        m_waypoints_v3.emplace_back(l_waypointsVector[i]->GetPosition());
    }
} 

/**
 * @brief 
 * 
 */
void AIDecisionComponent::NitroZonesMapping()
{
    std::map<int, std::vector<glm::vec3>> l_tempNitroZonesMap = m_mapManager_ptr->GetNitroWaypoints();
    std::vector<int> l_nitroZonesId_vn = m_physicsManager_ptr->GetNitroZonesId();
    int l_counter_n = 0;

    for(auto const& x : l_tempNitroZonesMap)
    {
        m_nitroWaypointsMap_m[l_nitroZonesId_vn[l_counter_n]] = x.second;
        l_counter_n++;
    }
}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::Update()
{
   //m_debugManager_ptr->AddTextToInterface(m_entityManager_ptr->GetEntityByName("Player"),
   //"Posicion: " + std::to_string(playerData->currentPosition.x) + ", " 
   //+ std::to_string(playerData->currentPosition.y) + ", " + std::to_string(playerData->currentPosition.z));
    if(m_sfsm_ptr->GetCurrentState() == LOD){ m_sfsm_ptr->PopState(); };

    if(!m_physicsData_ptr->frozen && !m_physicsData_ptr->electric_shock)
    {
        m_futurePosition_v3 = CalculateFuturePosition2(*m_currentVelocity_v3, *m_currentPosition_v3, 150.0f);

        m_closestObjects_v3.clear();

        m_closestObjects_v3 = m_octree_ptr->GetCloseObjectsV3(m_rigidBody_ptr);

        m_sfsm_ptr->Update(this);
    }
}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::LODUpdate()
{
    if(m_sfsm_ptr->GetCurrentState() != LOD)
    {
        while(m_sfsm_ptr->GetSize() != 1){ m_sfsm_ptr->PopState(); }

        m_sfsm_ptr->PushState(&LOD);
    }

    m_sfsm_ptr->Update(this);
}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::CalculateLODVelocity()
{
    int l_difference = *m_racePositionPlayer_n - *m_racePosition_n;

    if( l_difference > 0 )
    {
        // IA is ahead of the Player  -->  Slow Velocity
        m_LODVelocity_f = *m_maxVelocityStandar_f - 5 - (2 * glm::abs(l_difference));
    }
    else
    {
        // IA goes behind the Player  -->  Fast Velocity
        m_LODVelocity_f = *m_maxVelocityStandar_f + 15 + (2 * glm::abs(l_difference)); 
    }
}

/**
 * @brief 
 * 
 * @param l_desiredVelocity_v3 
 * @param l_resultanVel_v3 
 */
void AIDecisionComponent::CalculateSpinRotation(const glm::vec3& l_desiredVelocity_v3, const glm::vec3& l_resultantVel_v3)
{
    /** Calculate the angle between the desired velocity vector and the resultar vector calculated by steering behaviors. */
    float l_expectedRotation = PICASSO::GetOrientedAngleBewteen2D(l_desiredVelocity_v3, l_resultantVel_v3);
    /** Limit this angle by the Max Side Rotation allowed by the Physics Engine. */
    LimitAngleByFactor(m_physicsData_ptr->MaxRotSIDE, l_expectedRotation);

    float l_difference = l_expectedRotation - m_physicsData_ptr->RotSIDE;
    float l_increment;

    if(glm::abs(l_difference) > 10.0f)
        l_increment = copysign(10.0f, l_difference);
    else
        l_increment = l_difference;

    /** Calculate the Side rotation. */
    m_physicsData_ptr->RotSIDE += l_increment;
    /** Calculate the Forward rotation. */
    m_physicsData_ptr->RotFWRD = - (glm::length(l_resultantVel_v3) * m_physicsData_ptr->MaxRotFWRD) / m_physicsData_ptr->MaxVelocity;
    /** Calculate angle spin */
    m_physicsData_ptr->RotSPIN = CalculateSpinAngle2(l_resultantVel_v3);
}

/**
 * @brief 
 * 
 * @param l_velocity_v3 
 * @param l_targetIndex_n 
 */
void AIDecisionComponent::CalculateFuzzyVelocity(glm::vec3& l_velocity_v3, int l_targetIndex_n)
{
    glm::vec3 l_lastWaypoint_v3        = m_path_ptr->GetTarget(l_targetIndex_n);
    glm::vec3 l_nextWaypoint_v3        = m_path_ptr->GetTarget(l_targetIndex_n + 1);
    glm::vec3 l_positionToWaypoint1_v3 = l_lastWaypoint_v3 - *m_currentPosition_v3;
    glm::vec3 l_positionToWaypoint2_v3 = l_nextWaypoint_v3 - *m_currentPosition_v3;
    float l_curvature_f = 0;

    if(m_steerModule_ptr->IncomingCurve())
    {
        l_curvature_f = PICASSO::GetAngleBetween2D2(glm::normalize(l_positionToWaypoint1_v3), glm::normalize(l_positionToWaypoint2_v3));
        if(std::isnan(l_curvature_f))
        {
            l_curvature_f = 0;
        }
        LimitFuzzyValues(l_curvature_f, 15.0f, 0.5f, 180.0f, 179.0f);
    }
    
    if(glm::distance(*m_currentPosition_v3, l_lastWaypoint_v3) > m_path_ptr->GetWaypointRadius() || l_curvature_f > 170)
    {
        m_steerModule_ptr->NotIncomingCurve();
        l_curvature_f = 0;
    }

    float l_velocityLenght_f = glm::length(l_velocity_v3);    
    LimitFuzzyValues(l_velocityLenght_f, 0, 0.5, *m_maxVelocity_f, *m_maxVelocity_f - 1.0f);

    m_velocityModule.Fuzzify("Curvature", l_curvature_f);
    m_velocityModule.Fuzzify("Velocity", l_velocityLenght_f);
    m_velocityModule.Fuzzify("Mass",m_mass_f);

    float l_crispValue_f = (float)m_velocityModule.Defuzzify("Acceleration");
    m_accelerationBalance = l_crispValue_f;
    *m_currentAcceleration_v3 = glm::normalize(l_velocity_v3);

    if(l_crispValue_f >= 0.0f && l_crispValue_f <= 0.5f)
    {
        l_crispValue_f    = 1 - l_crispValue_f;
        *m_currentAcceleration_v3 *= (float)(-1);
    }

    *m_currentAcceleration_v3        *= m_physicsData_ptr->MaxAcc * l_crispValue_f;                  
    m_physicsData_ptr->v_velocity     = l_velocity_v3;
    PICASSO::GetVectorVelocity(m_physicsData_ptr);
}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::CalculateNitro()
{
    if(m_physicsData_ptr->chargeNITRO > 0)
    {
        float l_desirability_f = CalculateUsingNitro(); 

        if(l_desirability_f > 0.5f)
        {
            //std::cout << "=========USO NITRO=========\n";
            PICASSO::DecreaseNitro(m_physicsData_ptr,1);
        }
        else
        {
            PICASSO::DecreaseMaxVelocity(m_physicsData_ptr,1);
        }
    }
    else
    {
        PICASSO::DecreaseMaxVelocity(m_physicsData_ptr,1);
    }
}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::CalculatePowerUp()
{
    //std::cout << this->GetEntityId() << " nitro: " << m_physicsData_ptr->chargeNITRO << std::endl;

    if(m_physicsData_ptr->chargeNITRO == 100)
    {
        float l_desirability_f = CalculateUsingPowerUp();
        float l_typePU_f; 

        //std::cout << this->GetEntityId() << " desire: " << l_desirability_f << std::endl;

        for(int i = 0; i < 2; i++)
        {
            l_typePU_f = m_powerUpManager_ptr->GetPowerUpTypeValue(m_entityId_n,i);
            //std::cout << this->GetEntityId() << " type: " << l_typePU_f << std::endl;
            if( glm::abs(l_desirability_f - l_typePU_f) < 0.25f )
            {
                m_powerUpManager_ptr->CreatePUEvent(m_entityId_n,i);
                return;
            }

        }
    }
    //std::cout << std::endl;
}

/**
 * @brief 
 * 
 * @return float 
 */
float AIDecisionComponent::GetGoToChargeDesirability(const std::map<int, glm::vec3>& l_nitroZones_v3)
{
    float l_max_f = MinFloat;

    for(auto const & nitroZoneCandidate : l_nitroZones_v3)
    {
        float l_temporalValue_f = CalculateChargingNitro(nitroZoneCandidate.second);

        if(l_temporalValue_f > l_max_f)
        {
            l_max_f = l_temporalValue_f;
            m_selectedNitroWaypoints_n = nitroZoneCandidate.first;
        }
    }
    return l_max_f;
}

/**
 * @brief 
 * 
 * @return float 
 */
float AIDecisionComponent::CalculateStrategyBalance()
{
    if(*m_raceProgress_f > 1.0f)
        *m_raceProgress_f = 1.0f;
    //std::cout << "\n";
    m_energyStrategyModule.Fuzzify("Attackness", m_attackness_f);
    m_energyStrategyModule.Fuzzify("Positioness", *m_racePosition_n);
    m_energyStrategyModule.Fuzzify("Progress", *m_raceProgress_f);
    float l_strategy = m_energyStrategyModule.Defuzzify("StrategyBalance");
    //m_energyStrategyModule.writeDOMs();
    //std::cout << this->GetEntityId() << " " << "StrategyBalance: " << l_strategy << std::endl;
    //std::cout << "\n";

    CalculateTendency(&l_strategy);
    
    return l_strategy;
}

void AIDecisionComponent::CalculateTendency(float* l_strategy)
{
    //std::cout << "\n";
    //std::cout << "Tendency: " << m_tendency_f << std::endl;
    m_tendencyModule.Fuzzify("Tendency", m_tendency_f);
    m_tendencyModule.Fuzzify("StrategyBalanceIN", *l_strategy);
    *l_strategy = m_tendencyModule.Defuzzify("StrategyBalanceOUT");
    //m_tendencyModule.writeDOMs();
    //std::cout << "StrategyBalanceTendency: " << *l_strategy << "\n" << std::endl;
    //std::cout << "==============================================";
    //std::cout << "\n";  
}

/**
 * @brief 
 * 
 * @param l_nitroPosition 
 * @return float 
 */
float AIDecisionComponent::CalculateChargingNitro(const glm::vec3& l_nitroPosition)
{
    float l_deviationDistance_f = CalculateDeviationDistance(*m_currentPosition_v3,
                                                              m_path_ptr->GetTarget(m_steerModule_ptr->GetCurrentTarget()),
                                                              l_nitroPosition
                                                            );
    // std::cout << "\n";
    //std::cout << "Deviation distance: " << l_deviationDistance_f << std::endl;
    m_chargingNitroModule.Fuzzify("DeviationDistance", l_deviationDistance_f);
    m_chargingNitroModule.Fuzzify("NitroCharge", *m_nitroCharge_f);
    m_chargingNitroModule.Fuzzify("StrategyBalance", m_strategyBalance);
    float l_chargeDecision_f = m_chargingNitroModule.Defuzzify("ChargingBalance");
    //m_chargingNitroModule.writeDOMs();
    //std::cout << "Charge Decision: " << l_chargeDecision_f << std::endl;
    // std::cout << "\n";

    return l_chargeDecision_f;
}

/**
 * @brief 
 * 
 * @return float 
 */
float AIDecisionComponent::CalculateUsingNitro()
{

    //std::cout << "\n";
    m_usingNitroModule.Fuzzify("Acceleration", m_accelerationBalance);
    m_usingNitroModule.Fuzzify("StrategyBalance", m_strategyBalance);
    float l_usingNitroValue_f = m_usingNitroModule.Defuzzify("UsingNitro");
    //m_usingNitroModule.writeDOMs();
    //std::cout << "Using Nitro Value: " << l_usingNitroValue_f  << std::endl;
    //std::cout << "\n";

    return l_usingNitroValue_f;
}

/**
 * @brief 
 * 
 * @return float 
 */
float AIDecisionComponent::CalculateUsingPowerUp()
{
    //std::cout << this->GetEntityId() << " nearby " << *m_nearbyRivals_n << std::endl;
    //std::cout << this->GetEntityId() << " distance " << *m_distanceNextRival_f << std::endl;
    //std::cout << this->GetEntityId() << " position " << *m_racePosition_n << std::endl;  
    //std::cout << "================================" << std::endl;          

    //std::cout << "\n";
    m_usingPowerUpModule.Fuzzify("NearbyRivals", *m_nearbyRivals_n);
    m_usingPowerUpModule.Fuzzify("DistanceNextRival", *m_distanceNextRival_f);  
    m_usingPowerUpModule.Fuzzify("Positioness", *m_racePosition_n);
    float l_usingPowerUpValue_f = m_usingPowerUpModule.Defuzzify("TypePUValue");
    //m_usingPowerUpModule.writeDOMs();
    //std::cout << "Using PowerUp Value: " << l_usingPowerUpValue_f << std::endl;
    //std::cout << "\n";

    return l_usingPowerUpValue_f;
}

/**
 * @brief: Initialize the Velocity Module for breaking purposes.
 * 
 */
void AIDecisionComponent::InitVelocityModule()
{
    /** REMEMBER, ADDING SETS TAKES THE FOLLOWING PARAMETERS:
     *  - Peak
     *  - MinBound
     *  - MaxBound
     */
    FLV& Curvature          = m_velocityModule.CreateFLV("Curvature");
    FSet Curvature_Low      = Curvature.AddLeftShoulderFuzzySet("Curvature_Low", 25, 0, 30);
    FSet Curvature_Medium   = Curvature.AddTriangleFuzzySet("Curvature_Medium", 40, 25, 60);
    FSet Curvature_High     = Curvature.AddRightShoulderFuzzySet("Curvature_High", 90, 50, 180);

    FLV& Mass               = m_velocityModule.CreateFLV("Mass");
    FSet Mass_Low           = Mass.AddLeftShoulderFuzzySet("Mass_Low", 0.75, 0, 1.5);
    FSet Mass_Medium        = Mass.AddTriangleFuzzySet("Mass_Medium", 0.75, 1.5, 2.25);
    FSet Mass_High          = Mass.AddRightShoulderFuzzySet("Mass_High", 1.5, 2.25, 3);

    FLV& Velocity           = m_velocityModule.CreateFLV("Velocity");
    FSet Velocity_Slow      = Velocity.AddLeftShoulderFuzzySet("Velocity_Slow", 0.25 * (*m_maxVelocity_f), 0, 0.5 * (*m_maxVelocity_f));
    FSet Velocity_Medium    = Velocity.AddTriangleFuzzySet("Velocity_Medium", 0.5 * (*m_maxVelocity_f), 0.25 * (*m_maxVelocity_f), 0.75 * (*m_maxVelocity_f));
    FSet Velocity_Fast      = Velocity.AddTriangleFuzzySet("Velocity_Fast", *m_maxVelocity_f, 0.5 * (*m_maxVelocity_f), (*m_maxVelocity_f));
    FSet Velocity_Nitro     = Velocity.AddRightShoulderFuzzySet("Velocity_Nitro", 1.05 * (*m_maxVelocity_f), *m_maxVelocity_f, 1.5 * (*m_maxVelocity_f));

    FLV& Acceleration       = m_velocityModule.CreateFLV("Acceleration"); 
    FSet Acc_Low            = Acceleration.AddLeftShoulderFuzzySet("Acc_Low", 0.25, 0, 0.5);
    FSet Acc_High           = Acceleration.AddTriangleFuzzySet("Acc_High", 0.5, 0.25, 0.75);
    FSet Acc_Full           = Acceleration.AddRightShoulderFuzzySet("Acc_Full", 0.75, 0.5, 1);

    FAND ander = FAND(Curvature_Medium, Mass_High); 
    m_velocityModule.AddRule(ander, Acc_Low);

    FAND ander1 = FAND(Curvature_Medium, Velocity_Fast);
    m_velocityModule.AddRule(ander1, Acc_Low);

    FAND ander2 = FAND(Curvature_High, Velocity_Fast);
    m_velocityModule.AddRule(ander2, Acc_Low);

    FAND ander3 = FAND(Curvature_Low, Velocity_Fast);
    m_velocityModule.AddRule(ander3, Acc_Full);

    FAND ander4 = FAND(Curvature_Medium, Velocity_Medium);
    m_velocityModule.AddRule(ander4, Acc_High);

    FAND ander5 = FAND(Curvature_High, Velocity_Medium);
    m_velocityModule.AddRule(ander5, Acc_Low);

    FAND ander6 = FAND(Curvature_Low, Velocity_Medium);
    m_velocityModule.AddRule(ander6, Acc_Full);

    FAND ander7 = FAND(Curvature_Medium, Velocity_Slow);
    m_velocityModule.AddRule(ander7, Acc_Full);

    FAND ander8 = FAND(Curvature_High, Velocity_Slow);
    m_velocityModule.AddRule(ander8, Acc_High);

    FAND ander9 = FAND(Curvature_Low, Velocity_Slow);
    m_velocityModule.AddRule(ander9, Acc_Full);

    FAND ander10 = FAND(Curvature_Medium, Velocity_Nitro);
    m_velocityModule.AddRule(ander10, Acc_Low);

    FAND ander11 = FAND(Curvature_High, Velocity_Nitro);
    m_velocityModule.AddRule(ander11, Acc_Low);

    FAND ander12 = FAND(Curvature_Low, Velocity_Nitro);
    m_velocityModule.AddRule(ander12, Acc_Full);
}

/**
 * 
 * @brief 
 * 
 */
void AIDecisionComponent::InitEnergyStrategyModule()
{
    FLV& Attackness             = m_energyStrategyModule.CreateFLV("Attackness");
    FSet Attackness_Low         = Attackness.AddLeftShoulderFuzzySet("Attackness_Low", 2, 0, 5);
    FSet Attackness_Medium      = Attackness.AddTriangleFuzzySet("Attackness_Medium", 5, 3, 7);
    FSet Attackness_High        = Attackness.AddRightShoulderFuzzySet("Attackness_High", 8, 5, 10);

    FLV& Progress               = m_energyStrategyModule.CreateFLV("Progress");
    FSet Progress_Low           = Progress.AddLeftShoulderFuzzySet("Progress_Low", 0.15, 0, 0.5);
    FSet Progress_Medium        = Progress.AddTriangleFuzzySet("Progress_Medium", 0.5, 0.2, 0.8);
    FSet Progress_High          = Progress.AddRightShoulderFuzzySet("Progress_High", 0.85, 0.5, 1);

    FLV& Positioness            = m_energyStrategyModule.CreateFLV("Positioness");
    FSet Positioness_Excelent   = Positioness.AddTriangleFuzzySet("Positioness_Excelent", 1, 1, 2.5);
    FSet Positioness_Well       = Positioness.AddTriangleFuzzySet("Positioness_Well", 2.5, 1.5, 3.5);
    FSet Positioness_Bad        = Positioness.AddTriangleFuzzySet("Positioness_Bad", 4, 2.5, 4);

    FLV& StrategyBalance        = m_energyStrategyModule.CreateFLV("StrategyBalance"); 
    FSet SB_powerUp             = StrategyBalance.AddLeftShoulderFuzzySet("SB_powerUp", 0.25, 0, 0.5);
    FSet SB_balanced            = StrategyBalance.AddTriangleFuzzySet("SB_balanced", 0.5, 0.25, 0.75);
    FSet SB_nitro               = StrategyBalance.AddRightShoulderFuzzySet("SB_nitro", 0.75, 0.5, 1);

    FAND ander  = FAND(Attackness_Low, Progress_Low); // ok
    FAND ander1 = FAND(ander, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander1,  SB_powerUp);

    FAND ander2 = FAND(Attackness_Low, Progress_Medium); // ok
    FAND ander3 = FAND(ander2, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander3,  SB_powerUp);

    FAND ander4 = FAND(Attackness_Low, Progress_High); // ok
    FAND ander5 = FAND(ander4, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander5,  SB_balanced); // maybe nitro

    FAND ander6 = FAND(Attackness_Medium, Progress_Low); // ok
    FAND ander7 = FAND(ander6, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander7,  SB_powerUp);

    FAND ander8 = FAND(Attackness_Medium, Progress_Medium); // ok 
    FAND ander9 = FAND(ander8, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander9,  SB_balanced);

    FAND ander10 = FAND(Attackness_Medium, Progress_High); // ok
    FAND ander11 = FAND(ander10, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander11,  SB_nitro);

    FAND ander12 = FAND(Attackness_High, Progress_Low); // ok
    FAND ander13 = FAND(ander12, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander13,  SB_nitro);

    FAND ander14 = FAND(Attackness_High, Progress_Medium); // ok
    FAND ander15 = FAND(ander14, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander15,  SB_nitro);

    FAND ander16 = FAND(Attackness_High, Progress_High); // ok
    FAND ander17 = FAND(ander16, Positioness_Excelent);
    m_energyStrategyModule.AddRule(ander17,  SB_nitro);

    // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    FAND ander18 = FAND(Attackness_Low, Progress_Low); // ok
    FAND ander19 = FAND(ander18, Positioness_Well);
    m_energyStrategyModule.AddRule(ander19,  SB_balanced); // maybe nitro

    FAND ander20 = FAND(Attackness_Low, Progress_Medium); // ok
    FAND ander21 = FAND(ander20, Positioness_Well);
    m_energyStrategyModule.AddRule(ander21,  SB_nitro);

    FAND ander22 = FAND(Attackness_Low, Progress_High); // ok
    FAND ander23 = FAND(ander22, Positioness_Well);
    m_energyStrategyModule.AddRule(ander23,  SB_nitro);

    FAND ander24 = FAND(Attackness_Medium, Progress_Low); // ok
    FAND ander25 = FAND(ander24, Positioness_Well);
    m_energyStrategyModule.AddRule(ander25,  SB_powerUp);

    FAND ander26 = FAND(Attackness_Medium, Progress_Medium); // ok
    FAND ander27 = FAND(ander26, Positioness_Well);
    m_energyStrategyModule.AddRule(ander27,  SB_balanced); // maybe PU

    FAND ander28 = FAND(Attackness_Medium, Progress_High); // ok
    FAND ander29 = FAND(ander28, Positioness_Well);
    m_energyStrategyModule.AddRule(ander29,  SB_balanced); // maybe nitro

    FAND ander30 = FAND(Attackness_High, Progress_Low); // ok
    FAND ander31 = FAND(ander30, Positioness_Well);
    m_energyStrategyModule.AddRule(ander31,  SB_powerUp);

    FAND ander32 = FAND(Attackness_High, Progress_Medium); // ok
    FAND ander33 = FAND(ander32, Positioness_Well);
    m_energyStrategyModule.AddRule(ander33,  SB_powerUp);

    FAND ander34 = FAND(Attackness_High, Progress_High); // ok
    FAND ander35 = FAND(ander34, Positioness_Well);
    m_energyStrategyModule.AddRule(ander35,  SB_balanced);

    // BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB

    FAND ander36 = FAND(Attackness_Low, Progress_Low); // ok
    FAND ander37 = FAND(ander36, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander37,  SB_nitro);

    FAND ander38 = FAND(Attackness_Low, Progress_Medium); // ok
    FAND ander39 = FAND(ander38, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander39,  SB_nitro);

    FAND ander40 = FAND(Attackness_Low, Progress_High); // ok
    FAND ander41 = FAND(ander40, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander41,  SB_nitro);

    FAND ander42 = FAND(Attackness_Medium, Progress_Low); // ok
    FAND ander43 = FAND(ander42, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander43,  SB_balanced); // maybe nitro

    FAND ander44 = FAND(Attackness_Medium, Progress_Medium); // ok
    FAND ander45 = FAND(ander44, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander45,  SB_nitro);

    FAND ander46 = FAND(Attackness_Medium, Progress_High); // ok
    FAND ander47 = FAND(ander46, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander47,  SB_nitro);

    FAND ander48 = FAND(Attackness_High, Progress_Low); // ok
    FAND ander49 = FAND(ander48, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander49,  SB_powerUp);

    FAND ander50 = FAND(Attackness_High, Progress_Medium); // ok
    FAND ander51 = FAND(ander50, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander51,  SB_powerUp);

    FAND ander52 = FAND(Attackness_High, Progress_High); // ok
    FAND ander53 = FAND(ander52, Positioness_Bad);
    m_energyStrategyModule.AddRule(ander53,  SB_balanced); //maybe PU
}

void AIDecisionComponent::InitTendencyModule()
{
    FLV& Tendency                = m_tendencyModule.CreateFLV("Tendency"); 
    FSet Tendency_powerUp        = Tendency.AddLeftShoulderFuzzySet("Tendency_powerUp", 0.3, 0, 0.5);
    FSet Tendency_balanced       = Tendency.AddTriangleFuzzySet("Tendency_balanced", 0.5, 0.3, 0.7);
    FSet Tendency_nitro          = Tendency.AddRightShoulderFuzzySet("Tendency_nitro", 0.7, 0.5, 1);

    FLV& StrategyBalanceIN        = m_tendencyModule.CreateFLV("StrategyBalanceIN"); 
    FSet SBin_powerUp             = StrategyBalanceIN.AddLeftShoulderFuzzySet("SBinin_powerUp", 0.25, 0, 0.5);
    FSet SBin_balanced            = StrategyBalanceIN.AddTriangleFuzzySet("SBinin_balanced", 0.5, 0.25, 0.75);
    FSet SBin_nitro               = StrategyBalanceIN.AddRightShoulderFuzzySet("SBin_nitro", 0.75, 0.5, 1);

    FLV& StrategyBalanceOUT        = m_tendencyModule.CreateFLV("StrategyBalanceOUT"); 
    FSet SBout_powerUp             = StrategyBalanceOUT.AddLeftShoulderFuzzySet("SBoutout_powerUp", 0.25, 0, 0.5);
    FSet SBout_balanced            = StrategyBalanceOUT.AddTriangleFuzzySet("SBoutout_balanced", 0.5, 0.25, 0.75);
    FSet SBout_nitro               = StrategyBalanceOUT.AddRightShoulderFuzzySet("SBout_nitro", 0.75, 0.5, 1);

    FAND tendency_rule = FAND(SBin_balanced, Tendency_nitro);
    m_tendencyModule.AddRule(tendency_rule, SBout_nitro);

    FAND tendency_rule2 = FAND(SBin_powerUp, Tendency_nitro);
    m_tendencyModule.AddRule(tendency_rule2, SBout_powerUp);

    FAND tendency_rule3 = FAND(SBin_nitro, Tendency_nitro);
    m_tendencyModule.AddRule(tendency_rule3, SBout_nitro);

    FAND tendency_rule4 = FAND(SBin_balanced, Tendency_powerUp);
    m_tendencyModule.AddRule(tendency_rule4, SBout_powerUp);

    FAND tendency_rule5 = FAND(SBin_powerUp, Tendency_powerUp);
    m_tendencyModule.AddRule(tendency_rule5, SBout_powerUp);

    FAND tendency_rule6 = FAND(SBin_nitro, Tendency_powerUp);
    m_tendencyModule.AddRule(tendency_rule6, SBout_nitro);

    FAND tendency_rule7 = FAND(SBin_balanced, Tendency_balanced);
    m_tendencyModule.AddRule(tendency_rule7, SBout_balanced);

    FAND tendency_rule8 = FAND(SBin_powerUp, Tendency_balanced);
    m_tendencyModule.AddRule(tendency_rule8, SBout_powerUp);

    FAND tendency_rule9 = FAND(SBin_nitro, Tendency_balanced);
    m_tendencyModule.AddRule(tendency_rule9, SBout_nitro);
}

void AIDecisionComponent::InitChargingNitroModule()
{
    FLV& DeviationDistance         = m_chargingNitroModule.CreateFLV("DeviationDistance");
    FSet DeviationDistance_Low     = DeviationDistance.AddLeftShoulderFuzzySet("DeviationDistance_Low", 500, 0, 800);
    FSet DeviationDistance_Medium  = DeviationDistance.AddTriangleFuzzySet("DeviationDistance_Medium", 1000, 500, 1600);
    FSet DeviationDistance_High    = DeviationDistance.AddRightShoulderFuzzySet("DeviationDistance_High", 1600, 1000, MaxFloat);

    FLV& NitroCharge               = m_chargingNitroModule.CreateFLV("NitroCharge"); 
    FSet NitroCharge_Low           = NitroCharge.AddLeftShoulderFuzzySet("NitroCharge_Low", 25, 0, 50);
    FSet NitroCharge_Medium        = NitroCharge.AddTriangleFuzzySet("NitroCharge_Medium", 50, 25, 75);
    FSet NitroCharge_High          = NitroCharge.AddTriangleFuzzySet("NitroCharge_High", 99, 50, 99);
    FSet NitroCharge_Full          = NitroCharge.AddTriangleFuzzySet("NitroCharge_Full", 100, 99, 100);

    FLV& StrategyBalance           = m_chargingNitroModule.CreateFLV("StrategyBalance"); 
    FSet SB_powerUp                = StrategyBalance.AddLeftShoulderFuzzySet("SB_powerUp", 0.25, 0, 0.5);
    FSet SB_balanced               = StrategyBalance.AddTriangleFuzzySet("SB_balanced", 0.5, 0.25, 0.75);
    FSet SB_nitro                  = StrategyBalance.AddRightShoulderFuzzySet("SB_nitro", 0.75, 0.5, 1);

    FLV& ChargingBalance           = m_chargingNitroModule.CreateFLV("ChargingBalance"); 
    FSet ChargingBalance_NotCharge = ChargingBalance.AddLeftShoulderFuzzySet("ChargingBalance_NotCharge", 0.25, 0, 0.5);
    FSet ChargingBalance_Balanced  = ChargingBalance.AddTriangleFuzzySet("ChargingBalance_Balanced", 0.5, 0.25, 0.75);
    FSet ChargingBalance_Charge    = ChargingBalance.AddRightShoulderFuzzySet("ChargingBalance_Charge", 0.75, 0.5, 1);

    

    FAND ander  = FAND(DeviationDistance_Low, NitroCharge_Low); // ok
    FAND ander1 = FAND(ander, SB_powerUp);
    m_chargingNitroModule.AddRule(ander1,  ChargingBalance_Charge);

    FAND ander2 = FAND(DeviationDistance_Low, NitroCharge_Medium); // ok
    FAND ander3 = FAND(ander2, SB_powerUp);
    m_chargingNitroModule.AddRule(ander3,  ChargingBalance_Charge);

    FAND ander4 = FAND(DeviationDistance_Low, NitroCharge_High); // ok
    FAND ander5 = FAND(ander4, SB_powerUp);
    m_chargingNitroModule.AddRule(ander5,  ChargingBalance_Charge);

    FAND ander6 = FAND(DeviationDistance_Medium, NitroCharge_Low); // ok
    FAND ander7 = FAND(ander6, SB_powerUp);
    m_chargingNitroModule.AddRule(ander7,  ChargingBalance_Charge);

    FAND ander8 = FAND(DeviationDistance_Medium, NitroCharge_Medium); // ok 
    FAND ander9 = FAND(ander8, SB_powerUp);
    m_chargingNitroModule.AddRule(ander9,  ChargingBalance_Charge);

    FAND ander10 = FAND(DeviationDistance_Medium, NitroCharge_High); // ok
    FAND ander11 = FAND(ander10, SB_powerUp);
    m_chargingNitroModule.AddRule(ander11,  ChargingBalance_Charge);

    FAND ander12 = FAND(DeviationDistance_High, NitroCharge_Low); // ok
    FAND ander13 = FAND(ander12, SB_powerUp);
    m_chargingNitroModule.AddRule(ander13,  ChargingBalance_Charge);

    FAND ander14 = FAND(DeviationDistance_High, NitroCharge_Medium); // ok
    FAND ander15 = FAND(ander14, SB_powerUp);
    m_chargingNitroModule.AddRule(ander15,  ChargingBalance_Balanced); // MAYBE CHARGE

    FAND ander16 = FAND(DeviationDistance_High, NitroCharge_High); // ok
    FAND ander17 = FAND(ander16, SB_powerUp);
    m_chargingNitroModule.AddRule(ander17,  ChargingBalance_Balanced);

    // ======================================================================

    FAND ander18 = FAND(DeviationDistance_Low, NitroCharge_Low); // ok
    FAND ander19 = FAND(ander18, SB_balanced);
    m_chargingNitroModule.AddRule(ander19,  ChargingBalance_Charge);

    FAND ander20 = FAND(DeviationDistance_Low, NitroCharge_Medium); // ok
    FAND ander21 = FAND(ander20, SB_balanced);
    m_chargingNitroModule.AddRule(ander21,  ChargingBalance_Charge);

    FAND ander22 = FAND(DeviationDistance_Low, NitroCharge_High); // ok
    FAND ander23 = FAND(ander22, SB_balanced);
    m_chargingNitroModule.AddRule(ander23,  ChargingBalance_Charge);

    FAND ander24 = FAND(DeviationDistance_Medium, NitroCharge_Low); // ok
    FAND ander25 = FAND(ander24, SB_balanced);
    m_chargingNitroModule.AddRule(ander25,  ChargingBalance_Charge);

    FAND ander26 = FAND(DeviationDistance_Medium, NitroCharge_Medium); // ok
    FAND ander27 = FAND(ander26, SB_balanced);
    m_chargingNitroModule.AddRule(ander27,  ChargingBalance_Balanced); 

    FAND ander28 = FAND(DeviationDistance_Medium, NitroCharge_High); // ok
    FAND ander29 = FAND(ander28, SB_balanced);
    m_chargingNitroModule.AddRule(ander29,  ChargingBalance_Balanced); 

    FAND ander30 = FAND(DeviationDistance_High, NitroCharge_Low); // ok
    FAND ander31 = FAND(ander30, SB_balanced);
    m_chargingNitroModule.AddRule(ander31,  ChargingBalance_Balanced);

    FAND ander32 = FAND(DeviationDistance_High, NitroCharge_Medium); // ok
    FAND ander33 = FAND(ander32, SB_balanced);
    m_chargingNitroModule.AddRule(ander33,  ChargingBalance_Balanced);

    FAND ander34 = FAND(DeviationDistance_High, NitroCharge_High); // ok
    FAND ander35 = FAND(ander34, SB_balanced);
    m_chargingNitroModule.AddRule(ander35,  ChargingBalance_NotCharge);

    // ==============================================================

    FAND ander36 = FAND(DeviationDistance_Low, NitroCharge_Low); // ok
    FAND ander37 = FAND(ander36, SB_nitro);
    m_chargingNitroModule.AddRule(ander37,  ChargingBalance_Charge);

    FAND ander38 = FAND(DeviationDistance_Low, NitroCharge_Medium); // ok
    FAND ander39 = FAND(ander38, SB_nitro);
    m_chargingNitroModule.AddRule(ander39,  ChargingBalance_Charge);

    FAND ander40 = FAND(DeviationDistance_Low, NitroCharge_High); // ok
    FAND ander41 = FAND(ander40, SB_nitro);
    m_chargingNitroModule.AddRule(ander41,  ChargingBalance_Charge);

    FAND ander42 = FAND(DeviationDistance_Medium, NitroCharge_Low); // ok
    FAND ander43 = FAND(ander42, SB_nitro);
    m_chargingNitroModule.AddRule(ander43,  ChargingBalance_Balanced);

    FAND ander44 = FAND(DeviationDistance_Medium, NitroCharge_Medium); // ok
    FAND ander45 = FAND(ander44, SB_nitro);
    m_chargingNitroModule.AddRule(ander45,  ChargingBalance_Balanced);

    FAND ander46 = FAND(DeviationDistance_Medium, NitroCharge_High); // ok
    FAND ander47 = FAND(ander46, SB_nitro);
    m_chargingNitroModule.AddRule(ander47,  ChargingBalance_NotCharge);

    FAND ander48 = FAND(DeviationDistance_High, NitroCharge_Low); // ok
    FAND ander49 = FAND(ander48, SB_nitro);
    m_chargingNitroModule.AddRule(ander49,  ChargingBalance_Balanced);

    FAND ander50 = FAND(DeviationDistance_High, NitroCharge_Medium); // ok
    FAND ander51 = FAND(ander50, SB_nitro);
    m_chargingNitroModule.AddRule(ander51,  ChargingBalance_Balanced);

    FAND ander52 = FAND(DeviationDistance_High, NitroCharge_High); // ok
    FAND ander53 = FAND(ander52, SB_nitro);
    m_chargingNitroModule.AddRule(ander53,  ChargingBalance_NotCharge);

    m_chargingNitroModule.AddRule(NitroCharge_Full, ChargingBalance_NotCharge);
}

void AIDecisionComponent::InitUseNitroModule()
{
    FLV& Acceleration      = m_usingNitroModule.CreateFLV("Acceleration"); 
    FSet Acc_Low           = Acceleration.AddLeftShoulderFuzzySet("Acc_Low", 0.25, 0, 0.5);
    FSet Acc_High          = Acceleration.AddTriangleFuzzySet("Acc_High", 0.5, 0.25, 0.75);
    FSet Acc_Full          = Acceleration.AddRightShoulderFuzzySet("Acc_Full", 0.75, 0.5, 1);

    FLV& StrategyBalance   = m_usingNitroModule.CreateFLV("StrategyBalance"); 
    FSet SB_powerUp        = StrategyBalance.AddLeftShoulderFuzzySet("SB_powerUp", 0.25, 0, 0.5);
    FSet SB_balanced       = StrategyBalance.AddTriangleFuzzySet("SB_balanced", 0.5, 0.25, 0.75);
    FSet SB_nitro          = StrategyBalance.AddRightShoulderFuzzySet("SB_nitro", 0.75, 0.5, 1);

    FLV& UsingNitro        = m_usingNitroModule.CreateFLV("UsingNitro"); 
    FSet UsingNitro_NotUse = UsingNitro.AddLeftShoulderFuzzySet("UsingNitro_NotUse", 0.25, 0, 0.5);
    FSet UsingNitro_Use    = UsingNitro.AddRightShoulderFuzzySet("UsingNitro_Use", 0.75, 0.5, 1);


    FAND ander = FAND(Acc_Low, SB_powerUp);
    m_usingNitroModule.AddRule(ander, UsingNitro_NotUse);

    FAND ander2 = FAND(Acc_Low, SB_balanced);
    m_usingNitroModule.AddRule(ander2, UsingNitro_NotUse);

    FAND ander3 = FAND(Acc_Low, SB_nitro);
    m_usingNitroModule.AddRule(ander3, UsingNitro_NotUse);

    FAND ander4 = FAND(Acc_High, SB_powerUp);
    m_usingNitroModule.AddRule(ander4, UsingNitro_NotUse);    

    FAND ander5 = FAND(Acc_High, SB_balanced);
    m_usingNitroModule.AddRule(ander5, UsingNitro_NotUse);

    FAND ander6 = FAND(Acc_High, SB_nitro);
    m_usingNitroModule.AddRule(ander6, UsingNitro_NotUse);

    FAND ander7 = FAND(Acc_Full, SB_powerUp);
    m_usingNitroModule.AddRule(ander7, UsingNitro_NotUse);

    FAND ander8 = FAND(Acc_Full, SB_balanced);
    m_usingNitroModule.AddRule(ander8, UsingNitro_NotUse);

    FAND ander9 = FAND(Acc_Full, SB_nitro);
    m_usingNitroModule.AddRule(ander9, UsingNitro_Use);
}

void AIDecisionComponent::InitUsePowerUpModule()
{
    FLV& NearbyRivals           = m_usingPowerUpModule.CreateFLV("NearbyRivals"); 
    FSet Rivals_One             = NearbyRivals.AddTriangleFuzzySet("Rivals_One", 1, 0, 1.5);
    FSet Rivals_Some            = NearbyRivals.AddRightShoulderFuzzySet("Rivals_Some", 2, 1.5, 6);

    FLV& DistanceNextRival      = m_usingPowerUpModule.CreateFLV("DistanceNextRival"); 
    FSet Distance_Close         = DistanceNextRival.AddLeftShoulderFuzzySet("Distance_Close", 1900, 0, 2100);
    FSet Distance_Far           = DistanceNextRival.AddRightShoulderFuzzySet("Distance_Far", 2100, 2000, MaxFloat);

    FLV& Positioness            = m_usingPowerUpModule.CreateFLV("Positioness");
    FSet Positioness_Excelent   = Positioness.AddTriangleFuzzySet("Positioness_Excelent", 1, 1, 2.4);
    FSet Positioness_Well       = Positioness.AddTriangleFuzzySet("Positioness_Well", 2.5, 1.5, 3.5);
    FSet Positioness_Bad        = Positioness.AddTriangleFuzzySet("Positioness_Bad", 4, 2.6, 4);

    FLV& TypePUValue            = m_usingPowerUpModule.CreateFLV("TypePUValue");
    FSet Type_None              = TypePUValue.AddTriangleFuzzySet("Type_None", 0, 0, 0.5);
    FSet Type_AttackArea        = TypePUValue.AddTriangleFuzzySet("Type_AttackArea", 4, 3.5, 4.5);
    FSet Type_AttackIndividual  = TypePUValue.AddTriangleFuzzySet("Type_AttackIndividual", 3, 2.5, 3.5);
    FSet Type_DefenseClose      = TypePUValue.AddTriangleFuzzySet("Type_DefenseClose", 2, 1.5, 2.5);
    FSet Type_DefenseFar        = TypePUValue.AddTriangleFuzzySet("Type_DefenseFar", 1, 0.5, 1.5);

    /*You are the first*/
    FAND ander = FAND(Distance_Close, Positioness_Excelent);
    m_usingPowerUpModule.AddRule(ander, Type_DefenseClose);

    FAND ander2 = FAND(Distance_Far, Positioness_Excelent);
    m_usingPowerUpModule.AddRule(ander2, Type_DefenseFar);

    /*You are a loser*/
    FAND ander3 = FAND(Rivals_One, Positioness_Bad);
    m_usingPowerUpModule.AddRule(ander3, Type_AttackIndividual);

    FAND ander4 = FAND(Rivals_Some, Positioness_Bad);
    m_usingPowerUpModule.AddRule(ander4, Type_AttackArea);

    FAND ander8 = FAND(Distance_Far, Positioness_Bad);
    m_usingPowerUpModule.AddRule(ander8, Type_None);

    /*You are not the first but good*/
    FAND ander5 = FAND(Rivals_Some, Positioness_Well);
    m_usingPowerUpModule.AddRule(ander5, Type_AttackArea);

    FAND ander6 = FAND(Rivals_One, Positioness_Well);
    m_usingPowerUpModule.AddRule(ander6, Type_AttackIndividual);

    FAND ander7 = FAND(Distance_Far, Positioness_Well);
    m_usingPowerUpModule.AddRule(ander7, Type_None);
}

void AIDecisionComponent::InitCalculateTendencyModule()
{
    FLV& Attackness              = m_calculateTendencyModule.CreateFLV("Attackness");
    FSet Attackness_Low          = Attackness.AddLeftShoulderFuzzySet("Attackness_Low", 2, 0, 5);
    FSet Attackness_Medium       = Attackness.AddTriangleFuzzySet("Attackness_Medium", 5, 3, 7);
    FSet Attackness_High         = Attackness.AddRightShoulderFuzzySet("Attackness_High", 8, 5, 10);

    FLV& Personality             = m_calculateTendencyModule.CreateFLV("Personality");
    FSet Personality_Defense     = Personality.AddLeftShoulderFuzzySet("Personality_Defense", 2, 0, 5);
    FSet Personality_Balanced    = Personality.AddTriangleFuzzySet("Personality_Balanced", 5, 3, 7);
    FSet Personality_Attack      = Personality.AddRightShoulderFuzzySet("Personality_Attack", 8, 5, 10);

    FLV& Tendency                = m_calculateTendencyModule.CreateFLV("Tendency"); 
    FSet Tendency_powerUp        = Tendency.AddLeftShoulderFuzzySet("Tendency_powerUp", 0.25, 0, 0.5);
    FSet Tendency_balanced       = Tendency.AddTriangleFuzzySet("Tendency_balanced", 0.5, 0.25, 0.75);
    FSet Tendency_nitro          = Tendency.AddRightShoulderFuzzySet("Tendency_nitro", 0.75, 0.5, 1);

    FAND ander1 = FAND(Attackness_Low, Personality_Defense);
    m_calculateTendencyModule.AddRule(ander1, Tendency_powerUp);

    FAND ander2 = FAND(Attackness_Medium, Personality_Defense);
    m_calculateTendencyModule.AddRule(ander2, Tendency_balanced);

    FAND ander3 = FAND(Attackness_High, Personality_Defense);
    m_calculateTendencyModule.AddRule(ander3, Tendency_nitro);

    FAND ander4 = FAND(Attackness_Low, Personality_Balanced);
    m_calculateTendencyModule.AddRule(ander4, Tendency_balanced);

    FAND ander5 = FAND(Attackness_Medium, Personality_Balanced);
    m_calculateTendencyModule.AddRule(ander5, Tendency_balanced);

    FAND ander6 = FAND(Attackness_High, Personality_Balanced);
    m_calculateTendencyModule.AddRule(ander6, Tendency_balanced);

    FAND ander7 = FAND(Attackness_Low, Personality_Attack);
    m_calculateTendencyModule.AddRule(ander7, Tendency_nitro);

    FAND ander8 = FAND(Attackness_Medium, Personality_Attack);
    m_calculateTendencyModule.AddRule(ander8, Tendency_balanced);

    FAND ander9 = FAND(Attackness_High, Personality_Attack);
    m_calculateTendencyModule.AddRule(ander9, Tendency_powerUp);

    //std::cout << "\n";
    //std::cout << "Attackness: " << m_attackness_f << "\n";
    //std::cout << "personality: " << m_personality_n << "\n";
    m_calculateTendencyModule.Fuzzify("Attackness", m_attackness_f);
    m_calculateTendencyModule.Fuzzify("Personality", m_personality_n);
    m_tendency_f = m_calculateTendencyModule.Defuzzify("Tendency");
    //m_calculateTendencyModule.writeDOMs();
    //std::cout << "Tendency Value: " << m_tendency_f  << std::endl;
    //std::cout << "\n";

}

/**
 * @brief 
 * 
 */
void AIDecisionComponent::CraigReynoldsFromTarget()
{
        glm::vec3 l_targetAux_v3 = glm::vec3(0,0,0);

        float l_minDistance_f = MaxFloat;
        /** Loop through all the Waypoints to search for the best Target candidate */
        for (unsigned int i = 0; i < m_path_ptr->GetPathSize(); i++){
           
            /** Evaluate segments AB to calculate the NormalPoint (Scalar projection) */
            glm::vec3 l_pA             = m_path_ptr->m_waypoints_v3[i];
            glm::vec3 l_pB             = m_path_ptr->m_waypoints_v3[(i + 1) % m_path_ptr->m_waypointsCount_n];
            glm::vec3 l_normalPoint_v3 = CalculateNormalPoint(l_pA, l_pB, m_futurePosition_v3);
 
            /** Set the direction = segment's orientation */
            glm::vec3 l_dir_v3 = l_pB - l_pA;
 
            /** Check if the NormalPoint is on the segment
             *
             *  If not -> Look to the end point (Point B) of the segment
             *            and change the direction
            */
            if (l_normalPoint_v3.x < std::min(l_pA.x,l_pB.x) || l_normalPoint_v3.x > std::max(l_pA.x,l_pB.x)
            || l_normalPoint_v3.y < std::min(l_pA.y,l_pB.y) || l_normalPoint_v3.y > std::max(l_pA.y,l_pB.y))
            //|| l_normalPoint_v3.z < std::min(l_pA.z,l_pB.z) || l_normalPoint_v3.z > std::max(l_pA.z,l_pB.z))
            {
                l_normalPoint_v3 = l_pB;
                l_pA             = m_path_ptr->m_waypoints_v3[(i + 1) % m_path_ptr->m_waypointsCount_n];
                l_pB             = m_path_ptr->m_waypoints_v3[(i + 2) % m_path_ptr->m_waypointsCount_n];
                l_dir_v3         = l_pB - l_pA;
            }
 
            /** Check if this NormalPoint is the closest to the AI */
            float l_dist_f = glm::distance(m_futurePosition_v3, l_normalPoint_v3);
 
            /** If yes -> save this distance to keep iterating in order to find the closest
             *            and set this NormalPoint as the final Target
            */
            if(l_dist_f < l_minDistance_f)
            {
                l_minDistance_f = l_dist_f;
                l_dir_v3        = glm::normalize(l_dir_v3) * 25.0f;
                l_targetAux_v3  = l_normalPoint_v3 + l_dir_v3;
            }
        }
    
    /** At this point, we've calculated the Target that we have to Seek */
    FindClosest(l_targetAux_v3);
}

/**
 * @brief 
 * 
 * @param l_point_v3 
 * @return int 
 */
void  AIDecisionComponent::FindClosest(const glm::vec3& l_point_v3)
{
    float l_minDistance_f = MaxFloat;
    float l_distance_f = 0;
    size_t l_target_n = m_steerModule_ptr->GetCurrentTarget();

    for(size_t i = l_target_n; i < m_path_ptr->m_waypointsCount_n; i++)
    {
        l_distance_f = glm::distance(l_point_v3, m_path_ptr->m_waypoints_v3[i]);

        if(l_distance_f < l_minDistance_f)
        {
            l_minDistance_f = l_distance_f;
            l_target_n = i;
        }
    }
    //std::cout << "\nTarget: Waypoint " << l_target_n << "\n";

    m_steerModule_ptr->SetCurrentTarget(l_target_n);
}

