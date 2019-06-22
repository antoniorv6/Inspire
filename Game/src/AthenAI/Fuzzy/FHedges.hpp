/**
 * @file FHedges.hpp
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
#pragma once
#include <IFuzzyTerm.hpp>
#include <IFuzzySet.hpp>
#include <FSet.hpp>
#include <math.h>

class FVERY : public IFuzzyTerm
{
private: 

    IFuzzySet& m_fuzzySet;

    /** To prevent copying and assigment */
    FVERY(const FVERY& instance) : m_fuzzySet( instance.m_fuzzySet ){};
    FVERY& operator=(const FVERY&);

public: 

    FVERY(FSet& l_term) : m_fuzzySet(l_term.m_fuzzySet){};

    /** Returns the square of the term because VERY = (F(x))² */
    virtual double GetDOM() const { return m_fuzzySet.GetDOM() * m_fuzzySet.GetDOM(); };
    /** Virtual constructor. */
    virtual IFuzzyTerm* Clone() const { return new FVERY(*this); };
    /** Propagate the Clear of the DOM */
    virtual void ClearDOM() { m_fuzzySet.ClearDOM(); }; 
    /** Propagate the ORwithDOM with the square of the value, because VERY = (F(x))² */
    virtual void ORwithDOM(double l_value_d) { m_fuzzySet.ORWithDOM(l_value_d * l_value_d); };
};

class FFAIRLY : public IFuzzyTerm
{
private: 

    IFuzzySet& m_fuzzySet;

    /** To prevent copying and assigment */
    FFAIRLY(const FFAIRLY& instance) : m_fuzzySet( instance.m_fuzzySet ){};
    FFAIRLY& operator=(const FFAIRLY&);

public: 

    FFAIRLY(FSet& l_term) : m_fuzzySet(l_term.m_fuzzySet){};

    /** Returns the square of the term because VERY = (F(x))² */
    virtual double GetDOM() const { return sqrt(m_fuzzySet.GetDOM()); };
    /** Virtual constructor. */
    virtual IFuzzyTerm* Clone() const { return new FFAIRLY(*this); };
    /** Propagate the Clear of the DOM */
    virtual void ClearDOM() { m_fuzzySet.ClearDOM(); }; 
    /** Propagate the ORwithDOM with the square of the value, because VERY = (F(x))² */
    virtual void ORwithDOM(double l_value_d) { m_fuzzySet.ORWithDOM( sqrt(l_value_d) ); };
};
