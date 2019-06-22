/**
 * @file FuzzyModule.cpp
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
#include <FuzzyModule.hpp>
#include <iostream>

/**
 * @brief Destroy the Fuzzy Module:: Fuzzy Module object
 * 
 */
FuzzyModule::~FuzzyModule()
{
    for(auto& flv : m_flvMap)
    {
        delete flv.second;
        flv.second = nullptr;
    }

    for(auto& rule : m_rules)
    {
        delete rule;
        rule = nullptr;
    }
}

/**
 * FuzzyModule::AddRule
 * 
 * @brief 
 * 
 * @param l_ant 
 * @param l_cons 
 */
void FuzzyModule::AddRule(IFuzzyTerm& l_ant, IFuzzyTerm& l_cons)
{
    m_rules.push_back(new FuzzyRule(l_ant, l_cons));
}

/**
 * FuzzyModule::CreateFLV
 * 
 * @brief 
 * 
 * @param l_flvName 
 * @return FLV& 
 */
FLV& FuzzyModule::CreateFLV(const std::string& l_flvName)
{
    m_flvMap[l_flvName] = new FLV();

    return *m_flvMap[l_flvName];
}

void FuzzyModule::writeDOMs()
{
    for(auto& flv : m_flvMap)
    {
        std::cout << flv.first << std::endl;
        flv.second->WriteDOMs();
        std::cout << "\n";
    }
}

//Tenemos que pasarle la referencia a la entidad
//void FuzzyModule::writeInBoard(int l_entityId_n)
//{
//   for(auto& flv : m_flvMap)
//   {
//       
//        DebugGUIManager::getInstance()->AddTextToInterface(m_entityManager_ptr->GetEntity
//        (l_entityId_n), "FLV: " + flv.first);
//
//        flv.second->WriteOnBoard(l_entityId_n);
//
//        DebugGUIManager::getInstance()->AddTextToInterface(m_entityManager_ptr->GetEntity
//        (l_entityId_n), "\n");
//   }
//}