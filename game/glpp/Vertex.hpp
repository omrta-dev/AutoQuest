//
// Created by omar on 2/23/20.
//

#pragma once
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

namespace aik
{
    struct Vertex
    {
        inline Vertex() {}
        inline Vertex(glm::vec3 pos, glm::vec4 col) : position(pos), color(col) {}
        glm::vec3 position;
        glm::vec4 color;
    };
}
