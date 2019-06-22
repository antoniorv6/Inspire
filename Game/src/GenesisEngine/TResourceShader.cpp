#include <TResourceShader.hpp>
#include <TLight.hpp>
#include <vector>

/**
 * @brief Construct a new TResourceShader::TResourceShader object
 * 
 */
TResourceShader::TResourceShader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

/**
 * @brief Destroy the TResourceShader::TResourceShader object
 * 
 */
TResourceShader::~TResourceShader()
{
    ClearShader();
}

//void TResourceShader::CreateFromString(const char* vertexShader, const char* fragmentShader){
//
//    CompileShader(vertexShader, fragmentShader);
//}

void TResourceShader::LoadResource(const std::string& c_resourceDocument_str)
{
    /*
    
        This proccessing is not that simple as in Meshes and Textures
        here it is given the "folder/shadersName" -> we have to create two strings with the .vert and .frag extensions in order
        to achieve our main goal, have the vertex and the fragment shader loaded
    
    */

   std::string l_vertexShader = c_resourceDocument_str + ".vert";
   std::string l_fragmentShader = c_resourceDocument_str + ".frag";
   std::string l_geometryShader = c_resourceDocument_str + ".gs";

   CreateFromFiles(l_vertexShader.c_str(), l_fragmentShader.c_str(), l_geometryShader.c_str());
}

/**
 * @brief 
 * 
 * @param vertexLocation 
 * @param fragmentLocation 
 */
void TResourceShader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation)
{
    std::string vertexString   = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    std::string geometryString = ReadFile(geometryLocation);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    const char* geometryCode = geometryString.c_str();

    shaderID = glCreateProgram();

    if (!shaderID)
    {
        std::cout << "[ERROR] Could not create OpenGL program" << std::endl;
        return;
    }

    if(vertexCode[0])
        vertexID   =  AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    
    if(geometryCode[0])
        geometryID =  AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
    
    if(fragmentCode[0])
        fragmentID =  AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
    
    LinkProgram();

    GLint result = GL_FALSE;
    int InfoLogLenght;

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLenght);
    if(InfoLogLenght > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLenght +1);
        glGetProgramInfoLog(shaderID, InfoLogLenght, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

}

/**
 * @brief 
 * 
 * @param fileLocation 
 * @return std::string 
 */
std::string TResourceShader::ReadFile(const char* fileLocation){
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open()){
        //std::cout << "Failed to read " << fileLocation << " File doesn't exist" << std::endl;
        return "\0";
    }

    std::string line = "";
    while(!fileStream.eof()){
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    
    return content.c_str();
}

void TResourceShader::LinkProgram()
{
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: '%s" << eLog << std::endl;
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: " <<  eLog << std::endl;
        return;
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformView = glGetUniformLocation(shaderID, "view");
}

/**
 * @brief 
 * 
 * @param theProgram 
 * @param shaderCode 
 * @param shaderType 
 * @return GLuint 
 */
GLuint TResourceShader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the %d shader: " << shaderType << eLog << std::endl;
        return -1;
    }
    else
    {
        //std::cout << "[SUCCESS]: "<< eLog << "Shader compiled correctly.\n"; 
    }

    glAttachShader(theProgram, theShader);


    return theShader;
}

/**
 * @brief 
 * 
 */
void TResourceShader::UseShader()
{
    glUseProgram(shaderID);
}

/**
 * @brief 
 * 
 */
