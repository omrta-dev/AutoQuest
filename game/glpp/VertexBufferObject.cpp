//
// Created by omar on 2/23/20.
//

#include "VertexBufferObject.hpp"

aik::VertexBufferObject::VertexBufferObject() : vertexBufferObject_(0)
{
    bufferTarget_ = BufferTarget::ARRAY;
}

aik::VertexBufferObject::VertexBufferObject(aik::BufferTarget bufferTarget) : vertexBufferObject_(0), bufferTarget_(bufferTarget)
{
}

void aik::VertexBufferObject::createVertexBufferObject(aik::BufferTarget bufferTarget)
{
    bufferTarget_ = bufferTarget;
    glGenBuffers(1, &vertexBufferObject_);
}

void aik::VertexBufferObject::bind()
{
    glBindBuffer(static_cast<GLuint>(bufferTarget_), vertexBufferObject_);
}

void aik::VertexBufferObject::setData(const std::vector<aik::Vertex> &data)
{
    glBufferData(static_cast<GLuint>(bufferTarget_), data.size() * sizeof(aik::Vertex), data.data(), GL_STATIC_DRAW);
}

void aik::VertexBufferObject::setData(const std::vector<unsigned int> &data)
{
    glBufferData(static_cast<GLuint>(bufferTarget_), data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);
}

void aik::VertexBufferObject::addData(GLintptr offset, const std::vector<aik::Vertex> &data)
{
    glBufferSubData(static_cast<GLuint>(bufferTarget_), offset, data.size() * sizeof(data[0]), data.data());
}
