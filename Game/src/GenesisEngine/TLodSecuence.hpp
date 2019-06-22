/**
 * @file TLodSecuence.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <ISecuence.hpp>
#include <vector>
#include <string>
#include <glm.hpp>

class TLodMesh;

class TLodSecuence : public ISecuence
{
    public:
        TLodSecuence
        (
            uint32_t l_framesNumber,
            uint32_t l_levelsCount, 
            const std::vector<float>& l_distances,
            glm::vec3& l_viewPosition,
            glm::vec3& l_position,
            int l_startPoint_n, 
            bool l_loop
        );
        virtual ~TLodSecuence();

        virtual void Draw(int l_frame);
        virtual void SetTexture(const std::string& l_texturePath);
        virtual size_t GetSize() { return m_lodMeshes.size(); };
        void SetPositionsRef(glm::vec3& l_position_ptr);
        void SetLodFrame(uint32_t l_frame, const std::string& l_meshName);
    private:
        std::vector<TLodMesh*> m_lodMeshes;

};