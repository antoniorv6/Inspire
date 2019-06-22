/**
 * @file Particle.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-05-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Particle.hpp>
#include <TMesh.hpp>
#include <TResourceShader.hpp>
#include <TEntity.hpp>

/**
 * @brief Construct a new Particle:: Particle object
 * 
 */
Particle::Particle
(
    TResourceShader * l_shader,
    const glm::vec3& l_initialPosition, 
    const std::string& l_mesh, 
    const std::string& l_texture
)
{
    m_shader = l_shader;

    m_particleMesh = new TMesh(MeshType::standard);
    m_particleMesh->LoadMesh(l_mesh);
    m_particleMesh->SetTexture(l_texture);

    m_initialPosition = l_initialPosition;
    m_position        = l_initialPosition;
    m_scale           = glm::vec3(100.0f);
    m_rotation        = glm::vec3(0.0f);
    m_life            = 1.0f; 
    m_velocity        = glm::vec3(0.0f, 0.0f, 0.0f);
    m_color           = glm::vec4(1.0f);
    m_modelMatrix     = glm::mat4(1.0f);

    CalculateMatrix();
}

/**
 * @brief Destroy the Particle:: Particle object
 * 
 */
Particle::~Particle()
{
    delete m_particleMesh;
    m_particleMesh = nullptr;
}

/**
 * @brief 
 * 
 */
void Particle::CalculateMatrix()
{
    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::translate(m_modelMatrix, m_position);
    m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
    m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));
}


void Particle::Draw(float dt)
{
    CalculateMatrix();
    m_shader->UseShader();
    m_shader->SetMat4f("projection", TEntity::GetProjection());
    m_shader->SetMat4f("view", TEntity::GetView());
    m_shader->SetMat4f("model", m_modelMatrix);
    m_shader->SetVec4f("_color", m_color);
    m_particleMesh->D();
}

