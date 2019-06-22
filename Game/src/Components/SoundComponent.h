#pragma once

#include <IComponent.h>
#include <string>
#include <glm.hpp>
#include <memory>

class SoundComponent : public IComponent
{
    public:
        SoundComponent(const std::string& id, const std::string& eID, const std::string& eFMOD, int ent_id) : IComponent(id, ent_id){
            eventID = eID;
            isPlaying = false;
            //float volume = 0.0f;
            //float gain = 0.0f;
        }
        std::string getID() const 
        {
            return eventID;
        }
        std::string getFMODEvent() const
        {
            return FMODEvent;
        }

    private:
        std::string eventID   = "";
        std::string FMODEvent = "";
        //float volume;
        bool isPlaying = false;
        //float gain;
        glm::vec3 pos = glm::vec3(0.0f);
};