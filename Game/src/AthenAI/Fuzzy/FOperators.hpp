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
#pragma once
#include <IFuzzyTerm.hpp>
#include <Utils.hpp>
#include <cassert>
#include <vector>

class FAND : public IFuzzyTerm
{
private:
    /** This class may AND together up to 4 terms */
    std::vector<IFuzzyTerm*> m_operands;
    /** Disable assigment */
    FAND& operator= (const FAND&);

public:

    ~FAND();
    /** Copy constructor */
    FAND(const FAND& a);

    FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2);
    FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3);
    FAND(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3, IFuzzyTerm& l_op4);

    IFuzzyTerm* Clone() const { return new FAND(*this); };
    virtual double GetDOM() const;
    virtual void   ClearDOM();
    virtual void   ORwithDOM(double l_value_d);
};

class FOR : public IFuzzyTerm
{
private:
    /** This class may AND together up to 4 terms */
    std::vector<IFuzzyTerm*> m_operands;
    /** Disable assigment */
    FOR& operator= (const FOR&);

public:

    ~FOR(){};
    /** Copy constructor */
    FOR(const FOR& a);

    FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2);
    FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3);
    FOR(IFuzzyTerm& l_op1, IFuzzyTerm& l_op2, IFuzzyTerm& l_op3, IFuzzyTerm& l_op4);

    IFuzzyTerm* Clone() const { return new FOR(*this); };
    virtual double GetDOM() const;
    virtual void   ClearDOM();
    virtual void   ORwithDOM(double l_value_d);
};