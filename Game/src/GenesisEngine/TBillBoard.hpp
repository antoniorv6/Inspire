#include <TEntity.hpp>

class TResourceTexture;
class TResourceShader;
class ResourceManager;

class TBillBoard : public TEntity
{
    public:
        
        TBillBoard(float l_width_f, float l_height_f);
        
        ~TBillBoard(){};

        void BeginDraw();
        void EndDraw(){};

        bool GetActive(){return true;};

        void SetTexture(const std::string& c_newTexture_string);
        void SetPosition(glm::vec3 c_newPosition_v3);
    
    private:
        GLuint m_vertexBuffer_uint = 0, m_VAO_uint = 0;
        float m_width_f = 0.0f, m_height_f = 0.0f;

        TResourceTexture* m_texture             = nullptr;
        TResourceShader*  m_shader_ptr          = nullptr;
        ResourceManager*  m_resourceManager_ptr = nullptr;

        glm::vec3 m_position_v3 = glm::vec3(0.0f);
};