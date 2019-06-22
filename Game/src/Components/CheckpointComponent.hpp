#pragma once

#include "IComponent.h"
#include <string>
#include <iostream>


class CheckpointComponent: public IComponent{
    public:
        CheckpointComponent(const std::string& id, int l_entityId_s, int numerito, float vlimit, bool zone) : IComponent(id, l_entityId_s)
        {
            checkpoint_num = numerito;
            vertical_limit = vlimit;
            change_zone = zone;

            //std::cout << "ID CHECKPOINT: " << checkpoint_num 
            //          << " VERT.LIMIT: " << vertical_limit 
            //          << std::endl;
        }

        int GetId(){ return checkpoint_num; };
        float GetVerticalLimit(){ return vertical_limit; };
        int GetChangeZone(){ return change_zone; };
        
        ~CheckpointComponent(){};
    private:
        int checkpoint_num = 0;
        float vertical_limit = 0.0f;
        bool change_zone = false;
};