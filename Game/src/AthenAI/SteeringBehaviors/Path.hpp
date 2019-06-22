/**
 * @file SteerLib.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-02-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <glm.hpp>
#include <vector>

typedef glm::vec3 Vec3;

class SPath{

friend class SteerModule;
friend class AIDecisionComponent;

public:
    SPath(int l_waypoinstCount_n,
         float l_pathRadius_f,
         bool l_isCyclic_b,
         const std::vector<Vec3>& l_waypoints_v3);

    ~SPath();

    void Initialize(int l_waypoinstCount_n,
                    float l_pathRadius_f,
                    bool l_isCyclic_b,
                    const std::vector<Vec3>& l_waypoints_v3);

    Vec3 GetTarget(int l_index_n) const { return m_waypoints_v3[l_index_n]; };
    size_t GetPathSize()          const { return m_waypointsCount_n; }
    float  GetWaypointRadius()    const { return m_waypointsRadius_f; };

private:
    size_t m_waypointsCount_n = 0;
    float m_waypointsRadius_f = 0.0f;
    float m_pathRadius_f      = 0.0f;
    float m_totalLength_f     = 0.0f;
    bool  m_isCyclic_b        = false;

    std::vector<float> m_lenghts_vf;
    std::vector<Vec3>  m_waypoints_v3;
    std::vector<Vec3>  m_normals_v3;
};