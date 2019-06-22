/**
 * @file FuzzySet.hpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <cassert>
class IFuzzySet
{
protected:

    /** Holds the Degree of Membership in this set, given a specific value */
    double     m_DOM_d;

    /** Holds the maximum of the set's  membership function.
     *  If the membership function is a Triangle, then this will be the Peak Point.
     *  If the membership function has a plateau, then this will be the Mid Point of the plateau.
     *  This value is set in the constructor to avoid run-time calculations of midpoints.
     */
    double     m_representativeValue_d;
public:

    IFuzzySet(double l_representativeValue_d) : m_DOM_d(0.0), m_representativeValue_d(l_representativeValue_d){};
    virtual ~IFuzzySet(){};
    /** Returns the Degree of Membership by a given value. 
     *  This method do not set m_DOM_d to the DOM value pass: m_DOM_d;ed as parameter. This is
     *  because the Centro Defuzzification Method uses this method to determine the DOM 
     *  of the values it uses as its sample points:
     */
    virtual double CalculateDOM(double l_value_d) const = 0;

    /** If the FuzzySet is a part of a consequence in a FLV, the this method
     *  sets the DOM(confidence level) to the maximum of the parametrer or the
     *  m_DOM_d value. IF THE ARE MULTIPLE CONFIDENCES (EX: DESIRABLE = 0.2 and DESIRABLE = 0.5, then choose 0.5) 
     */
    void ORWithDOM(double l_value_d){ if(l_value_d > m_DOM_d) m_DOM_d = l_value_d; };
    /** Getters and Setters */
    double GetRepresentativeValue() const { return m_representativeValue_d; };
    /** Resets DOM to 0.0 */
    void   ClearDOM(){ m_DOM_d = 0.0; };
    /** Returns the DOM value */
    double GetDOM() const{ return m_DOM_d; };
    /** Sets a DOM value, checking first if this value is >= 0 and <= 1 */
    void   SetDOM(double l_value_d) 
    { 
        /** Terminates the program if the parameter is invalid */
        assert ((l_value_d <= 1) && (l_value_d >= 0) && "<FuzzySet::SetDOM> invalid value");
        m_DOM_d = l_value_d;
    };
};