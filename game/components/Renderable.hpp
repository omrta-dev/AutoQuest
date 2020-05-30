//
// Created by omar on 5/22/20.
//
#pragma once
#include <vector>
#include <glm/vec4.hpp>
#include <glpp/RenderTarget.hpp>
#include <glpp/Shader.hpp>

namespace aik::Component
{
    class Renderable
    {
    public:
        std::vector<glm::vec4> vertices; // a vertex is position.x, position.y and uv.x, uv.y
        unsigned long indices; // the count of the indices for this Renderable
        void* offset = nullptr; // this points to the offset of the first index in the EBO
        RenderTarget* buffers = nullptr; // pointer to the buffers that contain the data for this renderable
        Shader* shader = nullptr; // pointer to the shader that will be used to render this renderable
        bool isVisible = true; // this flag determines if the renderable should be drawn
        GLenum drawMode = GL_TRIANGLES; // draw mode of the renderable
        unsigned long int indexOffset = 0; // this stores the largest indices index to properly offset the buffer
    };
}