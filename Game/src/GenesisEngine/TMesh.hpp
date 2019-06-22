#pragma once
#include <TEntity.hpp>
#include <vector>

class TResourceMesh;
class ResourceManager;
class TResourceTexture;
class TResourceShader;

struct MeshMaterial
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

enum MeshType
{
    standard = 0,
    detailed = 1
};

class TMesh: public TEntity 
{
    public:
        TMesh(MeshType l_type);
        ~TMesh();
        void LoadMesh(const std::string& c_Meshsource, const std::string& c_TextureSource = "grid.jpg");
        void CreateMesh(GLfloat* vertices, GLuint *indices, unsigned int numOfVertices, unsigned int numOfIndices);

        void SetTexture(const std::string& c_TextureSource);
        
        void ClearMesh();
        void D();
        bool isVisible();
        void BeginDraw();
        void EndDraw();
        void SetTexture(TResourceTexture * l_texture){ texture = l_texture; };
        bool GetActive(){ return false; };
        TResourceMesh* GetTRMesh() { return mesh; };

    private:
        MeshType m_type;

        GLuint VAO = 0, VBO = 0, EBO = 0;
        GLsizei indexCount = 0;

        ResourceManager  * m_resourceManager_ptr = nullptr;
        TResourceMesh    * mesh                  = nullptr;
        TResourceTexture * texture               = nullptr;
        MeshMaterial       m_material_st;

        std::map<std::string, TResourceShader*> m_shadersMap;
        std::vector<glm::vec3> m_bounding_v3;

        int m_HasOutline = 0;
};