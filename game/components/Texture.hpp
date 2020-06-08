//
// Created by omar on 6/3/20.
//
#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <SFML/Graphics/Image.hpp>

namespace aik::Component
{
    class Texture
    {
    public:
        Texture(GLenum t)
        {
            type = t;
        }
        inline void createTexture(const std::string& filePath)
        {
            sf::Image image;
            if (image.loadFromFile(filePath))
            {
                glGenTextures(1, &id);
                glBindTexture(type, id);
                glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexImage2D(type, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
                glGenerateMipmap(type);
                glBindTexture(type, 0);
            }
        }

        inline void createTextureArray(const std::string& filePath)
        {
            sf::Image image;
            if(image.loadFromFile(filePath))
            {
                glGenTextures(1, &id);
                glBindTexture(type, id);
                glTexParameteri(type, GL_TEXTURE_WRAP_S, textureWrapS);
                glTexParameteri(type, GL_TEXTURE_WRAP_T, textureWrapT);
                glTexParameteri(type, GL_TEXTURE_MIN_FILTER, minFilter);
                glTexParameteri(type, GL_TEXTURE_MAG_FILTER, magFilter);
                glTexStorage3D(type, 2, GL_RGB8, image.getSize().x, image.getSize().y, 2);
                width = image.getSize().x;
                height = image.getSize().y;

                addImageToArray(0, 0, 0, image.getPixelsPtr());
                image.flipVertically();
                addImageToArray(0, 0, 1, image.getPixelsPtr());
                glGenerateMipmap(type);
            }
        }

        inline void addImageToArray(GLuint xOffset, GLuint yOffset, GLuint zOffset, const sf::Uint8* imgPtr)
        {
            glTexSubImage3D(type,
                            0, // mip
                            xOffset, yOffset, zOffset,    // x,y,z offset
                            width, height, depth, // width, height, depth
                            GL_RGBA, GL_UNSIGNED_BYTE, imgPtr);
        }
        GLuint width = 64;
        GLuint height = 64;
        GLuint depth = 1;
        GLenum type = GL_TEXTURE_2D_ARRAY;
        glm::vec4 borderColor = glm::vec4(1.0f);
        GLint textureWrapS = GL_REPEAT;
        GLint textureWrapT = GL_REPEAT;
        GLint minFilter = GL_NEAREST;
        GLint magFilter = GL_NEAREST;
        GLuint id;
    };
}
