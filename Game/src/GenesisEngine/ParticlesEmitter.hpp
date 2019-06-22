/**
 * @file ParticlesEmitter.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <glm.hpp>
#include <vector>
#include <string>

class Particle;
class TResourceShader;
class TResourceTexture;

class ParticlesEmitter{
public:
    ParticlesEmitter(
                        TResourceShader * l_shader, 
                        const std::string& l_mesh, 
                        const std::string& l_texture, 
                        const glm::vec3& l_position, 
                        uint32_t l_pAmount
                    );
    ~ParticlesEmitter();

    void Init(const std::string& l_mesh, const std::string& l_texture);
    void Update(float dt, uint32_t l_howManyNews, const glm::vec3& l_offset = glm::vec3(0.0f));
    void Draw(float dt);

    uint32_t Find1stUnused();
    void RespawnParticle(Particle* l_particle, const glm::vec3& l_offset = glm::vec3(0.0f));

private:
    uint32_t VAO;
    uint32_t m_pAmount          = 0;
    uint32_t m_lastUsedParticle = 0;
    float respawnTime           = 0.0f;

    glm::vec3 m_position = glm::vec3(0.0f);
    std::vector<Particle*> m_particles;
    
    TResourceShader * m_shader_ptr = nullptr;

};