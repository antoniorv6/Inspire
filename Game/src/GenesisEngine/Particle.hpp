/**
 * @file Particle.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include <glm.hpp>
#include <string>

class TMesh;
class TResourceShader;

class Particle
{
    friend class ParticlesEmitter;

    public:
        Particle
        (
            TResourceShader * l_shader, 
            const glm::vec3& l_initialPosition, 
            const std::string& l_mesh, 
            const std::string& l_texture
        );
        ~Particle();

        void CalculateMatrix();
        void Draw(float dt);


    private:
        TMesh * m_particleMesh     = nullptr;
        TResourceShader * m_shader = nullptr;

        float m_life;                
        glm::vec3 m_initialPosition;
        glm::vec3 m_position;        
        glm::vec3 m_velocity;      
        glm::vec3 m_scale;
        glm::vec3 m_rotation;  
        glm::vec4 m_color;           
        glm::mat4 m_modelMatrix; 

};