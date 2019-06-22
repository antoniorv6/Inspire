/**
 * @file SensorsLib.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <SensorsLib.hpp>
#include <Octree.h>

/**
 * @brief Construct a new Obstacle Sensor:: Obstacle Sensor object
 * 
 * @param l_position_v3 
 * @param l_velocity_v3 
 */
ObstacleSensor::ObstacleSensor(Vec3* l_position_v3, Vec3* l_velocity_v3, float l_rigidRadius_f)
{
    Initialize(l_position_v3, l_velocity_v3, l_rigidRadius_f);
}

/**
 * @brief Destroy the Obstacle Sensor:: Obstacle Sensor object
 * 
 */
ObstacleSensor::~ObstacleSensor()
{
    delete m_perpPlane_ptr;
    m_perpPlane_ptr = nullptr;
}

/**
 * @brief Construct a new Obstacle Sensor:: Initialize object
 * 
 * @param l_position_v3 
 * @param l_velocity_v3 
 */
void ObstacleSensor::Initialize(Vec3* l_position_v3, Vec3* l_velocity_v3, float l_rigidRadius_f)
{
    m_circleRadius_f              = 0.0f;
    m_radiusMultiplier_f          = 200.0f;
    m_detectionDistance_f         = 1000.0f;
    m_predictionTime_f            = 700.0f;

    m_currentPosition_v3 = l_position_v3;
    m_currentVelocity_v3 = l_velocity_v3;
    m_circleRadius_f = l_rigidRadius_f * m_radiusMultiplier_f;
    m_perpPlane_ptr = new PerpendicularPlane(*m_currentVelocity_v3, CalculateFuturePosition2(*m_currentVelocity_v3,
                                                                                             *m_currentPosition_v3,
                                                                                              m_predictionTime_f));
}

/**
 * @brief 
 * 
 */
void ObstacleSensor::Update()
{
    UpdatePerpendicularPlane();
}

/**
 * @brief 
 * 
 * @param l_normal_v3 
 * @param l_point_v3 
 */
void ObstacleSensor::UpdatePerpendicularPlane()
{
    m_perpPlane_ptr->Update(*m_currentVelocity_v3, CalculateFuturePosition2(*m_currentVelocity_v3, *m_currentPosition_v3, m_predictionTime_f));
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool ObstacleSensor::Detect()
{
    return false;
}

int ObstacleSensor::GetNumber()
{
    return 0;
}

/**
 * @brief: Use the contained circunference in the velocity's
 *         perpendicular Plane to determine which of the
 *         closest objects are a potential collision to avoid.
 * 
 * @return int 
 */
int ObstacleSensor::HowMany(const std::vector<Vec3>& l_closestObjects_v3, float l_distance_f, float l_visionAngle_f)
{
    /** Clear the obstacle candidates. */
    m_obstacleCandidates_v3.clear();

    m_obstacleCandidates_v3.reserve(l_closestObjects_v3.size());
    /** Auxiliar variable to accumulate the number of obstacle candidates. */
    int l_candidatesCount_n = 0;
    /** Go through all the closest objects given by the Octree. */
    for(size_t i = 0; i < l_closestObjects_v3.size(); i++)
    {
        /** If the distance between THIS and this close object <= detectionDistance */
        if(glm::distance(*m_currentPosition_v3, l_closestObjects_v3[i]) <= l_distance_f)
        {
            /** Create a a vector that goes from THIS position to the close object's position. */
            glm::vec3 m_centersVector = l_closestObjects_v3[i] - *m_currentPosition_v3;
            /** Calculate the angle between this vector and THIS velocity vector. */
            float l_angleBetween_f = PICASSO::GetAngleBetween2D(m_centersVector, *m_currentVelocity_v3);
            /** If this object in inside THIS vision angle. */
            if(l_angleBetween_f <= l_visionAngle_f)
            {
                /** Calculate the future position of THIS. */
                glm::vec3 l_futurePosition = CalculateFuturePosition2(*m_currentVelocity_v3, *m_currentPosition_v3, m_predictionTime_f);
                /** Calculate a point inside the velocity's perpendicular Plane as a candidate. */
                m_obstacleCandidates_v3.emplace_back
                (
                    GetObstacleCandidate2(m_perpPlane_ptr->ClosestPointPlane2(l_closestObjects_v3[i]), 
                                                                              l_futurePosition,
                                                                              m_circleRadius_f)
                );
                /** Increase the number of candidates. */
                l_candidatesCount_n++;
            }
        }
    }
    return l_candidatesCount_n;
}

/**
 * @brief 
 * 
 * @param l_closestObjects_v3 
 * @param l_distance_f 
 * @param l_visionAngle_f 
 * @return int 
 */
int ObstacleSensor::HowManyNitros(const std::map<int, Vec3>& l_closestObjects_v3, float l_distance_f, float l_visionAngle_f)
{
    /** [WARNING]: Using std::maps instead std::vector */
    m_nitroCandidates_v3.clear();

    int l_nitroCandidates_n = 0;
 
    for(auto const & nitroZone : l_closestObjects_v3)
    {
        if(glm::distance(*m_currentPosition_v3, nitroZone.second) <= l_distance_f)
        {
            /** Calculate the angle between this vector and THIS velocity vector. */
            float l_angleBetween_f = PICASSO::GetAngleBetween2D(nitroZone.second - *m_currentPosition_v3, *m_currentVelocity_v3);

            if(l_angleBetween_f <= l_visionAngle_f)
            {
                m_nitroCandidates_v3[nitroZone.first] = nitroZone.second;
                
                l_nitroCandidates_n++;
            }
        }
    }

    return l_nitroCandidates_n;
}

/**
 * @brief 
 * 
 * @param l_candidates_v3 
 * @return Vec3 
 */
Vec3 ObstacleSensor::CalculateAvoidanceTarget()
{
    /** Predict the future position of this AI entity. */
    glm::vec3 l_futurePosition_v3 = CalculateFuturePosition2(*m_currentVelocity_v3, 
                                                             *m_currentPosition_v3, 
                                                              m_predictionTime_f);
    /** Auxiliar variable. */
    glm::vec3 l_avoidanceTarget_v3 = glm::vec3(0,0,0);
    /** If there's only 1 obstacle to avoid, use it for calculations. */
    if(m_obstacleCandidates_v3.size() == 1)
    {
        l_avoidanceTarget_v3 = m_obstacleCandidates_v3[0];
    }
    /** Else, accumulate all of them, i.e the resultant vector. */
    else if(m_obstacleCandidates_v3.size() > 1)
    {
        for(size_t i = 0; i < m_obstacleCandidates_v3.size(); i++)
        {
            l_avoidanceTarget_v3 += m_obstacleCandidates_v3[i];
        }
        
    }
    /** To prevent a -NaN value by normalizing a 0 lenght vector. */
    if(glm::length(l_avoidanceTarget_v3) == 0)
    {
        return *m_currentVelocity_v3;
    }
    /** Calculate the avoidance target as: */
    l_avoidanceTarget_v3 = glm::normalize(l_avoidanceTarget_v3);
    l_avoidanceTarget_v3 = l_futurePosition_v3 + (l_avoidanceTarget_v3 * -1.0f * m_circleRadius_f);

    return l_avoidanceTarget_v3;
}