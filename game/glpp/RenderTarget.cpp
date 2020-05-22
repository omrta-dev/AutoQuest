//
// Created by omar on 5/21/20.
//
#include "RenderTarget.hpp"

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

void aik::RenderTarget::bindBuffer(BufferTarget bufferTarget)
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
