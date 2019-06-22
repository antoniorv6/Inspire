#include <TMesh.hpp>
#include <TResourceMesh.hpp>
#include <ResourceManager.hpp>
#include <TResourceTexture.hpp>
#include <TResourceShader.hpp>

/**
 * Construct a new TMesh::TMesh object
 * 
 */
TMesh::TMesh(MeshType l_type)
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indexCount = 0;
    m_resourceManager_ptr = ResourceManager::getInstance();
    
    if(l_type == MeshType::detailed)
        m_HasOutline = 1;

    m_material_st.ambient   = glm::vec3(1.0f, 1.0f, 1.0f);
    m_material_st.diffuse   = glm::vec3(1.0f, 1.0f, 1.0f);
    m_material_st.specular  = glm::vec3(0.0f, 0.0f, 0.0f);
    m_material_st.shininess = 0.4f * 128.0f;
    m_type = l_type;

    if(m_type == MeshType::standard)
    {
        m_shadersMap["base"] = (TResourceShader*)(m_resourceManager_ptr->GetShader("assets/shaders/shader"));
    }
    else
    {
        m_shadersMap["base"] = (TResourceShader*)(m_resourceManager_ptr->GetShader("assets/shaders/shader"));
    }
    
}

 /**
  * Destroy the TMesh::TMesh object
  * 
  */
TMesh::~TMesh(){

    ClearMesh();
}

/**
 * Get mesh resource from a file through a source url and save indices and vertices
 * 
 * @param c_source 
 */
void TMesh::LoadMesh(const std::string& c_source, const std::string& c_TextureSource)
{
    mesh = (TResourceMesh*)m_resourceManager_ptr->GetMesh(c_source);

    if(mesh->GetNumVertices() > 0)
    {
        m_bounding_v3 = mesh->GetMeshBounding();
        texture = (TResourceTexture*)m_resourceManager_ptr->GetTexture(c_TextureSource);
        this->CreateMesh(mesh->GetVertices(), mesh->GetIndices(), mesh->GetNumVertices(), mesh->GetNumIndices());
    }
}

/**
 * @brief 
 * 
 * @param c_TextureSource 
 */
void TMesh::SetTexture(const std::string& c_TextureSource)
{
   texture = (TResourceTexture*)m_resourceManager_ptr->GetTexture(c_TextureSource); 
}

/**
 * Create VAO, VBO and EBO and set its content (vertices and number of vertices)
 * 
 * @param vertices 
 * @param indices 
 * @param numOfVertices 
 * @param numOfIndices 
 */
void TMesh::CreateMesh(GLfloat* vertices, GLuint *indices, unsigned int numOfVertices, unsigned int numOfIndices){

    indexCount = numOfIndices;
    /** Create empty VAO (Vertex Attribute Object). */
    glGenVertexArrays(1, &VAO);
     /** Bind the Vertex Array Object previously created. */
    glBindVertexArray(VAO);
    /** Create empty VBO (Vertex Buffer Object). */
    glGenBuffers(1, &VBO);
    /** Create empty EBO (Element Buffer Object). */
    glGenBuffers(1, &EBO);
    /** Bind the Vertex Buffer Object previously created to the Buffer. */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /** Bind the Element Buffer Object, to make indexed draw. */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    /** [WARNING]: GL_STATIC_DRAW -> GL_STREAM_DRAW. */
    //Copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STREAM_DRAW);
    //Copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STREAM_DRAW);
    //This vertex attribute pointer is for the vertices handling
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //This vertex attribute pointer is for the texture coordinates
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, sizeof(float)*8, (void *)(sizeof(vertices[0])*3));
    glEnableVertexAttribArray(1);
    //This vertex attribute pointer is for the model normals
    glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE , sizeof(float)* 8, (void *)(sizeof(vertices[0])*5));
    glEnableVertexAttribArray(2);
    //Unbinding VAO & VBO & EBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * Clear buffers and vertex arrays
 * EBO, VBO, VAO
 * 
 */
void TMesh::ClearMesh()
{
    if(EBO != 0)
    {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }

    if(VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if(VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;

    m_resourceManager_ptr = nullptr;
    mesh                  = nullptr;
    texture               = nullptr;
    m_bounding_v3.clear();
}

/**
 * Draw mesh using data from VAO, VBO and EBO
 * 
 */
void TMesh::BeginDraw()
{
    glEnable(GL_DEPTH_CLAMP);
    if(isVisible())
    {
        auto& shader = *m_shadersMap["base"];
        shader.UseShader();

        shader.SetMat4f("model", m_model_mat4);
        shader.SetMat4f("MVP", (m_ViewProjection_mat4 * m_model_mat4));

        shader.SetVec3f("material.ambient", m_material_st.ambient);
        shader.SetVec3f("material.diffuse", m_material_st.diffuse);
        shader.SetVec3f("material.specular", m_material_st.specular);
        shader.SetFloat("material.shininess", m_material_st.shininess);


        shader.SetInt("HasOutline", m_HasOutline);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());

        //Bind buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //Draw indices as triangles in the scene (* 3 because each triangle has 3 vertices)
        glDrawElements(GL_TRIANGLES_ADJACENCY, indexCount, GL_UNSIGNED_INT, 0);
        //Unbind buffers
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

}

void TMesh::D()
{   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetID());

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Draw indices as triangles in the scene (* 3 because each triangle has 3 vertices)
    glDrawElements(GL_TRIANGLES_ADJACENCY, indexCount, GL_UNSIGNED_INT, 0);
    //Unbind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief 
 * 
 */
bool TMesh::isVisible()
{
    glm::mat4 MVP = TEntity::GetProjection() * TEntity::GetView() * TEntity::GetModel();
    std::vector<glm::vec3> bounding = m_bounding_v3;
    std::vector<glm::vec3> points_outside;
    points_outside.reserve(8);

    bool draw = false;
    int inside;

    glm::vec4 point;
    glm::vec3 planes;

    for(u_int32_t i = 0; i < bounding.size(); i++)
    {
        planes = {0,0,0};
        inside = 3;
        point = MVP * glm::vec4(bounding[i], 1.0f);
        bounding[i] = {point.x/point.w, point.y/point.w, point.z/point.w};

        for(u_int32_t j = 0; j < 3 ; j++)
        {
            if(bounding[i][j] > -1.0f && bounding[i][j] < 1.0f)
            {
                if ( --inside == 0 )
                {
                    draw = true;
                    goto draw; 
                }
            }
            else
            {
                     if( bounding[i][j] < -1.0f) {planes[j] = -1.0f;}
                else if( bounding[i][j] >  1.0f) {planes[j] =  1.0f;}
            }
        }

        points_outside.emplace_back(planes);
    }
    
    draw = true;

    for(u_int32_t j = 0; j < 3; j++)
    {
        for(u_int32_t i = 1; i < points_outside.size(); i++)
        {
            if(points_outside[i][j]!=0 && points_outside[i][j] == points_outside[i-1][j])
            {
                if(i == 7)
                {
                    draw = false;
                    goto draw; 
                }
                
            }
            else
            {
                break;
            }
            
        }
    }
 
    draw:

    return draw;
}

/**
 * @brief 
 * 
 */
void TMesh::EndDraw()
{
    
}

