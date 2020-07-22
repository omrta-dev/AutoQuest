//
// Created by omar on 6/8/20.
//
#include "Font.hpp"
#include <iostream>
#include <ft2build.h>
#include <algorithm>
#include FT_FREETYPE_H

void aik::resource::Font::test()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/free_serif.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    std::cout << "Glyph Info:\n";
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        std::cout << "Glyph: " << static_cast<int>(c) << " : " << c;
        std::cout << " Size: (" << face->glyph->bitmap.width << "," << face->glyph->bitmap.rows << ")\n";
    }
}
