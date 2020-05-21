//
// Created by omar on 5/15/20.
//

#include "Text.hpp"
#include <entt/entt.hpp>

void aik::Text::setFont(aik::Font *font)
{
    font_ = font;
}

void aik::Text::setText(const std::string &text)
{
    float x = position_.x;
    float y = position_.y;

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = font_->characters_[*c];
        characters_.push_back(ch);

        float xpos = x + ch.Bearing.x * scale_.x;
        float ypos = y - static_cast<float>(ch.Size.y - ch.Bearing.y) * scale_.y;

        float w = ch.Size.x * scale_.x;
        float h = ch.Size.y * scale_.y;
        // update VBO for each character
        vertices_ = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        x += (ch.Advance >> 6) * scale_.x; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
}
