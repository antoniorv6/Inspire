/**
 * @file TResourceCubeMap.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-03-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <TResource.hpp>
#include <vector>
#include <string>

class TResourceCubeMap : public TResource
{
public:
    TResourceCubeMap();
    virtual ~TResourceCubeMap();
    virtual void LoadResource(const std::string& c_resourceDocument_str){};
    void LoadCubeMap(const std::vector<std::string>& l_faces_vs);
    unsigned int GetCubeMapTextureID() { return m_cubeMapID_ui; };

private:
    unsigned int m_cubeMapID_ui;
};