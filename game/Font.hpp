//
// Created by omar on 5/15/20.
//
#pragma once
#include <string>
#include <map>
#include <glm/vec2.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace aik
{
    struct Character {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    class Font
    {
    public:
        bool loadLibrary(const std::string& fontLocation);
        std::map<char, Character> characters_;
    private:
        bool loadChars();
        FT_Face face_;
        FT_Library ft_;
    };
}
