#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <glew.h>
#include <TResource.hpp>
#include <vector>
#include <glm.hpp>

class aiMesh;

class TResourceMesh : public TResource {
    public:
        TResourceMesh();
        ~TResourceMesh();

        void LoadResource(const std::string& c_resourceDocument_str);
        void LoadWithAssimp(const std::string& c_resourceDocument_str);
        void LoadWithBinary(const std::string& c_resourceDocument_str);
        
        void ParseToBinary(const std::string& c_binaryPath_str);
        void CreateMeshBounding(aiMesh* l_mesh);

        int GetNumVertices() { return numVertices; };
        int GetNumIndices() { return numIndices; };
        std::vector<glm::vec3> GetMeshBounding() { return m_boundingVertex; };
        GLuint* GetIndices() { return indices; };
        GLfloat* GetVertices() { return vertices; };

        void CalculateAdjacencies(aiMesh*& l_mesh);

        int numVertices  = 0;
        int numIndices   = 0;
        GLuint *indices   = nullptr;
        GLfloat* vertices = nullptr;

        uint8_t loadedFromBinary = 0;

        std::vector<GLfloat> l_vertices;
        std::vector<GLuint>  l_indices;
        std::vector<glm::vec3> m_boundingVertex;
};