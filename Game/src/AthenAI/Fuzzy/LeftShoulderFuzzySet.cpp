/**
 * @file LeftShoulderFuzzySet.cpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: Left Shoulder Membership Function (any value greater than the midpoint, is the maximum value)
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <LeftShoulderFuzzySet.hpp>
#include <Utils.hpp>

double LeftShoulderFuzzySet::CalculateDOM(double l_value_d) const
{
    /** Checks if the Left Offset or the Right Offset are zero, to prevent divide by zero. */
    if( (IsEqual(m_rightOffset_d, 0.0) && (IsEqual(m_peakPoint_d, l_value_d))) || 
        (IsEqual(m_leftOffset_d, 0.0) && (IsEqual(m_peakPoint_d, l_value_d))) )
    {   
        return 1.0;
    }
    /** Checks if l_value_d is in the right side of the peak point, then find DOM */
    else if( (l_value_d >= m_peakPoint_d) && (l_value_d < (m_peakPoint_d + m_rightOffset_d)) )
    {
        double l_gradient_d = 1 / -m_rightOffset_d;

        return l_gradient_d * (l_value_d - m_peakPoint_d) + 1.0;
    }
    /** Checks if l_value_d is in the left side of the peak point, then DOM = 1.0*/
    else if( (l_value_d < m_peakPoint_d) && (l_value_d >= (m_peakPoint_d - m_leftOffset_d)) )
    {
        return 1.0;
    }
    /** l_value_d is out of the range of the FLV */
    else
    {
        return 0.0;
    }
}