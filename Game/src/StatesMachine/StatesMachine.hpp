#pragma once

#include <memory>
#include <stack>
#include <Clock.hpp>
#include <State.hpp>

class StatesMachine {
    public:
        static StatesMachine* getInstance()
            {
                static StatesMachine instance;
                return &instance;
            }
        ~StatesMachine();

        void AddState(std::unique_ptr<State> newState, bool isReplacing = true);
        void RemoveState(bool value);
        // Run at start of each loop in Game.cpp
        void ProcessStateChanges(Clock*, float&);

        std::unique_ptr<State> &GetActiveState();

        bool isActualStateOut(){return this->m_isRemoving;};

    private:
        std::stack<std::unique_ptr<State>> m_states;
        std::unique_ptr<State> m_newState;

        bool m_isRemoving = false, 
             m_isAdding = false,
             m_isReplacing = false,
             m_EndingGame = false;
};