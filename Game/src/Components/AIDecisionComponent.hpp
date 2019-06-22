
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
#pragma once
#include <vector>
#include <map>
#include <FuzzyModule.hpp>
#include <WaypointComponent.hpp>
#include <TransformComponent.h>
#include <PICASSO.hpp>

class  StackFSM;
class  SPath;
class  SteerModule;
class  ObstacleSensor;
class  RigidBodyComponent;
class  Octree;
class  PhysicsManager;
class  PositionComponent;
class  MapManager;
class  PowerUpManager;
class  DebugGUIManager;
class  PositionManager;
class  GenericData;

class AIDecisionComponent : public IComponent
{
    friend void Seek(AIDecisionComponent*);
    friend void Avoid(AIDecisionComponent*);
    friend void GoToCharge(AIDecisionComponent*);
    friend void LOD(AIDecisionComponent*);
public:
    AIDecisionComponent(const std::string& l_componentId_s, int l_entityId_s, const std::vector<WaypointComponent*>& l_waypointsVector, 
                        float force, int personality, GenericData *&l_genericData_ptr);
    ~AIDecisionComponent();

    void Update();
    void LODUpdate();
    void InitVariables();
    void InitFSM();
    void InitPath(const std::vector<WaypointComponent*>& l_waypointsVector);
    void InitSteerModule(float l_force_f);

    void InitFuzzyModules();
    void InitVelocityModule();
    void InitEnergyStrategyModule();
    void InitChargingNitroModule();
    void InitUseNitroModule();
    void InitUsePowerUpModule();
    void InitTendencyModule();
    void InitCalculateTendencyModule();

    void CalculateLODVelocity();
    void CraigReynoldsFromTarget();
    void FindClosest(const glm::vec3& l_point_v3);
    void InitSensor(glm::vec3* l_position_v3, glm::vec3* l_velocity_v3, float l_rigidRadius_f);
    void MapWaypointsToVec3(const std::vector<WaypointComponent*>& l_waypointsVector);
    void NitroZonesMapping();
    void CalculateSpinRotation(const glm::vec3& l_desiredVelocity_v3, const glm::vec3& l_resultanVel_v3);
    
    void CalculateFuzzyVelocity(glm::vec3& l_velocity, int l_targetIndex_n);
    void CalculateNitro();
    void CalculatePowerUp();

    float CalculateStrategyBalance();
    void  CalculateTendency(float* l_strategy);
    float CalculateChargingNitro(const glm::vec3& l_nitroPosition);
    float CalculateUsingNitro();
    float CalculateUsingPowerUp();
    
    float GetGoToChargeDesirability(const std::map<int, glm::vec3>&  l_nitroZones_v3);
    PICASSO::PhysicsVariables* GetPhysicsData(){ return m_physicsData_ptr; };
    void RestartClock(){ m_notCharge_ptr->Restart(); };

private:
    int     m_entityId_n;
    int     m_selectedNitroWaypoints_n    = 0;
    int     m_personality_n               = 0;
    float*  m_maxVelocity_f               = 0;
    float   m_LODVelocity_f               = 0;
    float   m_mass_f                      = 0;
    float   m_rigidRadius_f               = 0;
    float   m_strategyBalance             = 0.0f;
    float   m_accelerationBalance         = 0.0f;
    float*  m_nitroCharge_f               = nullptr;
    float*  m_raceProgress_f              = nullptr;
    int*    m_racePosition_n              = nullptr;
    int*    m_racePositionPlayer_n        = nullptr;
    float   m_attackness_f                = 0.0f;
    float*  m_distanceNextRival_f         = nullptr;
    int*    m_nearbyRivals_n              = nullptr;
    float   m_tendency_f                  = 0.0f;
    int     m_indexNW_n                   = 0;
    const float*  m_maxVelocityStandar_f  = 0;


    glm::vec3 * m_currentPosition_v3;  
    glm::vec3 * m_currentVelocity_v3;  
    glm::vec3 * m_currentAcceleration_v3;
    glm::vec3   m_futurePosition_v3;

    StackFSM                  * m_sfsm_ptr             = nullptr;
    SPath                     * m_path_ptr             = nullptr;
    SteerModule               * m_steerModule_ptr      = nullptr;
    ObstacleSensor            * m_obstacleSensor_ptr   = nullptr;
    PICASSO::PhysicsVariables * m_physicsData_ptr      = nullptr;
    PICASSO::PhysicsVariables * playerData             = nullptr;
    RigidBodyComponent        * m_rigidBody_ptr        = nullptr;
    PositionComponent         * m_position_ptr         = nullptr;
    Octree                    * m_octree_ptr           = nullptr;

    FuzzyModule                 m_velocityModule;
    FuzzyModule                 m_energyStrategyModule;
    FuzzyModule                 m_chargingNitroModule;
    FuzzyModule                 m_usingNitroModule;
    FuzzyModule                 m_usingPowerUpModule;
    FuzzyModule                 m_tendencyModule;
    FuzzyModule                 m_calculateTendencyModule;

    EntityManager*              m_entityManager_ptr   = nullptr;
    PhysicsManager*             m_physicsManager_ptr  = nullptr;
    MapManager*                 m_mapManager_ptr      = nullptr;
    PowerUpManager*             m_powerUpManager_ptr  = nullptr;
    DebugGUIManager*            m_debugManager_ptr    = nullptr;
    PositionManager*            m_positionManager_ptr = nullptr;

    Clock*                      m_notCharge_ptr       = nullptr;

    std::vector<glm::vec3>                  m_waypoints_v3;
    std::vector<glm::vec3>                  m_closestObjects_v3;
    std::map<int, glm::vec3>                m_nitroZonesMap_m;
    std::map<int, std::vector<glm::vec3>>   m_nitroWaypointsMap_m;
};