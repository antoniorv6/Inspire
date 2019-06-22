#pragma once

#include <string>
#include <iostream>
#include <IComponent.h>
#include <EntityManager.h>
#include <TransformComponent.h>
#include <PICASSO.hpp>
#include <GenericData.hpp>

class PositionManager;


class PositionComponent : public IComponent{
    friend class PositionManager;
    friend bool SortPositions(PositionComponent* l_pos, PositionComponent* r_pos, PositionManager *& c_manager);
    
    public:
        PositionComponent(const std::string& id, int l_entityId_s, GenericData *&l_genericData) : IComponent(id, l_entityId_s)
        {
            IComponent * component = l_genericData->g_entityManager_ptr->GetComponent(l_entityId_s, "Physics"); 
            TransformComponent* tr = ((TransformComponent*) component);
            m_physicsData_ptr = tr->GetPhysicsData();

            checkpoint = 0;
            next_checkpoint = 0;
            position = 1;
            distance = 0.0f;
            distance_next_rival = 0;
            race_progress = 0;
            nearby_rivals = 0;
            lap = 1;
        }

        void AddCheckpoint(int total_checkpoints){ checkpoint++; CalculateNextCheckpoint(total_checkpoints); };
        int GetPosition(){ return position; };
        int* GetRefPosition(){ return &position; };
        void SetPosition(int pos){ position = pos; };
        int GetCheckpoint(){ return checkpoint;};
        int* GetCheckpointPTR(){ return &checkpoint;};
        int GetNextCheckpoint(){ return next_checkpoint; };
        int* GetNextCheckpointPTR(){ return &next_checkpoint; };
        float GetDistance(){ return distance;};
        void SetDistance(float dis){ distance = dis; };
        void SetRaceProgress(float value){ race_progress = value;};
        float* GetRaceProgress(){ return &race_progress; };
        void SetDistanceNextRival(float value){ distance_next_rival = value;};
        float* GetDistanceNextRival(){ return &distance_next_rival; };
        int* GetNearbyRivals(){ return &nearby_rivals; };
        int GetLap(){ return lap; };
        int* GetLapPTR(){ return &lap; };
        void CalculateNextCheckpoint(int total_checkpoints)
        {
            next_checkpoint++;
            
            if(next_checkpoint >= total_checkpoints)
                next_checkpoint = 0;

            //std::cout << next_checkpoint << " " << num_check << std::endl;
        };
        void setOnline(int l_checkpoint_n, int l_next_checkpoint_n, int l_lap_n)
        { 
            checkpoint = l_checkpoint_n;
            next_checkpoint = l_next_checkpoint_n;
            lap = l_lap_n;
        };

    private:
        
        PICASSO::PhysicsVariables* m_physicsData_ptr = 0;
        float distance_next_rival = 0.0f;
        float race_progress = 0.0f;
        int position = 0;
        int checkpoint = 0;
        int next_checkpoint = 0;
        int lap = 0;
        float distance = 0.0f;
        bool dist_calc = false;
        int nearby_rivals;
        const int NUM_LAPS = 3;

};