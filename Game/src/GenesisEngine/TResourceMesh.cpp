#include <TResourceMesh.hpp>
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <fstream>
#include <chrono>
#include <bitset>
#include <array>
#include <map>


TResourceMesh::TResourceMesh(){
    vertices = nullptr;
    indices = nullptr;
}

TResourceMesh::~TResourceMesh(){
    if(loadedFromBinary)
    {
        delete[] vertices;
        vertices = nullptr;
        delete[] indices;
        indices = nullptr;
    }
    else
    {
        l_vertices.clear();
        l_indices.clear();
    }
    
};

void TResourceMesh::LoadResource(const std::string& c_resourceDocument_str)
{
    //Search if there is a binary file for the asset
    std::string l_binaryroute = "assets/binaries/models/" + c_resourceDocument_str + ".bin";
    std::ifstream l_binaryStream(l_binaryroute);
    if(l_binaryStream.good())
    {
        //std::cout<<"File exists, loading from binary"<<std::endl;
       // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        LoadWithBinary(l_binaryroute);
        //std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        //std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
    }
    else
    {
        //std::cout<<"File does not exist, loading with Assimp"<<std::endl;
        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        LoadWithAssimp("assets/Models/" + c_resourceDocument_str + ".obj");
        //std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        //std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() <<std::endl;
        if(l_vertices.size() > 0)
        {
            ParseToBinary(l_binaryroute);
        }
    }

    l_binaryStream.close();
}

void TResourceMesh::LoadWithAssimp(const std::string& c_resourceDocument_str)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(c_resourceDocument_str, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if(scene)
    {
        aiMesh* l_mesh = scene->mMeshes[0];
        ////Reserve on memory the size of the vertex data to change its type from aiVec3 to GLFloat
        if(l_mesh->mNumVertices > 1)
        {
            CreateMeshBounding(l_mesh);
        }

        l_vertices.reserve(l_mesh->mNumVertices);
        l_indices.reserve(l_mesh->mNumFaces);

        for(size_t i = 0; i< l_mesh->mNumVertices; i++)
        {
            //Insert the vertices into the array
            l_vertices.insert(l_vertices.end(), {l_mesh->mVertices[i].x, l_mesh->mVertices[i].y, l_mesh->mVertices[i].z});
            //Check if there are texture coordinates to insert
            if(l_mesh->HasTextureCoords(0))
            {
                l_vertices.insert(l_vertices.end(), {l_mesh->mTextureCoords[0][i].x, l_mesh->mTextureCoords[0][i].y});
            }
            else
            {
                l_vertices.insert(l_vertices.end(), {0.0f, 0.0f});
            }
            
            l_vertices.insert(l_vertices.end(), {l_mesh->mNormals[i].x, l_mesh->mNormals[i].y, l_mesh->mNormals[i].z});
        }

        //for(size_t i=0; i< l_mesh->mNumFaces; i++)
        //{
        //    aiFace l_face = l_mesh->mFaces[i];
        //    for(size_t j = 0; j< l_face.mNumIndices ; j++)
        //    {
        //        l_indices.push_back(l_face.mIndices[j]);
        //    }
        //}
        CalculateAdjacencies(l_mesh);

        numVertices = l_vertices.size();
        numIndices  = l_indices.size();

        vertices    = &l_vertices[0];
        indices     = &l_indices[0];
    }else{
        std::cout << "Mesh not loaded" << std::endl;
    }
}

