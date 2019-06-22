#include <TResourceTexture.hpp>
#include <TResourceShader.hpp>
#include <ResourceManager.hpp>
#include <TBillBoard.hpp>

TBillBoard::TBillBoard(float l_width_f, float l_height_f)
{
    m_texture = nullptr;
    m_resourceManager_ptr = ResourceManager::getInstance();
    m_shader_ptr = (TResourceShader*)m_resourceManager_ptr->GetShader("assets/shaders/billboard");
    m_position_v3 = glm::vec3(0,0,0);
    m_width_f = l_width_f;
    m_height_f = l_height_f;
    glGenVertexArrays(1, &m_VAO_uint);
    glGenBuffers(1, &m_vertexBuffer_uint);
}

void TBillBoard::SetTexture(const std::string& c_newTexture_string)
{
    m_texture = (TResourceTexture*)m_resourceManager_ptr->GetTexture(c_newTexture_string); 
}

void TBillBoard::SetPosition(glm::vec3 c_newPosition_v3)
{ 
    m_position_v3 = c_newPosition_v3; 
};

void TBillBoard::BeginDraw()
{
    if(m_texture)
    {   m_shader_ptr->SetFloat("width", m_width_f);
        m_shader_ptr->SetFloat("height", m_height_f);    
        glActiveTexture(GL_TEXTURE0);        
        glBindTexture(GL_TEXTURE_2D, m_texture->GetID());         
        glBindVertexArray(m_VAO_uint);        
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_uint);                
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_position_v3), &m_position_v3[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 0, (void*)0);                
        glEnableVertexAttribArray(0);         
        glDrawArrays(GL_POINTS, 0, 1);         
        glBindBuffer(GL_ARRAY_BUFFER, 0);        
        glBindVertexArray(0);
    }
}