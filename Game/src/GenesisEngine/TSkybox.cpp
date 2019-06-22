/**
 * @file TSkybox.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <TSkybox.hpp>
#include <ResourceManager.hpp>
#include <TResourceCubeMap.hpp>
#include <TResourceShader.hpp>
#include <TEntity.hpp>

/**
 * @brief Construct a new TSkybox::TSkybox object
 * 
 * @param l_cubeMapTextures 
 */
TSkybox::TSkybox(const std::vector<std::string>& l_cubeMapTextures)
{
    VAO = 0;
    VBO = 0;
    m_cubeMap_ptr = ResourceManager::getInstance()->LoadCubeMap(l_cubeMapTextures);
    CreateVariables();
}

/**
 * @brief Destroy the TSkybox::TSkybox object
 * 
 */
TSkybox::~TSkybox()
{
    if(VAO!= 0)
    {
        glDeleteBuffers(1, &VAO);
        VAO = 0;
    }

    if(VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
}

/**
 * @brief 
 * 
 */
void TSkybox::CreateVariables()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

/**
 * @brief 
 * 
 */
void TSkybox::Draw()
{

}

/**
 * @brief 
 * 
 * @return unsigned int 
 */
unsigned int TSkybox::GetSkyboxID()
{
    return m_cubeMap_ptr->GetCubeMapTextureID();
}