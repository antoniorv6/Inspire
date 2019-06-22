#include <TTransform.hpp>

/**
 * @brief 
 * 
 */
void TTransform::Identity(){
    matrix = glm::mat4(1.0);
}

/**
 * @brief 
 * 
 */
void TTransform::Inverse(){
    matrix = glm::inverse(matrix);
}

/**
 * @brief 
 * 
 */
void TTransform::Transpose(){
    matrix = glm::transpose(matrix);
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param z 
 */
void TTransform::Translate(const glm::vec3& l_translation_v3)
{
    matrix = glm::translate(matrix, l_translation_v3);
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param z 
 * @param w 
 */
void TTransform::Rotate(const glm::vec3& axis, float angle)
{
    matrix = glm::rotate(matrix, angle, axis);
}

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param z 
 */
void TTransform::Scale(const glm::vec3& l_scale_v3)
{
    matrix = glm::scale(matrix, l_scale_v3);
}

/**
 * @brief 
 * 
 */
void TTransform::BeginDraw()
{
    glm::mat4 &model = TEntity::GetModel();
    // Push current model matrix
    TEntity::GetStack().push(model);

    if(matrix != glm::mat4(1.0f))
    {
        // Multiply transformation matrix and current model matrix
        model = model * matrix;
    }
}

/**
 * @brief 
 * 
 */
void TTransform::EndDraw()
{  
    glm::mat4& model = TEntity::GetModel();

    model = TEntity::m_transform_stack.top();

    TEntity::m_transform_stack.pop();
}