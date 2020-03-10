//
// Created by omar on 2/23/20.
//

#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace aik
{
    // Contains the information for each Vertex in a model
    struct Vertex
    {
        Vertex() = default;
        inline Vertex(glm::vec3 pos, glm::vec2 uvCoord, glm::vec3 norm) : position(pos), uv(uvCoord), normal(norm) {}
        glm::vec3 position{};
        glm::vec2 uv{};
        glm::vec3 normal{};
    };
}
