#include <TResource.hpp>
#include <glew.h>
#include <glm.hpp>
#include <map>

struct TCharacter
{
    GLuint      m_TextureID;
    glm::ivec2  m_Size;
    glm::ivec2  m_Bearing;
    GLuint      m_advance;
};

class TResourceFont: public TResource
{
    public:
        TResourceFont(){};
        ~TResourceFont(){};
        virtual void LoadResource(const std::string& c_resourceDocument_str);
        TCharacter ReturnCharacter(GLchar c_index){ return m_fontCharacters[c_index];}
    
    private:
        std::map<GLchar, TCharacter> m_fontCharacters;
};