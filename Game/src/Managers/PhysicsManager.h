#pragma once

#include <PICASSO.hpp>
#include <RenderManager.h>
#include <Octree.h>
#include <iostream>
#include <EntityManager.h>
#include <EventManager.h>
#include <RenderManager.h>
#include <PositionManager.hpp>
#include <CameraManager.hpp>
#include <TransformComponent.h>
#include <RigidBodyComponent.h>
#include <Event.h>
#include <queue>


 
class ClientPredictor;
class PositionManager;
class LapManager;
class PowerUpManager;
class RenderUtilities;
class SoundManager;
class AIManager;

class PhysicsManager{

public:
    PhysicsManager();
    ~PhysicsManager();

    void Init(GenericData *& c_genericData);
    void Update(float delta, const float loopTime);
    void InterpolatePositions(float delta, const float loopTime);
    bool UpdateCollision(PICASSO::PhysicsVariables* , RigidBodyComponent* , PositionComponent*, glm::vec3, Entity*, float delta);
    void ClearList();
    void UpdateVerticalPosition(PICASSO::PhysicsVariables*, Entity*);
    void CollisionWithNextCheckpoint(Entity*, RigidBodyComponent*, PositionComponent*, PICASSO::PhysicsVariables*);

    void CreateRigidBodySphere(Entity* , float, float);
    void CreatePowerUpSphere(Entity* , float, float);
    void CreateRigidBodyRectangle(Entity*, float, float, float, float, float, float, float, bool, bool, bool);

    void CreateOctree();
    void DrawOctree(Octree*);
    Octree* GetOctree();

    void CreateComponent(Entity*);
    void CreateDronesRBList();

    void UpdateKeys(float, float, float, float, float, float, float, int);

    void SetOnlineMatch(bool isOnlineMatch);
    void MakeMovementPrediction(ClientPredictor& c_predictor, double c_now, int c_playerId);

    void CalculatePowerUpCollision(Entity*, Entity*);
    float DistanceToNextCheckpoint(RigidBodyComponent*, Entity* );
    int CalculateNearRivals(int l_entity_id, float radius);
    void SetElectricShock(PICASSO::PhysicsVariables*, int l_entityID_n);
    void SetFreezing(PICASSO::PhysicsVariables*, int l_entityID_n);

    void ResetNitro(PICASSO::PhysicsVariables* data);
    
    float UpdatePerspective(PICASSO::PhysicsVariables*, Entity*);
    void UpdateShader(PICASSO::PhysicsVariables*, Entity*, float);

    bool GetDebugBB(){ return debugBB; };
    bool GetOnlineMatch(){ return onlineMatch; };
    std::map<int, glm::vec3> GetNitroZonesPositions();
    std::vector<int> GetNitroZonesId();

    void PauseClocks();
    void PlayClocks();

    std::vector <TransformComponent*> GetPhysicsComponentList(){ return m_TransComp_list; };
    std::vector <RigidBodyComponent*> GetRBComponentList(){ return m_RBComp_list; };

private:

    std::vector <TransformComponent*> m_TransComp_list;
    std::vector <RigidBodyComponent*> m_RBComp_list;
    std::vector <RigidBodyComponent*> m_RBCompDrones_list;
    std::vector <RigidBodyComponent*> m_RBNitroZones_v3;

    std::vector <RigidBodyComponent*> RB_CloseComponents;

    Octree* octree = nullptr ;
    std::queue<Octree*> nodes;

    bool debugBB = false;
    bool onlineMatch = false;
    bool recharging = false;
    int InOut = 0;

    EntityManager     * m_entityManager_ptr     = nullptr;
    RenderManager     * m_renderManager_ptr     = nullptr;
    PowerUpManager    * m_powerUpManager_ptr    = nullptr;
    GenericData       * m_gameData_ptr          = nullptr;
    CheckpointManager * m_checkpointManager_ptr = nullptr; 
    EventManager      * m_eventManager_ptr      = nullptr; 
    PositionManager   * m_positionManager_ptr   = nullptr;
    LapManager        * m_lapManager_ptr        = nullptr;
    RenderUtilities   * m_renderUtilities_ptr   = nullptr;
    SoundManager      * m_soundManager_ptr      = nullptr;
    AIManager         * m_aiManager_ptr         = nullptr;
};