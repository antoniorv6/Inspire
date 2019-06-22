/**
 * @file TLodMesh.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <TLod.hpp>
#include <string>

class TMesh;
class TResourceTexture;

class TLodMesh : public TLod
{
    public:
        TLodMesh
        (
            uint32_t l_levelsCount, 
            const std::vector<float>& l_distances, 
            glm::vec3& l_viewPosition,
            glm::vec3& l_position
        );
        virtual ~TLodMesh();

        void SetLodMesh(const std::string& l_meshName, const std::string& l_textureSource = "grid.jpg");
        void SetLodTexture(const std::string& l_textureSource);
        
        virtual void BeginDraw();
        virtual void EndDraw();
        virtual bool GetActive();

    private:
        std::vector<TMesh*> m_meshes;
        std::vector<TResourceTexture*> m_textures;
};
