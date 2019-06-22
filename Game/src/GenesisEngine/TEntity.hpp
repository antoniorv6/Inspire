#pragma once

#include <stack>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glew.h>
#include <iostream>
#include <string>
#include <map>
#include <gtc/type_ptr.hpp>

class TEntity{
    public:
        TEntity(){};
        virtual ~TEntity(){};

        virtual void BeginDraw() = 0;
        virtual void EndDraw() = 0;

        virtual bool GetActive() = 0;

        static std::stack<glm::mat4>& GetStack() { return m_transform_stack; };
        static glm::mat4& GetModel() { return m_model_mat4; };
        static glm::mat4& GetView() { return m_view_mat4; };
        static glm::mat4& GetProjection() { return m_projection_mat4; };

        static void SetView(const glm::mat4 &l_view_mat4) { m_view_mat4 = l_view_mat4; };
        static void SetModelLocation(int c_loc) { modelLocation = c_loc;};
        static void SetModelViewProjectionLocation(int c_loc) {modelViewProjectionLocation = c_loc;};
        static void SetViewProjection()
        {
            m_ViewProjection_mat4 = m_projection_mat4 * m_view_mat4;
        };

    private:
        int m_type = 0;

    protected:
        static glm::mat4 m_model_mat4;
        static glm::mat4 m_view_mat4;
        static glm::mat4 m_projection_mat4;
        static glm::mat4 m_ViewProjection_mat4;
        static std::stack<glm::mat4> m_transform_stack;
        static int modelLocation;
        static int modelViewProjectionLocation;   
};