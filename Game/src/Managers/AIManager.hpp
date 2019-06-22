#include <glm.hpp>
#include <AIDecisionComponent.hpp>
#include <Entity.h>
#include <vector>
#include <queue>

class WaypointManager;
class PowerUpManager;
class MapManager;
class GenericData;
class AIManager;

typedef void (*Behavior)(AIDecisionComponent*, AIManager*);

//AI Behavior Events
struct AIBehaviorEvent
{
    AIDecisionComponent*    component;
    Behavior                function;
};

class AIManager{
    friend void LODBehavior(AIDecisionComponent* l_component, AIManager* manager);
    friend void DecisionBehavior(AIDecisionComponent* l_component, AIManager* manager);

public:
    AIManager();
    ~AIManager();
    void Init(GenericData *& c_genericData);
    void InitData();
    void Update();
    void Scheduling(const float delta);
    Behavior DeterminateBehavior(int l_entityID_n);
    void ClearList();
    void CreateDecisionComponent(Entity* entity, float l_force, int personality);
    void UpdateAverageTime(double* time, int* num);
    void SetScheduling(bool value){ m_scheduling_b = value; };
    bool GetScheduling(){ return m_scheduling_b; };
    void ClearAIQueue(){  std::queue<AIBehaviorEvent> empty; m_AIBehavior_queue.swap(empty); };
    void WriteValues(float, double, double);
    void WriteValues();
    void RestartAIClock(int id);

    void SetPlayerCheckpoint(int* l_checkpoint_n){ m_player_checkpoint_ptr = l_checkpoint_n; };
    void SetPlayerLap(int* l_lap_n){ m_player_lap_ptr = l_lap_n; };
    void SetNumCheckpoint(int l_num_checkpoint_n){ m_num_checkpoint_n = l_num_checkpoint_n; };

private:
    std::vector<AIDecisionComponent*> m_DecisionComponentsVector_vo;
    EntityManager*  m_entityManager_ptr    = nullptr;
    PhysicsManager* m_physicsManager_ptr   = nullptr;
    WaypointManager* m_waypointManager_ptr = nullptr;
    MapManager*      m_mapManager_ptr      = nullptr;
    PowerUpManager*  m_powerUpManager_ptr  = nullptr;
    GenericData*     m_genericData         = nullptr;

    //===== SCHEDULING ========
    std::queue<AIBehaviorEvent> m_AIBehavior_queue;   //Queue of all Behaviors
    bool m_scheduling_b = true;                       //Scheduling ON/OFF
    double m_expectedTime_d = 0;                      //Time precalculated for all the behaviors

    Clock* m_scheduling_clk = nullptr;
    Clock* m_behavior_clk = nullptr;

    double m_timeDecision_d = 0;
    double m_timeLOD_d = 0;

    int m_numDecision_n = 0;
    int m_numLOD_n = 0;

    int* m_player_checkpoint_ptr = nullptr;
    int* m_player_lap_ptr = nullptr;
    int  m_num_checkpoint_n  = 0;

    int  m_FWDdistanceLOD = 1;
    int  m_BHNDdistanceLOD = 0;

    //Update's loop time
    const float loopTime = 1.0f/30.0f; //update runs 30 times every second
};