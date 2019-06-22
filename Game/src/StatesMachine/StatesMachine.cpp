#include <StatesMachine.hpp>

void StatesMachine::AddState(std::unique_ptr<State> newState, bool isReplacing)
{
    m_isAdding = true;
    m_isReplacing = isReplacing;
    m_newState = std::move(newState);
}

StatesMachine::~StatesMachine()
{}

void StatesMachine::RemoveState(bool value) 
{
    m_isRemoving = true;
    m_EndingGame = value;
}

void StatesMachine::ProcessStateChanges(Clock* clock, float& delta)
{
    if (this->m_isRemoving && !this->m_states.empty()) 
    {
        this->m_states.pop();
        this->m_isRemoving = false;

        if(!this->m_isAdding)
        {
            this->m_states.top()->Resume();
            clock->Restart();
            delta = 0;
        }

        if(this->m_EndingGame)
        {
            this->m_states.top()->ChangeState();
        }

    }

    if (this->m_isAdding) 
    {
        if (!this->m_states.empty()) 
        {
            if (this->m_isReplacing) 
            {
                this->m_states.pop();       
            } 
        }

        this->m_states.push(std::move(this->m_newState));
        this->m_states.top()->Init();
        this->m_isAdding = false;

        if(clock)
        {
            clock->Restart();
            delta = 0;
        }
    }
}

std::unique_ptr<State> &StatesMachine::GetActiveState() 
{
    return m_states.top();
}