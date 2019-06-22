/**
 * @file TLod.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <TEntity.hpp>
#include <vector>

class TLod : public TEntity
{
    public:
        TLod(uint32_t l_levelsCount, glm::vec3& l_viewPosition, glm::vec3& l_position);
        virtual ~TLod();

        void FindLod();
        void SetDistance(float l_distance) { m_distances.emplace_back(l_distance); m_levelsCount = m_distances.size(); };
        void SetDistancesVector(const std::vector<float>& l_distances) { m_distances = l_distances; m_levelsCount = m_distances.size(); };
        
        virtual void BeginDraw() = 0;
        virtual void EndDraw() = 0;
        virtual bool GetActive() = 0;

    protected:
        uint32_t m_levelsCount = 0;
        uint32_t m_lastLOD = 0;
        
    private:
        glm::vec3* m_viewPosition_ptr   = nullptr;
        glm::vec3* m_nodePosition_ptr   = nullptr;
        std::vector<float> m_distances;
};