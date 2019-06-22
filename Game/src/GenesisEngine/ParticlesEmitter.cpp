/**
 * @file ParticlesEmitter.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <ParticlesEmitter.hpp>
#include <Particle.hpp>
#include <TResourceShader.hpp>
#include <TResourceTexture.hpp>
#include <TEntity.hpp>
#include <CommonData.hpp>
/**
 * @brief Construct a new Particles Emitter:: Particles Emitter object
 * 
 * @param l_shader 
 * @param l_texture 
 * @param l_position 
 * @param l_pAmount 
 */
ParticlesEmitter::ParticlesEmitter
(                        
    TResourceShader * l_shader, 
    const std::string& l_mesh, 
    const std::string& l_texture, 
    const glm::vec3& l_position, 
    uint32_t l_pAmount
)
{
    m_pAmount     = l_pAmount;
    m_position    = l_position;
    m_shader_ptr  = l_shader;

    Init(l_mesh, l_texture);
}

/**
 * @brief Destroy the Particles Emitter:: Particles Emitter object
 * 
 */
ParticlesEmitter::~ParticlesEmitter()
{
    for(uint32_t i = 0; i < m_particles.size(); i++)
    {
        delete m_particles[i];
        m_particles[i] = nullptr;
    }
}

/**
 * @brief 
 * 
 */
void ParticlesEmitter::Init(const std::string& l_mesh, const std::string& l_texture)
{
    // Create this->amount default particle instances
    for (uint32_t i = 0; i < m_pAmount; ++i)
    {
        m_particles.push_back(new Particle(m_shader_ptr, m_position, l_mesh, l_texture));
        m_particles.back()->m_position = m_position;
    }
        
}

/**
 * @brief 
 * 
 * @param dt 
 * @param l_howManyNews 
 * @param l_offset 
 */
void ParticlesEmitter::Update(float dt, uint32_t l_howManyNews, const glm::vec3& l_offset)
{
    respawnTime += dt;

    for (uint32_t i = 0; i < l_howManyNews; ++i)
    {
        int unusedParticle = Find1stUnused();

        if(unusedParticle >= 0 && respawnTime >= 0.2f)
        {
            RespawnParticle(m_particles[unusedParticle], l_offset);
            respawnTime = 0.0f;
        }
    }
       
    for(uint32_t i = 0; i < m_particles.size(); ++i)
    {
        Particle &p = *m_particles[i];
        p.m_life -= dt;

        if(p.m_life > 0.0f)
        {
            p.m_velocity += glm::vec3(0.0f,-9.81f, 0.0f) * (float)dt * 0.5f;
            p.m_position += p.m_velocity * dt;
            p.m_color.a  -= dt;
            p.m_color.x  += 0.0025f;
            p.m_color.y  += 0.0025f;
            p.m_color.z  += 0.0025f;
        }
    }
}

/**
 * @brief 
 * 
 */
void ParticlesEmitter::Draw(float dt)
{
    for(uint32_t i = 0; i < m_particles.size(); i++)
    {
        if(m_particles[i]->m_life > 0.0f)
        {
            m_particles[i]->Draw(dt);   
        }
    }
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t ParticlesEmitter::Find1stUnused()
{
    // First search from last used particle, this will usually return almost instantly
    for (uint32_t i = m_lastUsedParticle; i < m_particles.size(); i++){
        if (m_particles[i]->m_life < 0.0f){
            m_lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (uint32_t i = 0; i < m_lastUsedParticle; i++){
        if (m_particles[i]->m_life < 0.0f){
            m_lastUsedParticle = i;
            return i;
        }
    }
    // All particles are taken.
    return -1;
}

/**
 * @brief 
 * 
 */
void ParticlesEmitter::RespawnParticle(Particle* l_particle, const glm::vec3& l_offset)
{
    float random = ((rand() % 100) - 80) / 10.0f;

    float r = 300.0f * sqrt((float) rand()/RAND_MAX);
    float theta = ((float) rand()/RAND_MAX) * 2 * 3.14159265359;
    l_particle->m_position = glm::vec3(m_position.x + (r * cos(theta)), m_position.y, m_position.z + (r * sin(theta)));

    l_particle->m_life     = abs(random);
    l_particle->m_scale    = glm::vec3(160.0f);
    l_particle->m_rotation = glm::vec3(0.0f, (float) rand()/RAND_MAX, 0.0f);
    l_particle->m_velocity = glm::vec3(0.0f, 600.0f * (float) rand()/RAND_MAX, 0.0f);
    l_particle->m_color    = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
