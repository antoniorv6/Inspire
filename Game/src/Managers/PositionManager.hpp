#pragma once

#include <PositionComponent.hpp>
#include <CheckpointManager.hpp>
#include <EntityManager.h>
#include <Entity.h>

#include <vector>
#include <algorithm>
#include <iostream>

class RenderManager;
class PhysicsManager;
class CheckpointManager;
class GenericData;
class SoundManager;
class HUD;
class AIManager;
class RenderManager;

class PositionManager{
    public:
    PositionManager();
    ~PositionManager();

    void Init(GenericData *& c_genericData);
    
    void InitHUD(HUD*& c_HUD_ptr);

    void ClearList();

    void    CreateComponent(Entity* entity);
    void    Update();
    void    BillboardsUpdate();
    void    OnlineUpdate();
    void    AddCheckpoint(int id_entity);
    void    AddLap(int id_entity, PositionComponent* );
    void    Add2FinalResults(std::string name);
    void    DistanceToNextCheckpoint(PositionComponent* );
    Entity* GetDrone(int position);
    int     CalculateNextPosition(PositionComponent* );
    int     GetLastPosition(){ return m_positionComponent_v.size(); };
    void    SetNumCheckpoints(int value);
    int     CalculateRaceProgress(int l_entity_id);
    bool*   IsPlay(){ return &isPlay; };
    void    EndGame();
    PositionComponent* GetPlayerPosition();
    std::vector<std::string>* GetFinalResults(){ return &m_finalResults_v; };


    private:
        std::vector<PositionComponent*> m_positionComponent_v;
        std::vector<std::string> m_finalResults_v;

        std::vector<Entity*> m_entityBillboard_v;
        int m_numBillboards = 0;
        
        int m_numCheckpoints_n = 0;
        int m_playerPosition_i = 0;
        

        EntityManager*      m_entityManager_ptr      = nullptr;
        PhysicsManager*     m_physicsManager_ptr     = nullptr;
        CheckpointManager*  m_checkpointManager_ptr  = nullptr;
        DebugGUIManager*    m_debugGUIManager_ptr    = nullptr;
        SoundManager*       m_soundManager_ptr       = nullptr; 
        AIManager*          m_aiManager_ptr          = nullptr;
        RenderManager*      m_renderManager_ptr      = nullptr;
        GenericData*        m_genericData_ptr        = nullptr;

        Entity*             m_player_ptr = nullptr;
        HUD*                m_HUD_ptr = nullptr;
        bool                isPlay = false;
};