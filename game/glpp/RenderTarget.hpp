//
// Created by omar on 5/21/20.
//
#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>

namespace aik
{
    enum class BufferTarget : unsigned int {VERTEX_ARRAY = GL_VERTEX_ARRAY, VERTEX_BUFFER = GL_ARRAY_BUFFER, ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER};

    class RenderTarget
    {
    public:
        void allocate(BufferTarget bufferTarget, GLsizeiptr size, GLenum usage);
        void createBuffer(BufferTarget bufferTarget);
        void bindBuffer(BufferTarget bufferTarget) const;
        template <class T> void addBufferData(BufferTarget bufferTarget, std::vector<T> data, GLintptr offset = UINT64_MAX);
        void setupVertexAttributes();
        GLuint vao_;
        GLuint vbo_;
        GLuint ebo_;
        unsigned long int vboCurrentSize_;
        unsigned long int eboCurrentSize_;
    };

    template<class T>
    void RenderTarget::addBufferData(BufferTarget bufferTarget, std::vector<T> data, GLintptr offset)
    {
        if(offset != UINT64_MAX)
        {
            glBufferSubData(static_cast<GLuint>(bufferTarget), offset, data.size() * sizeof(data[0]), data.data());
        }
        else
        {
            glBufferSubData(static_cast<GLuint>(bufferTarget),
                            bufferTarget == BufferTarget::VERTEX_BUFFER ? vboCurrentSize_ : eboCurrentSize_,
                            data.size() * sizeof(data[0]), data.data());
        }
        bufferTarget == BufferTarget::VERTEX_BUFFER ? vboCurrentSize_+= data.size() * sizeof(data[0]) : eboCurrentSize_ += data.size() * sizeof(data[0]);
    }
}
