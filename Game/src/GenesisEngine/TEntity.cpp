#include <TEntity.hpp>

std::stack<glm::mat4> TEntity::m_transform_stack = std::stack<glm::mat4>();
glm::mat4 TEntity::m_model_mat4  = glm::mat4(1.0);
glm::mat4 TEntity::m_view_mat4 = glm::mat4(1.0);
glm::mat4 TEntity::m_projection_mat4 = glm::mat4(1.0);
glm::mat4 TEntity::m_ViewProjection_mat4 = glm::mat4(1.0);

int TEntity::modelLocation = 0;
int TEntity::modelViewProjectionLocation = 0;   