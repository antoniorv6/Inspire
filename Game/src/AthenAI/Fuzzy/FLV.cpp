/**
 * @file FLV.cpp
 * @author Edward Owen            (contact@edwardowen.es)
 *         Iván Palacios Verano   (ivanpverano@gmail.com)
 * 
 * @brief: Declaration of the Fuzzy Linguistic Variables
 * 
 * @version 0.1
 * @date 2018-12-19
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include <FLV.hpp>
#include <FOperators.hpp>
#include <TriangleFuzzySet.hpp>
#include <RightShoulderFuzzySet.hpp>
#include <LeftShoulderFuzzySet.hpp>
#include <FSet.hpp>
#include <Utils.hpp>
#include <cassert>

/**
 * @brief Destroy the FLV::FLV object
 * 
 */
FLV::~FLV()
{
    Sets::iterator it;
    for (it = m_Sets.begin(); it != m_Sets.end(); ++it)
    {
        delete it->second;
        it->second = nullptr;
    }
}

/**
 * FLV::Fuzzify
 * 
 * @brief: Calculates and Set the DOM for all the Sets.
 * 
 * @param l_value_d 
 */
void FLV::Fuzzify(double l_value_d)
{
    if(std::isnan(l_value_d))
    {
       l_value_d = 0;
    }
    //std::cout <<  "El minimo es: " + std::to_string(m_minRange_d) + " y el maximo es: " + std::to_string(m_maxRange_d) + " y el valor es: " + std::to_string(l_value_d) + "\n";
    //std::cout << "\n";
    assert( (l_value_d >= m_minRange_d) && (l_value_d <= m_maxRange_d) && "<FLV::Fuzzify> invalid value. ");

    Sets::iterator it;
    for (it = m_Sets.begin(); it != m_Sets.end(); ++it)
    {
        it->second->SetDOM(it->second->CalculateDOM(l_value_d));
    }

}

/**
 * FLV::DefuzzifyCentroid
 * 
 * @brief: Returns a Crisp Value using the Centroid Defuzzification Method.
 * 
 * @param l_samples_n 
 * @return double 
 */
double FLV::DefuzzifyCentroid(int l_samples_n) const
{
    //calculate the step size
    double l_stepSize_d     = (m_maxRange_d - m_minRange_d)/(double)l_samples_n;
    double l_totalArea_d    = 0.0;
    double l_sumOfMoments_d = 0.0;

    for (int i = 1; i <= l_samples_n; ++i)
    {
        double l_DOM2Calculate_d =  m_minRange_d + i * l_stepSize_d;
        //for each set get the contribution to the area. This is the lower of the 
        //value returned from CalculateDOM or the actual DOM of the fuzzified 
        //value itself   
        for (auto& set : m_Sets)
        {
            double contribution = 
                MinOf(set.second->CalculateDOM(l_DOM2Calculate_d),
                        set.second->GetDOM());

            l_totalArea_d += contribution;

            l_sumOfMoments_d += (l_DOM2Calculate_d)  * contribution;
        }
    }

    //make sure total area is not equal to zero
    if (IsEqual(0, l_totalArea_d))
    {
        return 0.0;
    } 

    return (l_sumOfMoments_d / l_totalArea_d);
}

/**
 * FLV::AddTriangleFuzzySet
 * 
 * @brief 
 * 
 * @param l_setName 
 * @param l_peak_d 
 * @param l_minBound_d 
 * @param l_maxBound_d 
 * @return FSet 
 */
FSet FLV::AddTriangleFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d,double l_maxBound_d)
{  
    m_Sets[l_setName] = new TriangleFuzzySet(l_peak_d, l_peak_d - l_minBound_d, l_maxBound_d - l_peak_d);

    AdjustRangeOfFLV(l_minBound_d, l_maxBound_d);

    return FSet(*m_Sets[l_setName]);
}

/**
 * FLV::AddRightShoulderFuzzySet
 * 
 * @brief 
 * 
 * @param l_setName 
 * @param l_peak_d 
 * @param l_minBound_d 
 * @param l_maxBound_d 
 * @return FSet 
 */
FSet FLV::AddRightShoulderFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d, double l_maxBound_d)
{
    m_Sets[l_setName] = new RightShoulderFuzzySet(l_peak_d, l_peak_d - l_minBound_d, l_maxBound_d + l_peak_d);

    AdjustRangeOfFLV(l_minBound_d, l_maxBound_d);

    return FSet(*m_Sets[l_setName]);
}

/**
 * FLV::AddLeftShoulderFuzzySet
 * 
 * @brief 
 * 
 * @param l_setName 
 * @param l_peak_d 
 * @param l_minBound_d 
 * @param l_maxBound_d 
 * @return FSet 
 */
FSet FLV::AddLeftShoulderFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d,double l_maxBound_d)
{
    m_Sets[l_setName] = new LeftShoulderFuzzySet(l_peak_d, l_peak_d - l_minBound_d, l_maxBound_d - l_peak_d);

    AdjustRangeOfFLV(l_minBound_d, l_maxBound_d);

    return FSet(*m_Sets[l_setName]);
}

/**
 * FLV::AdjustRangeOfFLV
 * 
 * @brief 
 * 
 * @param l_minRange_d 
 * @param l_maxRange_d 
 */
void FLV::AdjustRangeOfFLV(double l_minRange_d, double l_maxRange_d)
{
    if(l_minRange_d < m_minRange_d) m_minRange_d = l_minRange_d;
    if(l_maxRange_d > m_maxRange_d) m_maxRange_d = l_maxRange_d;
}


void FLV::WriteDOMs()
{
    for(auto& flv : m_Sets)
    {
        std::cout << "==================================\n";
        std::cout << "SET: " << flv.first << " // DOM: " <<  flv.second->GetDOM() << std::endl;
        std::cout << "==================================\n";
    }
}

//PASAR REFERENCIA A LA ENTIDAD
//void FLV::WriteOnBoard(int l_entityId_n)
//{
//    for(auto& set : m_Sets)
//    {
//        DebugGUIManager::getInstance()->AddTextToInterface(EntityManager::getInstance()->GetEntity
//        (l_entityId_n), "   SET -> " + set.first + " DOM: " + std::to_string(set.second->GetDOM()));
//
//    }
//}