#include <TNode.hpp>
#include <TEntity.hpp>

/**
 * @brief Construct a new TNode::TNode object
 * 
 */
TNode::TNode(uint32_t l_type) : m_type(l_type)
{
    father = nullptr;
    entity = nullptr;  

    m_currentPosition_v3 = glm::vec3(0,0,0); 
    m_currentRotation_v3 = glm::vec3(0,0,0); 
    m_currentScale_v3    = glm::vec3(0,0,0); 
}

/**
 * @brief Destroy the TNode::TNode object
 * 
 */
TNode::~TNode()
{
    ClearList();
    child.clear();
    if(entity)
    {
        delete entity;
        entity = nullptr;
    }
}

/**
 * @brief 
 * 
 * @param node 
 * @return int 
 */
int TNode::AddChild(TNode* node)
{
    child.emplace_back(node);
    node->SetFather(this);
    return 0;
}

/**
 * @brief 
 * 
 * @param node 
 * @return int 
 */
int TNode::RemoveChild(TNode* node)
{
    for(unsigned int i=0; i < child.size(); i++)
    {
        if(node == child[i]){
            child.erase(child.begin()+i);
            return 0;
        }
    }
    return 1;
}

/**
 * @brief 
 * 
 */
void TNode::Draw()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if(entity!=nullptr)
    {
        entity->BeginDraw();
    }
    
    for(unsigned int i = 0; i < this->child.size(); i++)
    {
        child[i]->Draw();
    }

    if(this->entity!=nullptr)
    {
        entity->EndDraw();
    }

    glDisable(GL_CULL_FACE);
}

/**
 * @brief 
 * 
 */
void TNode::ClearList()
{
    for(unsigned int i = 0; i < child.size(); i++)
     {
        delete child[i];
        child[i] = nullptr;
     }
    
    child.clear();
}

