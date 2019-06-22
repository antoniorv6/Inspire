/**
 * @file FOperators.hpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: Operators
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <FOperators.hpp>

//==============================================================================
//
//  IMPLEMENTATION OF FAND OPERATOR
//
//==============================================================================

FAND::FAND(const FAND& a)
{
    for(auto& currentTerm : a.m_operands)
    {
        m_operands.push_back(currentTerm->Clone());
    }
}

/**
 * @brief Construct a new FAND::FAND object
 * 
 * @param l_op1 
 * @param l_op2 
 */
FAND::FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
}

/**
 * @brief Construct a new FAND::FAND object
 * 
 * @param l_op1 
 * @param l_op2 
 * @param l_op3 
 */
FAND::FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
    m_operands.push_back(l_op3.Clone());
}

/**
 * @brief Construct a new FAND::FAND object
 * 
 * @param l_op1 
 * @param l_op2 
 * @param l_op3 
 * @param l_op4 
 */
FAND::FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3, IFuzzyTerm& l_op4)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
    m_operands.push_back(l_op3.Clone());
    m_operands.push_back(l_op4.Clone());
}

/**
 * @brief Destroy the FAND::FAND object
 * 
 */
FAND::~FAND()
{
    for(auto& currentTerm : m_operands)
    {
        delete currentTerm;
        currentTerm = nullptr;
    }
}

/**
 * FAND::GetDOM()
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 * @return double 
 */
double FAND::GetDOM() const
{
    double l_min_d = MaxDouble;

    for(auto& currentTerm : m_operands)
    {
        if(currentTerm->GetDOM() < l_min_d)
        {
            l_min_d = currentTerm->GetDOM();
        }
    }
    return l_min_d;
}

/**
 * FAND::ORwithDOM
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 * @param l_value_d 
 */
void FAND::ORwithDOM(double l_value_d)
{
    for(auto& currentTerm : m_operands)
    {
        currentTerm->ORwithDOM(l_value_d);
    }
}

/**
 * FAND::ClearDOM()
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 */
void FAND::ClearDOM()
{
    for(auto& currentTerm : m_operands)
    {
        currentTerm->ClearDOM();
    }
}

//==============================================================================
//
//  IMPLEMENTATION OF FOR OPERATOR
//
//==============================================================================

/**
 * @brief Construct a new FOR::FOR object
 * 
 * @param a 
 */
FOR::FOR(const FOR& a)
{
    for(auto& currentTerm : a.m_operands)
    {
        m_operands.push_back(currentTerm->Clone());
    }
}

/**
 * @brief Construct a new FOR::FOR object
 * 
 * @param l_op1 
 * @param l_op2 
 */
FOR::FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
}

/**
 * @brief Construct a new FOR::FOR object
 * 
 * @param l_op1 
 * @param l_op2 
 * @param l_op3 
 */
FOR::FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
    m_operands.push_back(l_op3.Clone());
}

/**
 * @brief Construct a new FOR::FOR object
 * 
 * @param l_op1 
 * @param l_op2 
 * @param l_op3 
 * @param l_op4 
 */
FOR::FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3, IFuzzyTerm& l_op4)
{
    m_operands.push_back(l_op1.Clone());
    m_operands.push_back(l_op2.Clone());
    m_operands.push_back(l_op3.Clone());
    m_operands.push_back(l_op4.Clone());
}

/**
 * FOR::GetDOM()
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 * @return double 
 */
double FOR::GetDOM() const
{
    double l_max_d = MinFloat;

    for(auto& currentTerm : m_operands)
    {
        if(currentTerm->GetDOM() > l_max_d)
        {
            l_max_d = currentTerm->GetDOM();
        }
    }
    return l_max_d;
}

/**
 * FOR::ORwithDOM
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 * @param l_value_d 
 */
void FOR::ORwithDOM(double l_value_d)
{
    for(auto& currentTerm : m_operands)
    {
        currentTerm->ORwithDOM(l_value_d);
    }
}

/**
 * FOR::ClearDOM
 * 
 * @brief: propagates this call to the composite's childs.
 * 
 */
void FOR::ClearDOM()
{
    for(auto& currentTerm : m_operands)
    {
        currentTerm->ClearDOM();
    }
}




