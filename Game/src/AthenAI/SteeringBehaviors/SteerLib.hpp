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
#include <Path.hpp>
#include <Utils.hpp>
#include <vector>

struct SteerResult
{
    Vec3 m_desiredVelocity_v3;
    Vec3 m_resultant_v3;
};

class SteerModule{

public:
    SteerModule(float l_mass_f, 
                float* l_maxSpeed_f, 
                float l_maxForce_f, 
                Vec3* l_position_v3, 
                Vec3* l_velocity_v3);

    ~SteerModule();

    void Initialize(float l_mass_f, 
                float* l_maxSpeed_f, 
                float l_maxForce_f, 
                Vec3* l_position_v3, 
                Vec3* l_velocity_v3);

    SteerResult SteerForSeek(const Vec3& l_target_v3);
    SteerResult NoLimitSeek(const Vec3& l_target_v3);
    SteerResult SteerToFollowPath(SPath* l_path);
    Vec3 LODFollowPath(SPath* l_path, float l_LODVelocity_ptr);
    Vec3 PredictFuturePosition(float l_predictionTime_f);

    int GetCurrentTarget()  const { return m_currentTarget_n; };
    int GetLastTarget()     const { return m_lastTargetIndex_n; };
    bool IncomingCurve()    const { return m_incomingCurve_b; };
    void NotIncomingCurve()  { m_incomingCurve_b = false; };
    void SetCurrentTarget(int l_target_n) { m_currentTarget_n = l_target_n; };

private:
    Vec3             * m_currentPosition_v3;
    Vec3             * m_currentVelocity_v3;

    int   m_currentTarget_n   = 0;
    int   m_lastTargetIndex_n = 0;
    int   m_direction_n       = 1;

    float m_mass_f          = 0.0f;
    float *m_maxSpeed_f     = nullptr;
    float m_maxForce_f      = 0.0f;
    
    bool m_incomingCurve_b = false;
};

/**
 * @brief Construct a new Steer Module:: Steer Module object
 * 
 * @param l_mass_f 
 * @param l_maxSpeed_f 
 * @param l_maxForce_f 
 * @param l_position_v3 
 * @param l_velocity_v3 
 */
SteerModule::SteerModule(float l_mass_f, 
                         float* l_maxSpeed_f, 
                         float l_maxForce_f, 
                         Vec3* l_position_v3, 
                         Vec3* l_velocity_v3)
{
    Initialize(l_mass_f, 
               l_maxSpeed_f, 
               l_maxForce_f, 
               l_position_v3, 
               l_velocity_v3);
}

/**
 * @brief Destroy the Steer Module:: Steer Module object
 * 
 */
SteerModule::~SteerModule()
{

}

/**
 * @brief 
 * 
 * @param l_mass_f 
 * @param l_maxSpeed_f 
 * @param l_maxForce_f 
 * @param l_position_v3 
 * @param l_velocity_v3 
 */
void SteerModule::Initialize(float l_mass_f, 
                             float* l_maxSpeed_f, 
                             float l_maxForce_f, 
                             Vec3* l_position_v3, 
                             Vec3* l_velocity_v3)
{
    m_mass_f              = l_mass_f;
    m_maxSpeed_f          = l_maxSpeed_f;
    m_maxForce_f          = l_maxForce_f;
    m_currentPosition_v3  = l_position_v3;
    m_currentVelocity_v3  = l_velocity_v3;
}

/**
 * @brief 
 * 
 * @param l_target_v3 
 * @return Vec3 
 */
SteerResult SteerModule::SteerForSeek(const Vec3& l_target_v3)
{
    /** Desired velocity with the MaxSpeed magnitude */
    Vec3 l_desiredVelocity_v3 = glm::normalize(l_target_v3 - *m_currentPosition_v3) * (*m_maxSpeed_f);
   
    /** Steering force to seek the target */
    Vec3 l_steeringForce_v3 = (l_desiredVelocity_v3 - *m_currentVelocity_v3);
 
    /** Limit the Steering Force by MaxForces (^ Force -> ^ Turn fastness) */
    LimitVectorByFactor(m_maxForce_f, l_steeringForce_v3);
 
    /** Take in count the Mass to determine how fast AI turns*/
    l_steeringForce_v3 = l_steeringForce_v3 / m_mass_f;
 
    /** Calculate the resultant force */
    Vec3 l_resultant_v3 = *m_currentVelocity_v3 + l_steeringForce_v3;
 
    /** Limit resultant force by MaxVelocity */
    LimitVectorByFactor(*m_maxSpeed_f, l_resultant_v3);

    /**Creates a SteerResult object to return it and use its relevant data. */
    SteerResult result;
    result.m_desiredVelocity_v3 = l_desiredVelocity_v3;
    result.m_resultant_v3       = l_resultant_v3;

    return result;
}

