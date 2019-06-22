/**
 * @file IFuzzyTerm.hpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: abstract class to provide an interface for classes able to be
 *         used as terms in a fuzzy if-then rule base.
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once

class IFuzzyTerm
{
public:

    virtual ~IFuzzyTerm(){};

    /** All the terms have to implement the same constructor. */
    virtual IFuzzyTerm* Clone() const = 0;

    /** Returns the Degree of Membership of the term. */
    virtual double GetDOM() const = 0;

    /** Resets the DOM to zero. */
    virtual void ClearDOM() = 0;

    /** Method to update the DOM of a consequent when a rule is fired. */
    virtual void ORwithDOM(double l_value_d) = 0;

};
