#pragma once

#include <TEntity.hpp>
#include <map>

class TCamera: public TEntity{
    public:
        TCamera(float near, float far, float zoom, const glm::vec3& l_position_v3);
        ~TCamera();

        void BeginDraw();
        void EndDraw();
        glm::mat4 Update();
        bool IsParalel() { return isParalel; };

        void SetPosition(const glm::vec3& l_position_v3) { m_position_v3 = l_position_v3; };
        void SetActive(bool state){ active = state; };
        void SetTarget(const glm::vec3& l_target_v3);
        void SetPerspectiveAngle(double l_angle);
        void SetPerspectiveProjection(float l_fieldOfView_f, float l_aspectRelation_f);
        void SetOrthoProjection();
        void SetProjectionLocation(unsigned int l_shaderId_n);

        bool GetActive(){ return active; };
        glm::vec3 GetTarget() { return m_target_v3; }

    private:
        unsigned int m_projectionLocation_n = 0;
        float near = 0.0f, far = 0.0f, left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f, zoom = 0.0f;
        double m_perspectiveAngle = 75.0f;
        bool isParalel = false;
        bool active = false;

        glm::vec3 m_position_v3     = glm::vec3(0.0f);
        glm::vec3 m_direction_v3    = glm::vec3(0.0f);
        glm::vec3 m_rightAxis_v3    = glm::vec3(0.0f);
        glm::vec3 m_upAxis_v3       = glm::vec3(0.0f);
        glm::vec3 m_target_v3       = glm::vec3(0.0f);
        glm::mat4 m_projection_mat4 = glm::mat4(1.0f);

};