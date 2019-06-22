#pragma once
#include <vector>
#include <glm.hpp>

enum NodeType
{
    Mesh = 0,
    AnimatedMesh = 1,
    Camera = 2, 
    Light = 3,
    Billboard = 4, 
    SBox = 5,
    Sprite = 6,
    Transform = 7,
    LODMesh = 8,
    Root = 9
};

class TEntity;

class TNode{
    public:
        TNode(u_int32_t l_type);
        ~TNode();
        int AddChild(TNode* node);
        int RemoveChild(TNode* node);
        void Draw();
        void ClearList();

        bool SetEntity(TEntity* entity_) { entity = entity_; return true;};
        void SetFather(TNode* node) { father = node;};
        void SetCurrentPosition(const glm::vec3& l_position_v3) { m_currentPosition_v3 = l_position_v3; };
        void SetCurrentRotation(const glm::vec3& l_rotation_v3) { m_currentRotation_v3 = l_rotation_v3; };
        void SetCurrentScale(const glm::vec3& l_scale_v3)       { m_currentScale_v3 = l_scale_v3; };

        TNode* getFather()             { return father; };
        TEntity* GetEntity()           { return entity; };
        u_int32_t GetType()            { return m_type; };
        glm::vec3& GetPositionRef()    { return m_currentPosition_v3; };
        glm::vec3 GetCurrentPosition() { return m_currentPosition_v3; };
        glm::vec3 GetCurrentRotation() { return m_currentRotation_v3; };
        glm::vec3 GetCurrentScale()    { return m_currentScale_v3; };
        std::vector<TNode*> GetNodeChildren() { return child; };

    private:
        uint32_t m_type = 0;

        TNode   * father = nullptr;
        TEntity * entity = nullptr;

        glm::vec3 m_currentPosition_v3 = glm::vec3(0.0f);
        glm::vec3 m_currentRotation_v3 = glm::vec3(0.0f);
        glm::vec3 m_currentScale_v3    = glm::vec3(0.0f);
        std::vector<TNode*> child;
};

