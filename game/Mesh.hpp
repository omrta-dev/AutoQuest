//
// Created by omar on 2/20/20.
//

#pragma once

#include <iostream>
#include <vector>
#include "glpp/Vertex.hpp"
#include "glpp/Texture.hpp"

/*
 * Mesh class which will be the base class for any drawable openGL entity
 * Contains transformation functions, setting vertices and forming the model matrix
 */

namespace aik
{
    class Mesh
    {
    public:
        Mesh(std::vector<aik::Vertex> vertices, std::vector<unsigned int> indices, std::vector<aik::Texture> textures);
        inline const std::vector<aik::Vertex>& getVertices() {return vertices_;}
        inline const std::vector<unsigned int>& getIndices() {return indices_;};
        inline const std::vector<aik::Texture>& getTextures() {return textures_;};
    private:
        std::vector<aik::Vertex> vertices_;
        std::vector<unsigned int> indices_;
        std::vector<aik::Texture> textures_;
    };
}
