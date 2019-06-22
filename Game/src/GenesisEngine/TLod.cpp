/**
 * @file TLod.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <TLod.hpp>


TLod::TLod(uint32_t l_levelsCount, glm::vec3& l_viewPosition, glm::vec3& l_position)
{
    m_levelsCount = l_levelsCount;
    m_distances.reserve(m_levelsCount);
    m_viewPosition_ptr = &l_viewPosition;
    m_nodePosition_ptr = &l_position;
}

/**
 * @brief Destroy the TLod::TLod object
 * 
 */
TLod::~TLod()
{

}

/**
 * @brief 
 * 
 */
void TLod::FindLod()
{
    float d = glm::distance(*m_viewPosition_ptr, *m_nodePosition_ptr);
    m_lastLOD  = 0;

    for(uint32_t i = m_levelsCount - 1; i > 0; i--)
    {
        if( d > m_distances[i] )
        {
            m_lastLOD = i;
            break;
        }
    }
}
