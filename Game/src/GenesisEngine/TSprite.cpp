#include <TSprite.hpp>
#include <ResourceManager.hpp>
#include <TResourceTexture.hpp>
#include <TResourceShader.hpp>
#include <glew.h>

/**
 * @brief Construct a new TSprite::TSprite object
 * 
 * @param c_newTexture_string 
 * @param c_width 
 * @param c_height 
 */
TSprite::TSprite(const std::string& c_newTexture_string, int c_width, int c_height, int c_xPos, int c_yPos)
{
    m_width = c_width;
    m_height = c_height;
    m_xPos = c_xPos; 
    m_yPos = c_yPos;
    m_resourceManager_ptr = ResourceManager::getInstance();
    m_texture_ptr = (TResourceTexture*)m_resourceManager_ptr->GetTexture(c_newTexture_string);
    //We have the texture loaded into our sprite

    m_shader_ptr = (TResourceShader*)m_resourceManager_ptr->GetShader("assets/shaders/sprites");

    //INITIALISING QUAD
    unsigned int l_VBO_ui;
    GLfloat l_vertices_f[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &m_VAO_ui);
    glGenBuffers(1, &l_VBO_ui);

    glBindBuffer(GL_ARRAY_BUFFER, l_VBO_ui);
    glBufferData(GL_ARRAY_BUFFER, sizeof(l_vertices_f), l_vertices_f, GL_STATIC_DRAW);

    glBindVertexArray(m_VAO_ui);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //END OF QUAD INITIALISATION

    CalculateModelMatrix();

    m_active = true;
    
}

void TSprite::ScaleSprite(int c_width, int c_height)
{
    m_width = c_width;
    m_height = c_height;
    
    CalculateModelMatrix();
}

void TSprite::CalculateModelMatrix()
{
    m_modelMatrix_m4 = glm::mat4(1.0);

    //Now we calculate the model matrix that has to be applied during draw time
    m_modelMatrix_m4 = glm::translate(m_modelMatrix_m4, glm::vec3(m_xPos, m_yPos, 0.0f));
    
    m_modelMatrix_m4 = glm::translate(m_modelMatrix_m4, glm::vec3(-0.5f * m_width, -0.5f*m_height, 0.0f));
    
    m_modelMatrix_m4 = glm::scale(m_modelMatrix_m4, glm::vec3(m_width, m_height, 1.0f));
}

void TSprite::BeginDraw()
{
    if(m_active)
    {
        m_shader_ptr->SetMat4f("ModelMat",m_modelMatrix_m4);
    
        glActiveTexture(GL_TEXTURE0);        
        glBindTexture(GL_TEXTURE_2D, m_texture_ptr->GetID());

        glBindVertexArray(m_VAO_ui);
        glDrawArrays(GL_TRIANGLES, 0,6);
        glBindVertexArray(0);
    }
}

void TSprite::SetTexture(const std::string& c_newTexture_string)
{
    m_texture_ptr = (TResourceTexture*)m_resourceManager_ptr->GetTexture(c_newTexture_string);
}


