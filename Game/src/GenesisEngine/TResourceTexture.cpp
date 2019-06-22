#include <TResourceTexture.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <glew.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

/**
 * @brief Construct a new TResourceTexture::TResourceTexture object
 * 
 */
TResourceTexture::TResourceTexture()
{
    m_textureID = 0;
    m_width_i = 0;
    m_height_i = 0;
    m_bDepth_i = 0;
}

/**
 * @brief Destroy the TResourceTexture::TResourceTexture object
 * 
 */
TResourceTexture::~TResourceTexture()
{
    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
    m_width_i = 0;
    m_height_i = 0;
    m_bDepth_i = 0;
}

/**
 * @brief 
 * 
 * @param c_resourceDocument_str 
 */
void TResourceTexture::LoadResource(const std::string& c_resourceDocument_str)
{
    std::string l_assetRoute = "assets/Textures/" + c_resourceDocument_str;
    LoadFromSTBI(l_assetRoute);
    BindTexture();
    stbi_image_free(m_imageData_cch);

    //We get the DOCUMENT name
    //std::string l_documentName = c_resourceDocument_str.substr(0, c_resourceDocument_str.find("."));
    //std::string l_binaryroute = "assets/binaries/textures/" + l_documentName + ".bin";
    //std::string l_assetRoute = "assets/" + c_resourceDocument_str;
    //std::ifstream l_binaryStream(l_binaryroute);
    //if(l_binaryStream.good())
    //{
    //    //std::cout<<"File exists, loading from binary"<<std::endl;
    //    LoadFromBinary(l_binaryroute);
    //    BindTexture();
    //    //We delete the unsigned char
    //    delete[] m_imageData_cch;
    //}
    //else
    //{
    //    //std::cout<<"File does not exist, loading with STBI"<<std::endl;
    //    LoadFromSTBI(l_assetRoute);
    //    if(m_imageData_cch)
    //    {
    //        ParseToBinary(l_binaryroute);
    //        BindTexture();
    //    }
    //    stbi_image_free(m_imageData_cch);
    //}

    //l_binaryStream.close();
}

void TResourceTexture::LoadFromSTBI(const std::string& c_resourceDocument_str)
{
    m_imageData_cch = stbi_load(c_resourceDocument_str.c_str(), &m_width_i, &m_height_i, &m_bDepth_i, 4);
    if(!m_imageData_cch)
    {
        //std::cout << "[ERROR] - Texture could not be found"<<std::endl;
        stbi_image_free(m_imageData_cch);
        return;
    }
}

void TResourceTexture::LoadFromBinary(const std::string& c_resourceDocument_str)
{
    std::ifstream readBinary(c_resourceDocument_str, std::ios::binary | std::ios::in);
    
    readBinary.read(reinterpret_cast<char*>(&m_width_i), sizeof(int));
    readBinary.read(reinterpret_cast<char*>(&m_height_i), sizeof(int));
    readBinary.read(reinterpret_cast<char*>(&m_bDepth_i), sizeof(int));

    int l_dataSize = 0;
    readBinary.read(reinterpret_cast<char*>(&l_dataSize), sizeof(int));  
    
    m_imageData_cch = new unsigned char [l_dataSize];

    readBinary.read(reinterpret_cast<char*>(m_imageData_cch), l_dataSize);  

    m_imageData_cch[l_dataSize] = '\0';

    readBinary.close();
}

void TResourceTexture::ParseToBinary(const std::string& c_binaryPath_str)
{
    //std::cout<<"Writing binary file..."<<std::endl;
    
    std::ofstream l_binaryFile(c_binaryPath_str, std::ios::binary | std::ios::out);

    l_binaryFile.write(reinterpret_cast<const char*>(&m_width_i), sizeof(int));
    l_binaryFile.write(reinterpret_cast<const char*>(&m_height_i), sizeof(int));
    l_binaryFile.write(reinterpret_cast<const char*>(&m_bDepth_i), sizeof(int));

    int l_dataSize = 0;
    l_dataSize = strlen((const char*)m_imageData_cch);
    
    l_binaryFile.write(reinterpret_cast<const char*>(&l_dataSize), sizeof(int));
    l_binaryFile.write(reinterpret_cast<const char*>(m_imageData_cch), l_dataSize);

    l_binaryFile.close();

    //std::cout<<"Written!"<<std::endl;
}

void TResourceTexture::BindTexture()
{
    glGenTextures(1, &m_textureID);

    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)m_width_i, (GLsizei)m_height_i, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageData_cch);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

 
