#include <TCamera.hpp>

/**
 * @brief Construct a new TCamera::TCamera object
 * 
 * @param n 
 * @param f 
 * @param l 
 * @param r 
 * @param t 
 * @param b 
 * @param z 
 * @param l_position_v3 
 */
TCamera::TCamera(float near, float far, float zoom, const glm::vec3& l_position_v3)
{
    this->near   = near;
    this->far    = far;
    this->zoom   = zoom;

    m_position_v3  = l_position_v3;
    m_direction_v3 = glm::normalize(m_position_v3 - glm::vec3(0,0,0));
    m_rightAxis_v3 = glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction_v3));
    m_upAxis_v3    = glm::cross(m_direction_v3, m_rightAxis_v3);
    active = false;
}

/**
 * @brief Destroy the TCamera::TCamera objects
 * 
 */
TCamera::~TCamera()
{

}

/**
 * @brief 
 * 
 */
void TCamera::BeginDraw()
{
    if(active)
    {
        //Get view and projection matrices
        SetPerspectiveProjection(m_perspectiveAngle, 4.0f/3.0f);
        //glUniformMatrix4fv(TEntity::viewLocation, 1, GL_FALSE, glm::value_ptr(TEntity::GetView()));
        //glUniformMatrix4fv(TEntity::projectionLocation, 1, GL_FALSE, glm::value_ptr(TEntity::GetProjection()));
    }
}

/**
 * @brief 
 * 
 */
void TCamera::EndDraw()
{

}

/**
 * @brief 
 * 
 * @return glm::mat4 
 */
glm::mat4 TCamera::Update()
{
    m_rightAxis_v3 = glm::normalize(glm::cross(glm::vec3(0, 1, 0), m_direction_v3));
    m_upAxis_v3    = glm::cross(m_direction_v3, m_rightAxis_v3);
    return glm::inverse(glm::lookAt(m_position_v3, m_target_v3, m_upAxis_v3));
}

/**
 * @brief 
 * 
 * @param l_fieldOfView_f 
 * @param l_aspectRelation_f 
 */
void TCamera::SetPerspectiveProjection(float l_fieldOfView_f, float l_aspectRelation_f)
{
    TEntity::GetProjection() = glm::perspective(glm::radians(l_fieldOfView_f), l_aspectRelation_f, near, far);
}

/**
 * @brief 
 * 
 * @param l_angle 
 */
void TCamera::SetPerspectiveAngle(double l_angle)
{
    m_perspectiveAngle = l_angle;
}

/**
 * @brief 
 * 
 */
void TCamera::SetOrthoProjection()
{
   TEntity::GetProjection() = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f); 
}

/**
 * @brief 
 * 
 * @param l_shaderId_n 
 */
void TCamera::SetProjectionLocation(unsigned int l_projectionLocation_n)
{
}

/**
 * @brief 
 * 
 * @param l_target_v3 
 */
void TCamera::SetTarget(const glm::vec3& l_target_v3)
{
    m_direction_v3 = glm::normalize(m_position_v3 - l_target_v3);
    m_target_v3 = l_target_v3;
}




