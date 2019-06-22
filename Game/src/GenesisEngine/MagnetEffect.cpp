/**
 * @file MagnetEffect.cpp
 * @author Edward Owen (contact@edwardowen.es)
 * @brief 
 * @version 0.1
 * @date 2019-04-26
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <MagnetEffect.hpp>
#include <TResourceShader.hpp>
#include <CommonData.hpp>

/**
 * @brief Construct a new Magnet Effect:: Magnet Effect object
 * 
 * @param l_screenWidth 
 * @param l_screenHeight 
 * @param l_shader_ptr 
 */
MagnetEffect::MagnetEffect(uint32_t l_screenWidth, uint32_t l_screenHeight, TResourceShader * l_shader_ptr, const std::string& l_ID) : IEffect(l_shader_ptr, l_ID)
{
    m_renderUtilities_ptr = RenderUtilities::getInstance();
    Init(l_screenWidth, l_screenHeight);
}

/**
 * @brief Destroy the Magnet Effect:: Magnet Effect object
 * 
 */
MagnetEffect::~MagnetEffect()
{
    this->DeleteBuffers();
}

/**
 * @brief Construct a new Magnet Effect:: Init object
 * 
 * @param l_screenWidth 
 * @param l_screenHeight 
 */
void MagnetEffect::Init(uint32_t l_screenWidth, uint32_t l_screenHeight)
{
    m_screenWidth  = l_screenWidth;
    m_screenHeight = l_screenHeight;
    m_shader_ptr->UseShader();
    m_shader_ptr->SetInt("screenTexture", 0);

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // framebuffer configuration
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, l_screenWidth, l_screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, l_screenWidth, l_screenHeight); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); 

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        //std::cout << "ERROR::MAGNET::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    else
    {
        //std::cout << "SUCCESS::MAGNET::FRAMEBUFFER:: Framebuffer complete!" << std::endl;
    }
        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief 
 * 
 * @param dt 
 */
void MagnetEffect::Draw(float dt)
{
    m_renderUtilities_ptr->UpdateMagnetValues();

    if(!m_renderUtilities_ptr->IsMagnetDesactivated())
    {
        /**
         *  Bind back to default framebuffer and draw a quad plane 
         *  with the attached framebuffer color texture. 
         */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        /**
         *  Disable depth test,
         *  so screen-space quad isn't discarded due to depth test.
         */
        glDisable(GL_DEPTH_TEST); 
        // Clear all relevant buffers.
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_shader_ptr->UseShader();
        m_shader_ptr->SetFloat("_deltaTime", dt);
        m_shader_ptr->SetFloat("_amplitude", m_renderUtilities_ptr->GetAmplitude());
        m_shader_ptr->SetFloat("_refraction", m_renderUtilities_ptr->GetRefraction());
        m_shader_ptr->SetFloat("_width", m_renderUtilities_ptr->GetWidth());

        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);	
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else
    {
        m_finished = true;
        m_renderUtilities_ptr->ResetMagnetValues();
    }
}