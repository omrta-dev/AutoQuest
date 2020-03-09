//
// Created by omar on 3/6/20.
//

#include "Texture.hpp"
#include <glad/glad.h>
#include <SFML/Graphics/Image.hpp>

#include <iostream>

aik::Texture aik::Texture::LoadTexture(const std::string& directory, const std::string& file, const std::string& type)
{
    Texture texture;
    texture.type_ = type;
    texture.path_ = file;
    glGenTextures(1, &texture.id_);

    std::string fileName = directory + '/' + file;
    sf::Image image;
    if(!image.loadFromFile(fileName))
    {
        std::cout << "Failed to load texture at: " << fileName << std::endl;
        exit(-1);
    }
    glBindTexture(GL_TEXTURE_2D, texture.id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

bool aik::Texture::operator==(const std::string &right) const
{
    return this->path_ == right;
}
