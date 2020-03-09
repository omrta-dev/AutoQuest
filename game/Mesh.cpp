//
// Created by omar on 2/20/20.
//

#include "Mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>

aik::Mesh::Mesh(std::vector<aik::Vertex> vertices, std::vector<unsigned int> indices,
                std::vector<aik::Texture> textures)
{
    vertices_ = std::move(vertices);
    indices_ = std::move(indices);
    textures_ = std::move(textures);
}