void TResourceShader::ClearShader()
{
    if(shaderID != 0)
    {
        glDetachShader(shaderID, vertexID);
        glDetachShader(shaderID, fragmentID);
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformProjection = 0;
    uniformModel = 0;
}

/**
 * @brief Set the Int object
 * 
 * @param l_name_s 
 * @param l_value_n 
 */
void TResourceShader::SetInt(const std::string& l_name_s, int l_value_n)
{
    glUniform1i(glGetUniformLocation(shaderID, l_name_s.c_str()), l_value_n);
}

/**
 * @brief 
 * 
 * @param l_name_s 
 * @param l_value_n 
 */
void TResourceShader::SetFloat(const std::string& l_name_s, float l_value_n)
{
    glUniform1f(glGetUniformLocation(shaderID, l_name_s.c_str()), l_value_n);
}

/**
 * @brief 
 * 
 * @param l_name_s 
 * @param l_value_n 
 */
void TResourceShader::SetVec2f(const std::string& l_name_s, float x, float y)
{
    glUniform2f(glGetUniformLocation(shaderID, l_name_s.c_str()), x, y);
}

/**
 * @brief 
 * 
 * @param l_name_s 
 * @param l_vec2f 
 */
void TResourceShader::SetVec2f(const std::string& l_name_s, const glm::vec2& l_vec2f)
{
    glUniform2fv(glGetUniformLocation(shaderID, l_name_s.c_str()), 1, &l_vec2f[0]);
}

/**
 * @brief 
 * 
 * @param l_name_s 
 * @param l_value_n 
 */
void TResourceShader::SetVec3f(const std::string& l_name_s, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(shaderID, l_name_s.c_str()), x, y, z);
}

/**
 * @brief 
 * 
 * @param l_name_s SetVec3f( void
 * @param l_vec3f 
 */
void TResourceShader::SetVec3f(const std::string& l_name_s, const glm::vec3& l_vec3f)
{
    glUniform3fv(glGetUniformLocation(shaderID, l_name_s.c_str()), 1, &l_vec3f[0]);
}

/**
 * @brief 
 * 
 * @param name 
 * @param value 
 */
void TResourceShader::SetVec4f(const std::string &name, const glm::vec4 &value)
{ 
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]); 
}

/**
 * @brief 
 * 
 * @param name 
 * @param x 
 * @param y 
 * @param z 
 * @param w 
 */
void TResourceShader::SetVec4f(const std::string &name, float x, float y, float z, float w) 
{ 
    glUniform4f(glGetUniformLocation(shaderID, name.c_str()), x, y, z, w); 
}

/**
 * @brief 
 * 
 * @param l_name_s 
 * @param l_value_m4 
 */
void TResourceShader::SetMat4f(const std::string& l_name_s, const glm::mat4& l_value_m4)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID,  l_name_s.c_str()), 1, GL_FALSE,  glm::value_ptr(l_value_m4));
}

/**
 * @brief 
 * 
 * @param light 
 * @param lightIndex 
 */
void TResourceShader::SetLightShaderValues(TLight* light, int lightIndex)
{
    char bufferToShader[100] = { '\0'  };

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].lightPos", lightIndex);
    unsigned int l_uniformPosition = glGetUniformLocation(shaderID, bufferToShader); 

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].ambient", lightIndex);
    unsigned int l_uniformAmbient = glGetUniformLocation(shaderID, bufferToShader); 

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].diffuse", lightIndex);
    unsigned int l_uniformDiffuse = glGetUniformLocation(shaderID, bufferToShader); 

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].specular", lightIndex);
    unsigned int l_uniformSpecular = glGetUniformLocation(shaderID, bufferToShader);

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].constant", lightIndex);
    unsigned int l_uniformConstant = glGetUniformLocation(shaderID, bufferToShader);

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].linear", lightIndex);
    unsigned int l_uniformLinear = glGetUniformLocation(shaderID, bufferToShader);

    snprintf(bufferToShader, sizeof(bufferToShader), "phongLights[%d].quadratic", lightIndex);
    unsigned int l_uniformQuadratic = glGetUniformLocation(shaderID, bufferToShader);

    light->SetUniformValues(
                                l_uniformPosition, 
                                l_uniformAmbient, 
                                l_uniformDiffuse, 
                                l_uniformSpecular, 
                                l_uniformConstant, 
                                l_uniformLinear,
                                l_uniformQuadratic
                            );
}