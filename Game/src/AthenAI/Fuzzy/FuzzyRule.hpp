/**
 * @file FuzzyRule.hpp
 * @author Edward Thomas        (contact@edwardowen.es)
 *         Iván Palacios Verano (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-27
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "FSet.hpp"
#include "FOperators.hpp"

class FuzzyRule
{
private:

    /** Antecedent, usually a composite or some fuzzy sets and operators */
    const IFuzzyTerm* m_antecedent_ptr;

    /** Consequence, usually only one Set */
    IFuzzyTerm* m_consequence_ptr;

    /** We don't want to allow clients make copies */
    FuzzyRule(const FuzzyRule&);
    FuzzyRule& operator=(const FuzzyRule&);

public:

    FuzzyRule(const IFuzzyTerm& l_ant, const IFuzzyTerm& l_cons) : m_antecedent_ptr(l_ant.Clone()), m_consequence_ptr(l_cons.Clone()){}

    ~FuzzyRule(){ delete m_antecedent_ptr; delete m_consequence_ptr; m_antecedent_ptr = nullptr; m_consequence_ptr = nullptr; }

    /** Clears the confidencies levels of the consequence*/
    void SetConfidenceToZero() { m_consequence_ptr ->ClearDOM(); }

    /** Calculates the Degree of Membership, using the Antecedent's DOM */
    void Calculate()
    {
        m_consequence_ptr->ORwithDOM(m_antecedent_ptr->GetDOM());
    }
};