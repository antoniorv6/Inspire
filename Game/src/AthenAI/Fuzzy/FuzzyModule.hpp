/**
 * @file FuzzyModule.hpp
 * @author Edward Thomas         (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * 
 * @brief 
 * @version 0.1
 * @date 2018-12-27
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <cassert>
#include <FuzzyRule.hpp>
#include <IFuzzyTerm.hpp>
#include <FSet.hpp>
#include <IFuzzySet.hpp>
#include <FOperators.hpp>
#include <FHedges.hpp>
#include <FLV.hpp>
#include <DebugGUIManager.hpp>

class FuzzyModule
{
private: 

    typedef std::map<std::string, FLV*> FLVMap;

private: 
    /** Map with all the Fuzzy Linguistic Variables this module uses */
    FLVMap m_flvMap;
    /** Vector with all the Fuzzy Rules this module uses */
    std::vector<FuzzyRule*> m_rules;

    /** Put Zeros to all the consequences */
    inline void SetConfidencesToZero()
    {
        for(auto& rule : m_rules)
        {
            rule->SetConfidenceToZero();
        }
    }
    
    int m_centroidSamples = 15;
    EntityManager* m_entityManager_ptr;

public: 
    ~FuzzyModule();
    /** Creates a new EMPTY Fuzzy Linguistic Variable and returns a reference to it */
    FLV& CreateFLV(const std::string& l_flvName);
    /** Adds a new Fuzzy Rule */
    void AddRule(IFuzzyTerm& l_ant, IFuzzyTerm& l_cons);

    /** Method calls the Fuzzify method of the name FLV */
    inline void Fuzzify(const std::string& l_flvName, double l_value_d)
    {
        //std::cout << "[FUZZIFYING]: (" << l_flvName << ")\n";
        // std::cout << "\n";
        assert( (m_flvMap.find(l_flvName) != m_flvMap.end()) && "<FuzzyModule::Fuzzify> FLV not found");

        m_flvMap[l_flvName]->Fuzzify(l_value_d);
    };

    /** Method that returns a crisp value, given a FLV, using the Centroid Defuzzification Method */
    inline double Defuzzify(const std::string& l_flvName)
    {
        assert( (m_flvMap.find(l_flvName) != m_flvMap.end()) && "<FuzzyModule::Defuzzify> FLV not found");

        SetConfidencesToZero();

        for(auto& rule : m_rules)
        {
            rule->Calculate();
        }

        return m_flvMap[l_flvName]->DefuzzifyCentroid(m_centroidSamples);
    };
    /** DEBUG */
    void writeDOMs();
    void writeInBoard(int l_entityId_n);
};