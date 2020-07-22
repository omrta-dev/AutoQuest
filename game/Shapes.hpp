//
// Created by omar on 5/22/20.
//
#pragma once
#include <vector>
#include <glm/vec3.hpp>

namespace aik::Shape
{
    struct Square
    {
        static inline std::vector<glm::vec3> getVertices() { return {{1.f, 1.f, 1.0f}, // top right
                                                                     {1.f, 0.f, 1.0f}, // bottom right
                                                                     {0.f, 0.f, 1.0f}, // bottom left
                                                                     {0.f, 1.f, 1.0f}}; } // top left
        static inline std::vector<unsigned int> getIndices() { return {0, 1, 3, 1, 2, 3}; }
    };
}