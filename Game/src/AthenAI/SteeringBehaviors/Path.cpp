/**
 * @file Path.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Path.hpp>

/**
 * @brief Construct a new SPath::SPath object
 * 
 * @param l_waypoinstCount_n 
 * @param l_pathRadius_f 
 * @param l_isCyclic_b 
 * @param l_waypoints_v3 
 */
SPath::SPath(int l_waypoinstCount_n, float l_pathRadius_f, bool l_isCyclic_b, const std::vector<Vec3>& l_waypoints_v3)
{
    Initialize(l_waypoinstCount_n, 
               l_pathRadius_f, 
               l_isCyclic_b, 
               l_waypoints_v3);
}

/**
 * @brief Destroy the SPath::SPath object
 * 
 */
SPath::~SPath()
{

}

/**
 * @brief 
 * 
 * @param l_waypoinstCount_n 
 * @param l_pathRadius_f 
 * @param l_isCyclic_b 
 * @param l_waypoints_v3 
 */
void SPath::Initialize(int l_waypoinstCount_n, float l_pathRadius_f, bool l_isCyclic_b, const std::vector<Vec3>& l_waypoints_v3)
{
    // m_pathRadius_f     = l_pathRadius_f;
    // m_isCyclic_b       = l_isCyclic_b;
    // m_waypointsCount_n = l_waypoinstCount_n;
    // if (m_isCyclic_b)  m_waypointsCount_n++;

    // m_waypoints_v3.reserve(m_waypointsCount_n);
    // m_normals_v3.reserve(m_waypointsCount_n);
    // m_lenghts_vf.reserve(l_waypoinstCount_n);

    // for(int i = 0; i < m_waypointsCount_n; i++)
    // {
    //     bool l_isCloseCycle_b = m_isCyclic_b && (i == m_waypointsCount_n - 1);
    //     int j = l_isCloseCycle_b ? 0 : i;
    //     m_waypoints_v3[i] = l_waypoints_v3[j];

    //     if(i > 0)
    //     {
    //         m_normals_v3[i]  = m_waypoints_v3[i] - m_waypoints_v3[i - 1];
    //         m_lenghts_vf[i]  = m_normals_v3[i].length();
    //         m_normals_v3[i]  = glm::normalize(m_normals_v3[i]);
    //         m_totalLength_f += m_lenghts_vf[i];
    //     }
    // }
    m_waypoints_v3      = l_waypoints_v3;
    m_waypointsCount_n  = l_waypoints_v3.size();
    m_waypointsRadius_f = l_pathRadius_f; 
}
