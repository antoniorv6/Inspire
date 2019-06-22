/**
 * @file Utils.hpp
 * @author Edward Owen           (contact@edwardowen.es)
 *         Iván Palacios Verano  (ivanpverano@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2018-12-18
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/closest_point.hpp>
#include <math.h>
#include <glm.hpp>
#include <limits>
#include <cassert>
#include <iostream>

/** Useful CONSTANTS to make operations */
const int     MaxInt    = (std::numeric_limits<int>::max)();
const double  MaxDouble = (std::numeric_limits<double>::max)();
const double  MinDouble = (std::numeric_limits<double>::min)();
const float   MaxFloat  = (std::numeric_limits<float>::max)();
const float   MinFloat  = (std::numeric_limits<float>::min)();

/**
 * @brief 
 * 
 */
struct PerpendicularPlane
{
  glm::vec3 normal;
  glm::vec3 point;
  float d = 0.0f;

  PerpendicularPlane(const glm::vec3& l_normal, const glm::vec3& l_point)
  {
      normal = glm::normalize(l_normal);
      point  = l_point;
      d = glm::dot(normal, point);
  };

  ~PerpendicularPlane(){};

  inline void Update(const glm::vec3& l_normal, const glm::vec3& l_point);
  inline float DistancePointPlane2(const glm::vec3 &l_point);
  inline glm::vec3 ClosestPointPlane2(const glm::vec3 &l_point);

};

/**
 * @brief 
 * 
 * @param l_normal 
 * @param l_point 
 */
inline void PerpendicularPlane::Update(const glm::vec3& l_normal, const glm::vec3& l_point)
{
    normal = glm::normalize(l_normal);
    point  = l_point;
    d = glm::dot(normal, point);
}

/**
 * @brief 
 * 
 * @param l_point 
 * @return float 
 */
inline float PerpendicularPlane::DistancePointPlane2(const glm::vec3 &l_point)
{
    return (glm::dot(normal, l_point) - d);
};

/**
 * @brief 
 * 
 * @param l_point 
 * @return glm::vec3 
 */
inline glm::vec3 PerpendicularPlane::ClosestPointPlane2(const glm::vec3 &l_point)
{
    float dist = DistancePointPlane2(l_point);

    return l_point - dist * normal;
};

/**
 * @brief 
 * 
 * @param a 
 * @param b 
 * @return true 
 * @return false 
 */
inline bool IsEqual(double a, double b)
{
  if (fabs(a-b) < 1E-12)
  {
    return true;
  }

  return false;
}

/**
 * @brief 
 * 
 * @tparam T 
 * @param a 
 * @param b 
 * @return T 
 */
//returns the minimum of two values
template <class T>
inline T MinOf(const T& a, const T& b)
{
    if (a<b) 
    return a; 

    return b;
}

/**
 * @brief 
 * 
 * @param l_factor_f 
 * @param l_vector_v3 
 */
inline void LimitVectorByFactor(const float& l_factor_f, glm::vec3& l_vector_v3)
{
    l_vector_v3 = glm::length(l_vector_v3) < l_factor_f ? l_vector_v3 : glm::normalize(l_vector_v3) * l_factor_f;
}

/**
 * @brief 
 * 
 * @param l_factor 
 * @param l_angle 
 */
inline void LimitAngleByFactor(const float& l_factor, float& l_angle)
{
    l_angle = l_angle < l_factor ? l_angle : l_factor;
}

/**
 * @brief 
 * 
 * @param name 
 * @param l_vector 
 */
inline void show_vector(std::string name, glm::vec3 l_vector)
{
    std::cout << name << " vector: (" << l_vector.x << ", " << l_vector.y << ", " << l_vector.z << ")\n";
};

/**
 * @brief 
 * 
 * @param vel_v3 
 * @return float 
 */
inline float CalculateSpinAngle2(const glm::vec3& vel_v3)
{
    float dot = vel_v3.y;             //dot product between [x1, y1] and [x2, y2] -> dot = x1*x2 + y1*y2
    float det = vel_v3.x;             //det = x1*y2 - y1*x2  
    float angle = atan2(det, dot);    //arctangent

    return -1 * angle * 180 / M_PI;
}

/**
 * AIFlyComponent::CalculateFuturePosition
 *
 * @brief
 *
 */
inline glm::vec3 CalculateFuturePosition2(const glm::vec3& l_velocity, const glm::vec3& l_position, float l_distance)
{
    /** Scaling Velocity vector by a N factor (N could be based on velocity) */
    glm::vec3 predict = (glm::normalize(l_velocity)) * l_distance;
    /** Calculate the future Position */
    return  l_position + predict;
}

/**
 * AISensorComponent
 *
 * @brief
 *
 * @param l_projection
 * @return glm::vec3
 */
inline glm::vec3 GetObstacleCandidate2(const glm::vec3& l_projection_v3, const glm::vec3& l_futurePosition_v3, float l_radius_f)
{
    return glm::normalize(l_projection_v3 - l_futurePosition_v3) * l_radius_f;
}

/**
 * @brief 
 * 
 * @param l_variable_f 
 * @param l_min_f 
 * @param l_newMin_f 
 * @param l_max_f 
 * @param l_newMax 
 */
inline void LimitFuzzyValues(float& l_variable_f, float l_min_f, float l_newMin_f, float l_max_f, float l_newMax)
{
    if(l_variable_f <= l_min_f)
    {
        l_variable_f = l_newMin_f;
    }
    else if(l_variable_f >= l_max_f)
    {
        l_variable_f = l_newMax;
    }
}

inline void ShowVector(std::string name, const glm::vec3& l_vector)
{
    std::cout << name << " vector: (" << l_vector.x << ", " << l_vector.y << ", " << l_vector.z << ")\n";
};

/**
 * @brief 
 * 
 * @param l_currentPosition_v3 
 * @param l_futurePosition_v3 
 * @param l_nitroZonePosition_v3 
 * @return float 
 */
inline float CalculateDeviationDistance(const glm::vec3& l_currentPosition_v3, 
                                        const glm::vec3& l_futurePosition_v3, 
                                        const glm::vec3& l_nitroZonePosition_v3)
{
    //ShowVector("Waypoint", l_futurePosition_v3);
    //ShowVector("NitroZone", l_nitroZonePosition_v3);

    return glm::distance(glm::closestPointOnLine(l_nitroZonePosition_v3, l_currentPosition_v3, l_futurePosition_v3), l_nitroZonePosition_v3);
}



/**
 * @brief
 *
 * @param l_a_v3
 * @param l_b_v3
 */
inline glm::vec3 CalculateNormalPoint(const glm::vec3& l_A_v3, const glm::vec3& l_B_v3, const glm::vec3& l_futurePosition_v3)
{
    /** AP segment (SegmentStartPoint -> FuturePosition) */
    glm::vec3 l_AP_v3 = l_futurePosition_v3 - l_A_v3;
    /** AB segment (SegmentEndPosition -> SegmentStartPosition) */
    glm::vec3 l_AB_v3 = l_B_v3 - l_A_v3;
    /** Normalize the line segment */
    l_AB_v3 = glm::normalize(l_AB_v3);
    /** Scalar projection AP over AB */
    l_AB_v3 = l_AB_v3 * (glm::dot(l_AP_v3, l_AB_v3));
 
    /** Return the NormalPoint */
    return l_A_v3 + l_AB_v3;
}