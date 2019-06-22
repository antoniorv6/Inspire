/**
 * @file TResourceCubeMap.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <TResourceCubeMap.hpp>
#include <stb_image.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>

/**
 * @brief Construct a new TResourceCubeMap::TResourceCubeMap object
 * 
 * @param l_cubeMapTextures_v 
 */
TResourceCubeMap::TResourceCubeMap()
{

}

/**
 * @brief Destroy the TResourceCubeMap::TResourceCubeMap object
 * 
 */
TResourceCubeMap::~TResourceCubeMap()
{

}

/**
 * @brief Construct a new TResourceCubeMap::LoadResource object
 * 
 * @param c_resourceDocument_str 
 */
void TResourceCubeMap::LoadCubeMap(const std::vector<std::string>& l_faces_vs)
{
    unsigned int l_textureID_ui;
    glGenTextures(1, &l_textureID_ui);
    glBindTexture(GL_TEXTURE_CUBE_MAP, l_textureID_ui);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < l_faces_vs.size(); i++)
    {
        unsigned char *data = stbi_load(l_faces_vs[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            //std::cout << "[SUCCESS]: Cubemap texture loaded succesfully at path: " << l_faces_vs[i] << std::endl;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            //std::cout << "[ERROR]: Cubemap texture failed to load at path: " << l_faces_vs[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_cubeMapID_ui = l_textureID_ui;
}