/**
 * @file FLV.hpp
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
#pragma once
#include <map>
#include <iostream>
#include <DebugGUIManager.hpp>

class FSet;
class IFuzzySet;
class IFuzzyTerm;

class FLV
{
private:
    /** Map to store all sets the Fuzzy Linguistic Variable has.  */
    typedef std::map<std::string, IFuzzySet*> Sets;

private:
    /** Dissable copies and assigment */
    FLV(const FLV&);
    FLV& operator=(const FLV&);

private:

    Sets m_Sets;
    /** Minimum range value of this FLV */
    double m_minRange_d = 0.0;
    /** Maximum range value of this FLV */
    double m_maxRange_d = 0.0;
    /** Updates the FLV max and min range each time a new Set is added */
    void AdjustRangeOfFLV(double l_minRange_d, double l_maxRange_d);

    /** The destructor of an FLV is private to prevent deleting FLVs, because this client
     *  retrieves a reference of the FLV when uses FuzzyModule::CreateFLV()
     */
    ~FLV();
    /** Only FuzzyModule can call the destructor of this class */
    friend class FuzzyModule;

public:

    FLV() :  m_minRange_d(0.0), m_maxRange_d(0.0){};
    /** This methods adds new sets with different Membership Functions to this FLV */
    FSet AddTriangleFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d,double l_maxBound_d);
    FSet AddRightShoulderFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d,double l_maxBound_d);
    FSet AddLeftShoulderFuzzySet(std::string l_setName, double l_peak_d, double l_minBound_d,double l_maxBound_d);
    /** Fuzzify a given value */
    void Fuzzify(double l_value_d);
    /** Returns a Crisp Value using the Centroid Defuzzification Method */
    double DefuzzifyCentroid(int l_samples_n) const;
    /** For Debug purposes */
    void WriteDOMs();
    void WriteOnBoard(int l_entityId_n);
};