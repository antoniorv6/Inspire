#include <TText.hpp>
#include <TResourceShader.hpp>
#include <TResourceFont.hpp>
#include <ResourceManager.hpp>


TText::TText(float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation)
{
    m_xPos = c_xPos;
    m_yPos = c_yPos;
    m_scale = c_Scale;
    m_textToRender = c_textToRender;
    m_font = (TResourceFont*) ResourceManager::getInstance()->GetFont(c_fontLocation);
    m_shader = (TResourceShader*) ResourceManager::getInstance()->GetShader("assets/shaders/text");

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
        
void TText::BeginDraw()
{
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    std::string::const_iterator l_iterator;
    GLfloat l_xPos = m_xPos;

    for(l_iterator = m_textToRender.begin(); l_iterator != m_textToRender.end(); l_iterator++)
    {
        TCharacter l_charToRender = m_font->ReturnCharacter(*l_iterator);
        
        GLfloat l_xToRender = l_xPos + l_charToRender.m_Bearing.x * m_scale;
        GLfloat l_yToRender = m_yPos - (l_charToRender.m_Size.y - l_charToRender.m_Bearing.y) * m_scale;
        GLfloat l_widthToRender = l_charToRender.m_Size.x * m_scale;
        GLfloat l_heightToRender = l_charToRender.m_Size.y * m_scale;

        GLfloat l_verticesToRender [6][4] = {
            {l_xToRender, l_yToRender + l_heightToRender, 0.0, 0.0},
            {l_xToRender, l_yToRender,                    0.0, 1.0},
            {l_xToRender + l_widthToRender, l_yToRender,  1.0, 1.0},
            
            {l_xToRender, l_yToRender + l_heightToRender, 0.0, 0.0},
            {l_xToRender + l_widthToRender, l_yToRender,  1.0, 1.0},
            {l_xToRender + l_widthToRender, l_yToRender+ l_heightToRender, 1.0,0.0}
        };

        glBindTexture(GL_TEXTURE_2D, l_charToRender.m_TextureID);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(l_verticesToRender), l_verticesToRender);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0,6);

        l_xPos += (l_charToRender.m_advance >> 6) * m_scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}