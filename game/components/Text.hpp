//
// Created by omar on 6/8/20.
//
#pragma once
#include <resource/Font.hpp>
#include <glad/glad.h>
#include <string>

namespace aik::component
{
    enum class Location {INSIDE, OUTSIDE}; //< Inside will render the text inside the transformable, outside will render it outside
    enum class Justification {LEFT, CENTER, RIGHT};
    enum class VJustification {TOP, CENTER, BOTTOM};

    class Text
    {
    public:
        aik::resource::Font* font = nullptr;
        Shader* shader = nullptr; // pointer to the shader that will be used to render this text
        std::string text;
        Location location = Location::INSIDE;
        Justification justification = Justification::CENTER;
        VJustification verticalJustification = VJustification::CENTER;
    };
}