SteerResult SteerModule::NoLimitSeek(const Vec3& l_target_v3)
{
    /** Desired velocity with the MaxSpeed magnitude */
    Vec3 l_desiredVelocity_v3 = glm::normalize(l_target_v3 - *m_currentPosition_v3) * (*m_maxSpeed_f);
   
    /** Steering force to seek the target */
    Vec3 l_steeringForce_v3 = (l_desiredVelocity_v3 - *m_currentVelocity_v3);
 
    /** Calculate the resultant force */
    Vec3 l_resultant_v3 = *m_currentVelocity_v3 + l_steeringForce_v3;
 
    /** Limit resultant force by MaxVelocity */
    LimitVectorByFactor(*m_maxSpeed_f, l_resultant_v3);

    /**Creates a SteerResult object to return it and use its relevant data. */
    SteerResult result;
    result.m_desiredVelocity_v3 = l_desiredVelocity_v3;
    result.m_resultant_v3       = l_resultant_v3;

    return result;
}

/**
 * @brief 
 * 
 * @param l_direction_n 
 * @param l_predictionTime_f 
 * @param l_path 
 * @return Vec3 
 */
SteerResult SteerModule::SteerToFollowPath(SPath* l_path)
{
    /** Get the current Target */
    Vec3 l_target_v3 = l_path->m_waypoints_v3[m_currentTarget_n];
    /** Check the distance Between AI's current position and this Target
     *  If less than the distance  -> look at the next Target
     *  Else                       -> Continue Seeking the current Target
    */
    if(glm::distance(*m_currentPosition_v3, l_target_v3) <= l_path->m_waypointsRadius_f)
    {
        m_incomingCurve_b = true;

        m_lastTargetIndex_n = m_currentTarget_n;
        /** Next target by a given direction */
        m_currentTarget_n+=m_direction_n;
        /** If AI is in the end of the path -> Inverse the direction to going back (complete a LAP) */
        if(m_currentTarget_n >= (int)l_path->m_waypoints_v3.size() || m_currentTarget_n < 0){
            m_currentTarget_n = 0;
        }
    }
    /** For debug purposes */
    l_target_v3 = l_path->m_waypoints_v3[m_currentTarget_n];
    /** SteerForSeek */
    return SteerForSeek(l_target_v3);
}

/**
 * @brief 
 * 
 * @param l_direction_n 
 * @param l_predictionTime_f 
 * @param l_path 
 * @return Vec3 
 */
Vec3 SteerModule::LODFollowPath(SPath* l_path, float l_LODVelocity_ptr)
{
    /** Get the current Target */
    Vec3 l_target_v3 = l_path->m_waypoints_v3[m_currentTarget_n];
    /** Check the distance Between AI's current position and this Target
     *  If less than the distance  -> look at the next Target
     *  Else                       -> Continue Seeking the current Target
    */
    if(glm::distance(*m_currentPosition_v3, l_target_v3) <= l_path->m_waypointsRadius_f)
    {
        m_incomingCurve_b = true;

        m_lastTargetIndex_n = m_currentTarget_n;
        /** Next target by a given direction */
        m_currentTarget_n+=m_direction_n;
        /** If AI is in the end of the path -> Inverse the direction to going back (complete a LAP) */
        if(m_currentTarget_n >= (int)l_path->m_waypoints_v3.size() || m_currentTarget_n < 0){
            m_currentTarget_n = 0;
        }
    }
    /** For debug purposes */
    l_target_v3 = l_path->m_waypoints_v3[m_currentTarget_n];
    
    /** Desired velocity with the MaxSpeed magnitude */
    Vec3 l_desiredVelocity_v3 = glm::normalize(l_target_v3 - *m_currentPosition_v3) * l_LODVelocity_ptr;
    
    return l_desiredVelocity_v3;
}

/**
 * @brief 
 * 
 * @param l_predictionTime_f 
 * @return Vec3 
 */
Vec3 SteerModule::PredictFuturePosition(float l_predictionTime_f)
{
    return *m_currentPosition_v3 + (*m_currentVelocity_v3 * l_predictionTime_f);
}



