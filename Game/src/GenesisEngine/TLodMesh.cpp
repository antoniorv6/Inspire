/**
 * @file TLodMesh.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include<TLodMesh.hpp>
#include <TMesh.hpp>
#include <TResourceTexture.hpp>
#include <CommonData.hpp>
#include <ResourceManager.hpp>
/**
 * @brief Construct a new TLodMesh::TLodMesh object
 * 
 * @param l_levelsCount 
 * @param l_distances 
 * @param l_lodMeshes 
 */
TLodMesh::TLodMesh        
(
    uint32_t l_levelsCount, 
    const std::vector<float>& l_distances, 
    glm::vec3& l_viewPosition,
    glm::vec3& l_position

) : TLod(l_levelsCount, l_viewPosition, l_position)
{
    this->SetDistancesVector(l_distances);
    
    m_meshes.reserve(l_levelsCount);
}

/**
 * @brief Destroy the TLodMesh::TLodMesh object
 * 
 */
TLodMesh::~TLodMesh()
{
    for(uint32_t i = 0; i < m_meshes.size(); i++)
    {
        delete m_meshes[i];
        m_meshes[i] = nullptr;
    }
    std::vector<TResourceTexture*> emptyTextures;
    m_textures.swap(emptyTextures);
}

/**
 * @brief 
 * 
 * @param l_meshNamae 
 * @param l_textureSource 
 */
void TLodMesh::SetLodMesh(const std::string& l_meshName, const std::string& l_textureSource)
{
    TMesh * mesh = new TMesh(MeshType::detailed);
    mesh->LoadMesh(l_meshName);
    mesh->SetTexture(l_textureSource);

    m_meshes.push_back(mesh);
}

/**
 * @brief 
 * 
 * @param l_textureSource 
 */
void TLodMesh::SetLodTexture(const std::string& l_textureSource)
{
    std::vector<std::string> l_textureNames = RenderUtilities::getInstance()->GetLODTexturesNames(l_textureSource);

    for(uint32_t i = 0; i < l_textureNames.size(); i++)
    {
        m_textures.push_back( (TResourceTexture*)ResourceManager::getInstance()->GetTexture(l_textureNames[i]) );
    }
}

/**
 * @brief 
 * 
 */
void TLodMesh::BeginDraw()
{
    FindLod();

    if(m_meshes.size() > 0)
    {
        m_meshes[m_lastLOD]->SetTexture(m_textures[m_lastLOD]);
        m_meshes[m_lastLOD]->BeginDraw();
    }
}

/**
 * @brief 
 * 
 */
void TLodMesh::EndDraw()
{

}

/**
 * @brief 
 * 
 */
bool TLodMesh::GetActive()
{
    return true;
}