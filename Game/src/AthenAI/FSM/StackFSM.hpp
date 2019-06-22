 /* @file StackFSM.hpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <vector>
 
class AIDecisionComponent;

class StackFSM
{
public:
    typedef void (*state)(AIDecisionComponent*);

public:
    StackFSM();
    ~StackFSM();

    void Update(AIDecisionComponent* l_DComponent);
    void PushState(state);
    void PopState();
    int  GetSize();

    state GetCurrentState();

private:
    std::vector<state> m_states;
};