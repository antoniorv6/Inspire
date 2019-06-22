/**
 * @file IDecTreeNode.cpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <DecTree.hpp>
#include <memory>

IDecTreeNode* TreeDecision::MakeDecision()
{
    if(GetBranch())
    {
        if(m_trueBranch_ptr == NULL)
        {
            return NULL;
        }
        else
        {
            return m_trueBranch_ptr->MakeDecision();
        }
    }
    else
    {
        if(m_falseBranch_ptr == NULL)
        {
            return NULL;
        }
        else
        {
            return m_falseBranch_ptr->MakeDecision();
        }
    }
}