/**
 * @file RightShoulderFuzzySet.hpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: Right Shoulder Membership Function (any value greater than the midpoint, is the maximum value)
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <IFuzzySet.hpp>

class RightShoulderFuzzySet : public IFuzzySet
{
private:
    /** Variables to construct a Right Shoulder Membership Function */
    double m_peakPoint_d;
    double m_leftOffset_d;
    double m_rightOffset_d;

public:

    RightShoulderFuzzySet(double l_peakPoint_d, double l_leftOffset_d, double l_rightOffset_d) : IFuzzySet( ((l_peakPoint_d + l_rightOffset_d) + l_peakPoint_d) / 2 ),
                                                                                                m_peakPoint_d(l_peakPoint_d),
                                                                                                m_leftOffset_d(l_leftOffset_d),
                                                                                                m_rightOffset_d(l_rightOffset_d){};

    virtual ~RightShoulderFuzzySet(){};
    /** This method calculates the Degree of Membership of a given value for this Membership Function */
    virtual double CalculateDOM(double l_value_d) const;

};