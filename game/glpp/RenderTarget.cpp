//
// Created by omar on 5/21/20.
//
#include "RenderTarget.hpp"

void aik::RenderTarget::allocate(aik::BufferTarget bufferTarget, GLsizeiptr size, GLenum usage)
{
    bindBuffer(bufferTarget);
    glBufferData(static_cast<GLuint>(bufferTarget), size, nullptr, usage);
}

void aik::RenderTarget::createBuffer(aik::BufferTarget bufferTarget)
{
    if(bufferTarget == aik::BufferTarget::VERTEX_ARRAY)
    {
        glGenVertexArrays(1, &vao_);
    }
    else if(bufferTarget == aik::BufferTarget::VERTEX_BUFFER)
    {
        glGenBuffers(1, &vbo_);
    }
    else if(bufferTarget == aik::BufferTarget::ELEMENT_ARRAY)
    {
        glGenBuffers(1, &ebo_);
    }
}

void aik::RenderTarget::bindBuffer(BufferTarget bufferTarget) const
{
    if(bufferTarget == aik::BufferTarget::VERTEX_ARRAY)
    {
        glBindVertexArray(vao_);
    }
    else if(bufferTarget == aik::BufferTarget::VERTEX_BUFFER)
    {
        glBindBuffer(static_cast<GLuint>(bufferTarget), vbo_);
    }
    else if(bufferTarget == aik::BufferTarget::ELEMENT_ARRAY)
    {
        glBindBuffer(static_cast<GLuint>(bufferTarget), ebo_);
    }
}

void aik::RenderTarget::setupVertexAttributes()
{
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
