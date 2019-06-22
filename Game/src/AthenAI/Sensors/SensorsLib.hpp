/**
 * @file SensorsLib.hpp
 * @author Edward Owen (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <Utils.hpp>
#include <vector>
#include <map>
#include <PICASSO.hpp>

typedef glm::vec3 Vec3;
class Octree;

/**
 * @brief 
 * 
 */
struct SensorResult
{
    int l_objectsCount_n;
    Vec3 l_futurePosition;
    std::vector<Vec3> l_candidates_v3;
};

/**
 * @brief 
 * 
 */
class ISensor
{
public:
    ~ISensor(){};
    /** Common method to update all the Sensors. */
    virtual void Update()   = 0;
    /** Common method to check if a Target has been detected. */
    virtual bool Detect()   = 0;
    /** Returns the number of detected targets.  */
    virtual int GetNumber() = 0;
};

/**
 * @brief 
 * 
 */
class ObstacleSensor : public ISensor
{
public:
    ObstacleSensor(Vec3* l_position_v3, Vec3* l_velocity_v3, float l_rigidRadius_f);
    virtual ~ObstacleSensor();

    void Initialize(Vec3* l_position_v3, Vec3* l_velocity_v3, float l_rigidRadius_f);

    virtual void Update();
    virtual bool Detect();
    virtual int GetNumber();

    int HowMany(const std::vector<Vec3>& l_closestObjects_v3, float l_distance_f, float l_visionAngle_f);
    int HowManyNitros(const std::map<int, Vec3>& l_closestObjects_v3, float l_distance_f, float l_visionAngle_f);
    void UpdatePerpendicularPlane();
    Vec3 CalculateAvoidanceTarget();

    std::map<int, Vec3> GetNitroCandidates() { return m_nitroCandidates_v3; };

private:
    PerpendicularPlane * m_perpPlane_ptr;
    Vec3               * m_currentPosition_v3;
    Vec3               * m_currentVelocity_v3;

    float m_circleRadius_f;
    float m_radiusMultiplier_f;
    float m_detectionDistance_f;
    float m_predictionTime_f;

    std::vector<Vec3> m_obstacleCandidates_v3;
    std::map<int, Vec3> m_nitroCandidates_v3;
    std::vector<Vec3> m_closestObjects_v3;
};


