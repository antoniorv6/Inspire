/**
 * @file TriangleFuzzySet.cpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <TriangleFuzzySet.hpp>
#include <Utils.hpp>

/**
 * TriangleFuzzySet::CalculateDOM
 * 
 * @brief: This method calculates the Degree of Membership 
 *         of a given value for this Membership Function.
 * 
 * @param l_value_d 
 * @return double 
 */
double TriangleFuzzySet::CalculateDOM(double l_value_d) const
{
    /** Checks if the Left Offset or the Right Offset are zero, to prevent divide by zero. */
    if((IsEqual(m_rightOffset_d, 0.0) && (IsEqual(m_peakPoint_d, l_value_d))) || 
    (IsEqual(m_leftOffset_d, 0.0) && (IsEqual(m_peakPoint_d, l_value_d))) )
    {   
        return 1.0;
    }
    /** Checks if l_value_d is in the left side of the Triangle's center, then find DOM */
    else if((l_value_d <= m_peakPoint_d) && (l_value_d >= (m_peakPoint_d - m_leftOffset_d)) )
    {
        double l_gradient_d = 1.0 / m_leftOffset_d;

        return l_gradient_d * (l_value_d - (m_peakPoint_d - m_leftOffset_d));
    }
    /** Checks if l_value_d is in the left side of the Triangle's center, then find DOM */
    else if( (l_value_d > m_peakPoint_d) && (l_value_d < (m_peakPoint_d + m_rightOffset_d)) )
    {
        double l_gradient_d = 1.0 / -m_rightOffset_d;

        return l_gradient_d * (l_value_d - m_peakPoint_d) + 1.0;
    }
    /** l_value_d is out of the range of the FLV */
    else
    {
        return 0.0;
    }
}