//
// Created by omar on 5/15/20.
//
#pragma once
#include "Entity.hpp"
#include "Font.hpp"

namespace aik
{
class Text : public aik::Entity
    {
    public:
        void setFont(aik::Font * font);
        void setText(const std::string& text);
    private:
        aik::Font * font_;
        std::vector<glm::vec4> vertices_;
        std::vector<Character> characters_;
    };
}