void TResourceMesh::CalculateAdjacencies(aiMesh*& l_mesh)
{
    using TVertex = std::array<float,3>;

    std::vector<int> l_uniquemap;
    std::map<TVertex, int> tempUniqueVertices;
    int l_uniqueIndex = 0;
    for(size_t vI = 0; vI < l_mesh->mNumVertices; ++vI)
    {
        auto vIt = tempUniqueVertices.find({l_mesh->mVertices[vI].x, l_mesh->mVertices[vI].y, l_mesh->mVertices[vI].z});
        if(vIt == tempUniqueVertices.end())
        {
            tempUniqueVertices[{l_mesh->mVertices[vI].x, l_mesh->mVertices[vI].y, l_mesh->mVertices[vI].z}] = l_uniqueIndex;
            l_uniquemap.push_back(l_uniqueIndex);
            l_uniqueIndex ++;
        }
        else
        {
            l_uniquemap.push_back(vIt->second);
        }
    }
    tempUniqueVertices.clear();

    std::map<std::tuple<int,int>, std::vector<int>> edges;
    for(size_t i = 0; i< l_mesh->mNumFaces; i++)
    {
        aiFace l_face = l_mesh->mFaces[i];
        for(int pI = 0; pI < 3; ++pI)
        {
            int edgeU[2]{l_uniquemap[l_face.mIndices[pI]], l_uniquemap[l_face.mIndices[(pI+1)%3]]};
            int i0 = edgeU[0] < edgeU[1] ? 0:1;
            edges[{edgeU[i0], edgeU[1-i0]}].push_back(l_mesh->mFaces[i].mIndices[(pI+2)%3]);
        }
    }

    for(size_t i = 0; i< l_mesh->mNumFaces; i++)
    {
        aiFace l_face = l_mesh->mFaces[i];
        for(int pI = 0; pI < 3; ++pI)
        {
            int edgeU[2]{l_uniquemap[l_face.mIndices[pI]], l_uniquemap[l_face.mIndices[(pI+1)%3]]};
            int i0 = edgeU[0] < edgeU[1] ? 0 : 1;
            auto& adjs = edges[{edgeU[i0], edgeU[1 - i0]}];
            int adjI = adjs.size() > 1 && adjs[0] == l_mesh->mFaces[i].mIndices[(pI + 2)%3] ? 1 : 0;
            l_indices.push_back(l_mesh->mFaces[i].mIndices[pI]);
            l_indices.push_back(adjs[adjI]);
        }
    }
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 */
void TResourceMesh::LoadWithBinary(const std::string& c_resourceDocument_str)
{
    std::ifstream readBinary(c_resourceDocument_str, std::ios::binary | std::ios::in);
    
    readBinary.read(reinterpret_cast<char*>(&numVertices), sizeof(int));

    vertices = new GLfloat [numVertices];

    readBinary.read(reinterpret_cast<char*>(vertices), sizeof(float)*numVertices);

    readBinary.read(reinterpret_cast<char*>(&numIndices), sizeof(int));

    indices = new GLuint [numIndices];

    readBinary.read(reinterpret_cast<char*>(indices), sizeof(GLuint)*numIndices);

    int sizeofbounding = 0;
    readBinary.read(reinterpret_cast<char*>(&sizeofbounding), sizeof(int));

    for(int i = 0; i < sizeofbounding; i++)
    {
        glm::vec3 vecToPush;
        readBinary.read(reinterpret_cast<char*>(&vecToPush), sizeof(glm::vec3));
        m_boundingVertex.push_back(vecToPush);
    }  

    loadedFromBinary = 1;  

    readBinary.close();
}

/**
 * @brief 
 * 
 * @param c_binaryPath_str 
 */
void TResourceMesh::ParseToBinary(const std::string& c_binaryPath_str)
{
    //std::cout<<"Writing binary file..."<<std::endl;
    
    std::ofstream l_binaryFile(c_binaryPath_str, std::ios::binary | std::ios::out);
    
    l_binaryFile.write(reinterpret_cast<const char*>(&numVertices), sizeof(int));

    for(int i = 0; i< numVertices; i++)
        l_binaryFile.write(reinterpret_cast<const char*>(&l_vertices[i]), sizeof(float));
    
    l_binaryFile.write(reinterpret_cast<const char*>(&numIndices), sizeof(int));

    for(int i = 0; i< numIndices; i++)
        l_binaryFile.write(reinterpret_cast<const char*>(&l_indices[i]), sizeof(unsigned int));

    int sizeofbounding = m_boundingVertex.size();
    l_binaryFile.write(reinterpret_cast<const char*>(&sizeofbounding), sizeof(int));

    for(int i=0; i< sizeofbounding ; i++)
        l_binaryFile.write(reinterpret_cast<const char*>(&m_boundingVertex[i]), sizeof(glm::vec3));
    
    l_binaryFile.close();
    
    //std::cout<<"Writtern"<<std::endl;
}

/**
 * @brief 
 * 
 * @param l_mesh 
 */
void TResourceMesh::CreateMeshBounding(aiMesh* l_mesh)
{
    aiVector3D max = l_mesh->mVertices[0];
    aiVector3D min = l_mesh->mVertices[0];

    for (unsigned int i = 1; i < l_mesh->mNumVertices; i++) 
    {
        max.x = max.x > l_mesh->mVertices[i].x ? max.x : l_mesh->mVertices[i].x;
        max.y = max.y > l_mesh->mVertices[i].y ? max.y : l_mesh->mVertices[i].y;
        max.z = max.z > l_mesh->mVertices[i].z ? max.z : l_mesh->mVertices[i].z;

        min.x = min.x < l_mesh->mVertices[i].x ? min.x : l_mesh->mVertices[i].x;
        min.y = min.y < l_mesh->mVertices[i].y ? min.y : l_mesh->mVertices[i].y;
        min.z = min.z < l_mesh->mVertices[i].z ? min.z : l_mesh->mVertices[i].z;
    }

    /* COMPLETE ARRAY OF 8 VERTEX */
    for(unsigned int i=0; i<8; i++)
    {
        int aux[3];
        std::string binary = std::bitset<3>(i).to_string(); //to binary
        glm::vec3 vertex;

        for(int j = 2; j > -1; j--)
        {
            aux[j] = (int) binary[j] - 48;
        }

        for(int i = 0; i < 3 ; i++)
        {
            if(aux[i] == 1)
            {
                vertex[i] = max[i];
            }
            else
            {
                vertex[i] = min[i];
            }
        }

        m_boundingVertex.push_back(vertex);
    }

}

