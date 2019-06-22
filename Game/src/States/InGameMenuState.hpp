#pragma once
#include <State.hpp>

class RenderManager;
class SoundManager;
class EntityManager;
class EventManager;
class StatesMachine;
class Event;
class GUI;
class GenericData;
class InGameMenuState : public State
{

    public:
        InGameMenuState(GenericData* c_gameData, EntityManager *l_entityManager,RenderManager *l_renderManager, SoundManager *l_soundManager, EventManager *l_eventManager);
        ~InGameMenuState();
        void Init();
        void Update(float&);
        void Draw();
        void ChangeState();
        virtual void ProcessGUIEvent(int, int);


    private:
        RenderManager* renderManager    = nullptr;
        SoundManager*  soundManager     = nullptr;
        EntityManager* entityManager    = nullptr;
        EventManager*  eventManager     = nullptr;
        StatesMachine* statesMachine    = nullptr;
        GUI* gui                        = nullptr;
        bool   exit = false;
};