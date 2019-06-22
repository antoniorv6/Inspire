#include <TLight.hpp>
#include <glew.h>

/**
 * @brief Construct a new TLight::TLight object
 * 
 */
TLight::TLight(const glm::vec3& l_color, const glm::vec3& l_attenuation)
{
    active        = false;

    m_intensity   = glm::vec3(0,0,0);
    m_lightColor  = l_color;

    glm::vec3 diffuseColor = m_lightColor * glm::vec3(0.9f); 
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.6f);

    m_ambient_v3  = ambientColor;
    m_diffuse_v3  = diffuseColor;
    m_specular_v3 = glm::vec3(1.0f, 1.0f, 1.0f);

    // =========== ATTENUATION TABLE ========== //
    /**  Distance	Constant  Linear	Quadratic
    *       7	      1.0	    0.7	       1.8
    *       13	      1.0	    0.35	   0.44
    *       20	      1.0	    0.22	   0.20
    *       32	      1.0	    0.14	   0.07
    *       50	      1.0	    0.09	   0.032
    *       65	      1.0	    0.07	   0.017
    *       100	      1.0	    0.045	   0.0075
    *       160	      1.0	    0.027	   0.0028
    *       200	      1.0	    0.022	   0.0019
    *       325	      1.0	    0.014	   0.0007
    *       600	      1.0	    0.007	   0.0002
    *       3250	  1.0	    0.0014     0.000007
    */

    // m_constant_f  = 1.0f;
    // m_linear_f    = 0.0000000000014f;
    // m_quadratic_f = 0.000000000000000007f;
    // VAO           = 0;
    // VBO           = 0;

    m_constant_f  = l_attenuation.x;
    m_linear_f    = l_attenuation.y;
    m_quadratic_f = l_attenuation.z;
    VAO           = 0;
    VBO           = 0;
    
    /** Create empty VAO (Vertex Attribute Object). */
    glGenVertexArrays(1, &VAO);
    /** Create empty VBO (Vertex Buffer Object). */
    glGenBuffers(1, &VBO);

    /** Bind the Vertex Array Object previously created. */
    glBindVertexArray(VAO);
    /** Bind the Vertex Buffer Object previously created to the Buffer. */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /** [WARNING]: GL_STATIC_DRAW -> GL_STREAM_DRAW. */
    //Copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), m_vertices, GL_STATIC_DRAW);

    //This vertex attribute pointer is for the vertices handling
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Unbinding VAO & VBO & EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief Destroy the TLight::TLight object
 * 
 */
TLight::~TLight()
{

}

/**
 * @brief 
 * 
 * @param c_uniformPositionLocation_ui 
 * @param c_uniformAmbientLocation_ui 
 * @param c_uniformDiffuseLocation_ui 
 * @param c_uniformSpecularLocation_ui 
 */
void TLight::SetUniformValues(unsigned int c_uniformPositionLocation_ui, 
                              unsigned int c_uniformAmbientLocation_ui,
                              unsigned int c_uniformDiffuseLocation_ui,
                              unsigned int c_uniformSpecularLocation_ui,
                              unsigned int c_uniformConstantLocation_ui,
                              unsigned int c_uniformLinearLocation_ui,
                              unsigned int c_uniformQuadraticLocation_ui
                              )
{
    m_uniformPositionLocation_ui  = c_uniformPositionLocation_ui;
    m_uniformAmbientLocation_ui   = c_uniformAmbientLocation_ui;
    m_uniformDiffuseLocation_ui   = c_uniformDiffuseLocation_ui;
    m_uniformSpecularLocation_ui  = c_uniformSpecularLocation_ui;
    m_uniformConstantLocation_ui  = c_uniformConstantLocation_ui;
    m_uniformLinearLocation_ui    = c_uniformLinearLocation_ui;
    m_uniformQuadraticLocation_ui = c_uniformQuadraticLocation_ui;
}

/**
 * @brief 
 * 
 * @param c_lightPosition_v3 
 */
void TLight::SetShaderValues(const glm::vec3& c_lightPosition_v3)
{
    glUniform3f(m_uniformPositionLocation_ui, c_lightPosition_v3.x, c_lightPosition_v3.y, c_lightPosition_v3.z);
    glUniform3f(m_uniformAmbientLocation_ui, m_ambient_v3.x, m_ambient_v3.y, m_ambient_v3.z);
    glUniform3f(m_uniformDiffuseLocation_ui, m_diffuse_v3.x, m_diffuse_v3.y, m_diffuse_v3.z);
    glUniform3f(m_uniformSpecularLocation_ui, m_specular_v3.x, m_specular_v3.y, m_specular_v3.z);
    glUniform1f(m_uniformConstantLocation_ui, m_constant_f);
    glUniform1f(m_uniformLinearLocation_ui, m_linear_f);
    glUniform1f(m_uniformQuadraticLocation_ui, m_quadratic_f);
}

/**
 * @brief 
 * 
 */
void TLight::DrawLight(unsigned int modelLocation, unsigned int viewLocation, unsigned int projectionLocation)
{
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_modelMatrix_m4));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(TEntity::GetView()));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(TEntity::GetProjection()));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

/**
 * @brief 
 * 
 */
void TLight::BeginDraw(){
    
}

/**
 * @brief 
 * 
 */
void TLight::EndDraw()
{
    
}