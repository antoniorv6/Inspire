#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include <TResourceFont.hpp>


void TResourceFont::LoadResource(const std::string& c_resourceDocument_str)
{
    FT_Library l_ft;
    if(FT_Init_FreeType(&l_ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library"<<std::endl;
        std::cin.get();
    }

    FT_Face l_face;
    //"assets/fonts/SouthernAire_Personal_Use_Only.ttf"
    if(FT_New_Face(l_ft, c_resourceDocument_str.c_str(), 0, &l_face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font"<<std::endl;
        std::cin.get();
    }

    FT_Set_Pixel_Sizes(l_face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLubyte c = 0; c<128; c++)
    {
        if(FT_Load_Char(l_face, c, FT_LOAD_RENDER))
        {
            std::cout<< "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint l_textureID;
        glGenTextures(1, &l_textureID);
        glBindTexture(GL_TEXTURE_2D, l_textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, l_face->glyph->bitmap.width, l_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, l_face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        TCharacter l_character = {
            l_textureID, glm::ivec2(l_face->glyph->bitmap.width, l_face->glyph->bitmap.rows),
            glm::ivec2(l_face->glyph->bitmap_left, l_face->glyph->bitmap_top), l_face->glyph->advance.x
        };

        m_fontCharacters.insert(std::pair<GLchar, TCharacter>(c, l_character));
    }

    FT_Done_Face(l_face);
    FT_Done_FreeType(l_ft);
}