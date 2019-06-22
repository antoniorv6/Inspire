#pragma once

#include <IComponent.h>
#include <Entity.h>
#include <PICASSO.hpp>

#include <string>
#include <glm.hpp>
#include <memory>

#include <GenericData.hpp>

enum EventType
{
    Nitro_Charge,
    Power_Up,
    Single_Player,
    Void_Event,
    RightPowerUP,
    LeftPowerUP,
    StartGame,
    Resume,
    Exit,
    ExitPodium,
    LeftDrone,
    RightDrone,
    ChooseDrone,
    Selector_back,
    Exit_Game
};

struct Event
{
    std::string id_str;
    EventType type;
    Entity* entity_pt;
    GenericData* m_eventData_ptr;
    int GUIAction;
};


