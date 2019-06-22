#include "EventManager.h"
#include <iostream>

/**
 * @brief Destroy the Event Manager:: Event Manager object
 * 
 */
EventManager::~EventManager()
{
    listeners.clear();
    std::queue<Event>().swap(events);
}

/**
 * @brief 
 * 
 */
void EventManager::update()
{
    while(!events.empty()){
        Event e = events.front();
        events.pop();
        processEvent(e);
    }
}

/**
 * @brief 
 * 
 * @param e 
 */
void EventManager::addEvent(const Event& e)
{
    events.push(e);
}

/**
 * @brief 
 * 
 * @param l 
 * @param type 
 */
void EventManager::addListener(const Listener& l, const EventType& type)
{
    //std::cout << "TYPE" << type << std::endl;
    //Add vector if doesn't exist in the matrix
    if(listeners.size() <= type)
    {
        for(unsigned int i = listeners.size(); i <= type; i++){
            listeners.push_back(std::vector<Listener>());
        }
    }

    if(listeners[type].size() == 0)
    {
        listeners[type].push_back(l);
    }
}

/**
 * @brief 
 * 
 * @param e 
 */
void EventManager::processEvent(const Event& e)
{
    for(unsigned int i = 0; i < listeners[e.type].size(); i++)
    {
      listeners[e.type][i].l(e);
    }
}

/**
 * @brief 
 * 
 */
void EventManager::ClearList()
{
    //for(unsigned int i = 0; i < listeners.size(); i++)
    //{
    //    listeners[i].clear();
    //}
    // 
    //listeners.clear();

    while(!events.empty())
    { 
        events.pop();
    }
   
}