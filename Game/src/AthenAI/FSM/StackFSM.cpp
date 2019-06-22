 /* @file StackFSM.cpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <StackFSM.hpp>

/**
 * @brief Construct a new Stack F S M:: Stack F S M object
 * 
 */
StackFSM::StackFSM()
{
    //m_states = new std::vector<StackFSM::state>;
}

/**
 * @brief Destroy the Stack F S M:: Stack F S M object
 * 
 */
StackFSM::~StackFSM(){

}

/**
 * @brief 
 * 
 */
void StackFSM::Update(AIDecisionComponent* l_DComponent)
{
    StackFSM::state l_currentState = GetCurrentState();

    if(l_currentState)
    {
        l_currentState(l_DComponent);
    }
}

/**
 * @brief 
 * 
 * @param l_state 
 */
void StackFSM::PushState(StackFSM::state l_state)
{
    if(GetCurrentState() != l_state)
    {
        m_states.push_back(l_state);
    }
}

/**
 * @brief: pops back the last State
 * 
 */
void StackFSM::PopState()
{
    m_states.pop_back();
}

/**
 * @brief: return size of vector
 * 
 */
int StackFSM::GetSize()
{
    return m_states.size();
}

/**
 * @brief 
 * 
 * @return StackFSM::state 
 */
StackFSM::state StackFSM::GetCurrentState()
{
    return m_states.size() > 0 ? m_states[m_states.size() -1] : NULL;
}
