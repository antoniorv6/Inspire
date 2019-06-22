#pragma once
#include <TEntity.hpp>

class TLight : public TEntity{
    public:
        TLight(const glm::vec3& l_color, const glm::vec3& l_attenuation);
        ~TLight();

        void DrawLight(unsigned int modelLocation, unsigned int viewLocation, unsigned int projectionLocation);
        void BeginDraw();
        void EndDraw();

        void SetActive(bool state){ active = state; };
        bool GetActive(){ return active; };

        void SetUniformValues(unsigned int c_uniformPositionLocation_ui, 
                              unsigned int c_uniformAmbientLocation_ui,
                              unsigned int c_uniformDiffuseLocation_ui,
                              unsigned int c_uniformSpecularLocation_ui,
                              unsigned int c_uniformConstantLocation_ui,
                              unsigned int c_uniformLinearLocation_ui,
                              unsigned int c_uniformQuadraticLocation_ui);
        void SetShaderValues(const glm::vec3& c_lightPosition_v3);

        void SetModelMatrix(const glm::mat4& l_modelMatrix_m4) { m_modelMatrix_m4 = l_modelMatrix_m4; };
        void SetAmbient(const glm::vec3& c_newAmbient_v3) {  m_ambient_v3 = c_newAmbient_v3;  };
        void SetDiffuse(const glm::vec3& c_newDiffuse_v3) {  m_diffuse_v3 = c_newDiffuse_v3;  };
        void SetSpecular(const glm::vec3& c_newSpecular_v3) {  m_specular_v3 = c_newSpecular_v3;  };
        void SetIntensity(glm::vec3 l_intensity_vec3){ m_intensity = l_intensity_vec3; };
        glm::vec3 GetIntensity(){ return m_intensity; };

    private:
        bool active = false;
        GLuint VAO = 0, VBO = 0;
        
        unsigned int m_uniformPositionLocation_ui  = 0,
                     m_uniformAmbientLocation_ui   = 0,
                     m_uniformDiffuseLocation_ui   = 0,
                     m_uniformSpecularLocation_ui  = 0,
                     m_uniformConstantLocation_ui  = 0,
                     m_uniformLinearLocation_ui    = 0,
                     m_uniformQuadraticLocation_ui = 0;

        float m_vertices[9] = { 0.5f, -0.5f, 0.0f,   -0.5f, -0.5f, 0.0f,   0.0f, 0.5f, 0.0f };

        //Light properties
        glm::vec3 m_intensity      = glm::vec3(0.0f);
        glm::vec3 m_lightColor     = glm::vec3(0.0f);
        glm::vec3 m_ambient_v3     = glm::vec3(0.0f);
        glm::vec3 m_diffuse_v3     = glm::vec3(0.0f);
        glm::vec3 m_specular_v3    = glm::vec3(0.0f);
        glm::mat4 m_modelMatrix_m4 = glm::mat4(0.0f);

        //Attenuation variables
        float m_constant_f   = 0.0f;
        float m_linear_f     = 0.0f;
        float m_quadratic_f  = 0.0f;
};