/**
 * @file TLodSecuence.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <TLodSecuence.hpp>
#include <TLodMesh.hpp>
#include <TResourceTexture.hpp>

/**
 * @brief Construct a new TLodSecuence::TLodSecuence object
 * 
 * @param l_levelsCount 
 * @param l_distances 
 * @param l_viewPosition 
 */
TLodSecuence::TLodSecuence(uint32_t l_framesNumber, uint32_t l_levelsCount, const std::vector<float>& l_distances, glm::vec3& l_viewPosition, glm::vec3& l_position, int l_startPoint_n, bool l_loop)
{
    m_lodMeshes.reserve(l_framesNumber);

    for(uint32_t i = 0; i < l_framesNumber; i++)
    {
        m_lodMeshes.emplace_back(new TLodMesh(l_levelsCount, l_distances, l_viewPosition, l_position));
    }

    m_framesNumber_n = m_lodMeshes.size();
    m_loop = l_loop;
}

/**
 * @brief Destroy the TLodSecuence::TLodSecuence object
 * 
 */
TLodSecuence::~TLodSecuence()
{
    std::vector<TLodMesh*> m_lodMeshesEmpty;
    for(uint32_t i = 0; i < m_lodMeshes.size(); i++)
    {
        delete m_lodMeshes[i];
        m_lodMeshes[i] = nullptr;
    }

    m_lodMeshes.swap(m_lodMeshesEmpty);
}

/**
 * @brief 
 * 
 * @param l_frame 
 */
void TLodSecuence::Draw(int l_frame)
{
    m_lodMeshes[l_frame]->BeginDraw();
}

/**
 * @brief 
 * 
 * @param l_frame 
 * @param l_meshName 
 */
void TLodSecuence::SetLodFrame(uint32_t l_frame, const std::string& l_meshName)
{
    m_lodMeshes[l_frame]->SetLodMesh(l_meshName);
}

/**
 * @brief 
 * 
 * @param l_texturePath 
 */
void TLodSecuence::SetTexture(const std::string& l_texturePath)
{
    for(uint32_t i = 0; i < m_lodMeshes.size(); i++)
    {
        m_lodMeshes[i]->SetLodTexture(l_texturePath);
    }
}