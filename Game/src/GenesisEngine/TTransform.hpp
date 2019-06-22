#pragma once

#include <TEntity.hpp>

enum TTransformType 
{
    Rotation = 0,
    Translation = 1,
    Scale = 2,
    RotationAroundParent = 3
};

class TTransform: public TEntity {
    public:
        TTransform(const TTransformType& c_type){
            m_type = c_type;
            matrix = glm::mat4(1.0);
        };
        ~TTransform(){};

        void Identity();
        void Inverse();
        void Transpose();

        void SetMatrix(const glm::mat4& _matrix) { matrix = _matrix; };
        glm::mat4 GetMatrix() { return matrix; };
        bool GetActive(){ return false; };

        void Translate(const glm::vec3& l_translation_v3);
        void Rotate(const glm::vec3& axis, float angle);
        void Scale(const glm::vec3& l_scale_v3);

        void BeginDraw();
        void EndDraw();

    private:
        glm::mat4 matrix = glm::mat4(1.0f);
        TTransformType m_type;
};