#pragma once
#include <queue>
#include <vector>
#include <Event.h>

class EventManager{
    public:
        EventManager(){};
        ~EventManager();
        
        struct Listener{
            void (*l)(Event);
        };
        
        void Init();
        void update();
        void ClearList();
        void addEvent(const Event&);
        void addListener(const Listener& l, const EventType& type);
        void processEvent(const Event& e);

        std::queue<Event> GetEventsQueue()
        {
            return events;
        };

        std::queue<Event> events;
        std::vector<std::vector<Listener>> listeners;
};