/**
 * @file IEffect.hpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-04-23
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <glew.h>
#include <glfw3.h>
#include <string>

class TResourceShader;

class IEffect
{
public:
    IEffect(TResourceShader * l_shader_ptr, const std::string& l_ID) 
    { 
        m_shader_ptr = l_shader_ptr; 
        m_ID = l_ID;
    };
    virtual ~IEffect(){};
    virtual void Init(uint32_t l_screenWidth, uint32_t l_screenHeight) = 0;
    virtual void SetShadingValues() = 0;
    virtual void Draw(float dt) = 0;

    // [WARNING] : PROBABLY BINDING MULTIPLE FRAME BUFFERS WILL PRODUCE ERRORS.
    void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer); };

    void DeleteBuffers() 
    {  
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteFramebuffers(1, &frameBuffer);
        glDeleteTextures(1, &textureColorBuffer);
        glDeleteRenderbuffers(1, &RBO);
    }

    std::string GetID() { return m_ID; };
    bool IsFinished() { return m_finished; };
    void ResetFinished() { m_finished = false; };

protected:
    std::string m_ID = "";
    bool m_finished = false;
    TResourceShader * m_shader_ptr = nullptr;

    uint32_t quadVAO = 0, quadVBO = 0, RBO = 0, frameBuffer = 0, textureColorBuffer = 0;
    /** 
     *  Vertex attributes for a quad that fills 
     *  the entire screen in Normalized Device Coordinates.
     */
    float quadVertices[24] = 
    { 
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
};