/**
 * @file TSkybox.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <TEntity.hpp>
#include <string>
#include <vector>

class TResourceCubeMap;
class TResourceShader;

class TSkybox : public TEntity
{
public:
    TSkybox(const std::vector<std::string>& l_cubeMapTextures);
    virtual ~TSkybox();

    virtual void BeginDraw(){};
    virtual void EndDraw(){};
    virtual bool GetActive(){ /*[WARNING] NO RETURN*/ return false;}

    void CreateVariables();
    void Draw();

    unsigned int GetSkyboxID();
    unsigned int GetVAO() { return VAO; };
    unsigned int GetVBO() { return VBO; };

private:
    GLuint VAO = 0, VBO = 0;
    TResourceCubeMap* m_cubeMap_ptr   = nullptr;
    TResourceShader*  m_shader_ptr    = nullptr;

    float skyboxVertices[108] = 
    {         
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
};