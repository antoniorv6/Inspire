#include <TEntity.hpp>
#include <glm.hpp>

class TResourceFont;
class TResourceShader;
class ResourceManager;

class TText : public TEntity
{
    public:
        TText(float c_xPos, float c_yPos, float c_Scale, const std::string& c_textToRender, const std::string& c_fontLocation);
        ~TText(){};

        
        void BeginDraw();
        void EndDraw(){};
        bool GetActive(){return true;}
    private:
        GLuint m_VAO = 0, m_VBO = 0;
        float m_xPos = 0, m_yPos = 0, m_scale = 0;

        TResourceShader* m_shader = nullptr;
        TResourceFont*   m_font   = nullptr;

        glm::vec3 m_color = glm::vec3(0.0f);
        std::string m_textToRender = "";
};