#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <glew.h>
#include <glm.hpp>

#include <TResource.hpp>

class TLight;

class TResourceShader : public TResource{

    public:
        TResourceShader();
        ~TResourceShader();
        //void CreateFromString(const char* vertexShader, const char* fragmentShader);
        void LoadResource(const std::string& c_resourceDocument_str);
        void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation);
        std::string ReadFile(const char* fileLocation);
        
        void UseShader();
        void ClearShader();
        
        GLuint AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
        void LinkProgram();
        //void AddShader(GLuint shaderProgram, const char* shaderCode, GLenum shaderType);

        void SetInt(const std::string& l_name_s, int l_value_n);
        void SetFloat(const std::string& l_name_s, float l_value_f);
        void SetVec2f(const std::string& l_name_s, const glm::vec2& l_vec2f);
        void SetVec2f(const std::string& l_name_s, float x, float y);
        void SetVec3f(const std::string& l_name_s, const glm::vec3& l_vec3f);
        void SetVec3f(const std::string& l_name_s, float x, float y, float z);
        void SetVec4f(const std::string &name, const glm::vec4& l_value);
        void SetVec4f(const std::string& l_name_s, float x, float y, float z, float w);
        void SetMat4f(const std::string& l_name_s, const glm::mat4& l_value_m4);
        void SetLightShaderValues(TLight* light, int lightIndex);

        GLuint GetProjectionLocation() { return uniformProjection; };
        GLuint GetModelLocation()      { return uniformModel; };
        GLuint GetShaderProgram()      { return shaderID; };
        GLuint GetViewLocation()       { return uniformView; };

    private:
        GLuint  shaderID           = 0, 
                vertexID           = 0, 
                fragmentID         = 0,
                geometryID         = 0, 
                uniformProjection  = 0, 
                uniformModel       = 0, 
                uniformView        = 0, 
                lightCount         = 0;
};