#include <TEntity.hpp>
#include <glm.hpp>

class TResourceTexture;
class ResourceManager;
class TResourceShader;

class TSprite : public TEntity
{
    public:
        TSprite(const std::string& c_newTexture_string, int c_width, int c_height, int c_xPos, int c_yPos);
        ~TSprite(){};

        void BeginDraw();
        void EndDraw(){};
        bool GetActive()                 {  return true;  };
        void SetActive(bool c_status)    {  m_active = c_status;  };

        void SetTexture(const std::string& c_newTexture_string);
        void ScaleSprite(int c_width, int c_height);
        void CalculateModelMatrix();

    private:

        TResourceTexture* m_texture_ptr          = nullptr;
        TResourceShader*  m_shader_ptr           = nullptr;
        ResourceManager*  m_resourceManager_ptr  = nullptr;

        int m_height = 0, m_width = 0, m_xPos = 0, m_yPos = 0;
        unsigned int m_VAO_ui = 0;

        glm::mat4 m_modelMatrix_m4 = glm::mat4(1.0f);

        bool m_active;

};