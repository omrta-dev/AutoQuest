//
// Created by omar on 2/23/20.
//

#pragma once
#include <vector>
#include <glad/glad.h>
#include "Vertex.hpp"

namespace aik
{
    enum class BufferTarget : GLuint {ARRAY = GL_ARRAY_BUFFER, ELEMENT_BUFFER = GL_ELEMENT_ARRAY_BUFFER};

    // C++ OpenGL VBO + EBO Wrapper
    class VertexBufferObject
    {
    public:
        VertexBufferObject();
        VertexBufferObject(BufferTarget bufferTarget);
        void allocate(unsigned int size);
        void setTarget(BufferTarget bufferTarget) {bufferTarget_ = bufferTarget;}
        void createVertexBufferObject(aik::BufferTarget bufferTarget = BufferTarget::ARRAY);
        void bind();
        void setData(const std::vector<aik::Vertex>& data);
        void setData(const std::vector<unsigned int>& data);
        void addData(GLintptr offset, const std::vector<aik::Vertex>& data);
        void addData(GLintptr offset, const std::vector<unsigned int> &data);
    private:
        GLuint vertexBufferObject_;
        BufferTarget bufferTarget_;
        unsigned long maxSize_;
        unsigned long currentSize_;

    };
}
