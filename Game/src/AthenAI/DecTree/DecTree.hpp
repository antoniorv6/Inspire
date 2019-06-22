/**
 * @file DecTree.hpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-01-18
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once

class IDecTreeNode{
    
public:
    virtual IDecTreeNode* MakeDecision() = 0;

};

class TreeAction : public IDecTreeNode
{

public:
    virtual IDecTreeNode* MakeDecision()
    {
        return this;
    }
};

class TreeDecision : public IDecTreeNode
{

public:

    IDecTreeNode* m_trueBranch_ptr;
    IDecTreeNode* m_falseBranch_ptr;

    virtual bool GetBranch() = 0;
    virtual IDecTreeNode* MakeDecision();

};