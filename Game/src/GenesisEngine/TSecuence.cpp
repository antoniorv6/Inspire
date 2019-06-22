/**
 * @file TSecuence.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-06
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <TSecuence.hpp>
#include <TMesh.hpp>
#include <TResourceShader.hpp>

/**
 * @brief Construct a new TSecuence::TSecuence object
 * 
 * @param l_frames 
 */
TSecuence::TSecuence(const std::vector<std::string>& l_frames, int l_startPoint_n, bool l_loop)
{
    m_frames.reserve(l_frames.size());
    TMesh * mesh;

    for(uint32_t i = 0; i < l_frames.size(); i++)
    {
        mesh = new TMesh(MeshType::detailed);
        mesh->LoadMesh(l_frames[i]);
        m_frames.emplace_back(mesh);
    }

    m_framesNumber_n = m_frames.size();
}

/**
 * @brief Destroy the TSecuence::TSecuence object
 * 
 */
TSecuence::~TSecuence()
{
    std::vector<TMesh*> m_framesEmpty;

    for(uint32_t i = 0; i < m_frames.size(); i++)
    {
        delete m_frames[i];
        m_frames[i] = nullptr;
    }

    m_frames.swap(m_framesEmpty);
}

/**
 * @brief 
 * 
 * @param l_frame 
 */
void TSecuence::Draw(int l_frame)
{
    m_frames[l_frame]->BeginDraw();
}

/**
 * @brief 
 * 
 * @param l_texturePath 
 */
void TSecuence::SetTexture(const std::string& l_texturePath)
{
    for(uint32_t i = 0; i < m_frames.size(); i++)
    {
        m_frames[i]->SetTexture(l_texturePath);
    }
}