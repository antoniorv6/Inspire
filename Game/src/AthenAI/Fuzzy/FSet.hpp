/**
 * @file FSet.hpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: Proxy to represent a FuzzySet and perfom rules operations
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <IFuzzyTerm.hpp>
#include <IFuzzySet.hpp>

 class FAND; 

class FSet : public IFuzzyTerm
{

private: 

    friend class FVERY;  
    friend class FFAIRLY;

private:
    /** Reference to the set this Proxy represents. */
    IFuzzySet& m_fuzzySet;

public:

    FSet(IFuzzySet& l_fuzzySet) : m_fuzzySet(l_fuzzySet){};

    IFuzzyTerm*    Clone() const { return new FSet(*this); };
    virtual double GetDOM() const { return m_fuzzySet.GetDOM(); };
    virtual void   ClearDOM() { m_fuzzySet.ClearDOM(); };
    virtual void   ORwithDOM(double l_value_d) { m_fuzzySet.ORWithDOM(l_value_d); };
};