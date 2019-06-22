/**
 * @file TriangleFuzzySet.hpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <IFuzzySet.hpp>

class TriangleFuzzySet : public IFuzzySet
{
private:
    /** Variables to construct a Triangle Membership Function */
    double m_peakPoint_d;
    double m_leftOffset_d;
    double m_rightOffset_d;

public:
    TriangleFuzzySet(double l_peakPoint_d, double l_leftOffset_d, double l_rightOffset_d) : IFuzzySet(l_peakPoint_d),
                                                                                            m_peakPoint_d(l_peakPoint_d),
                                                                                            m_leftOffset_d(l_leftOffset_d),
                                                                                            m_rightOffset_d(l_rightOffset_d){};
    virtual ~TriangleFuzzySet(){};
    /** This method calculates the Degree of Membership of a given value for this Membership Function */
    virtual double CalculateDOM(double l_value_d) const;

};