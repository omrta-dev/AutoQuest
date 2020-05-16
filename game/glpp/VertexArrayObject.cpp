//
// Created by omar on 2/23/20.
//

#include "VertexArrayObject.hpp"
#include "Vertex.hpp"

aik::VertexArrayObject::VertexArrayObject() : vertexArrayObject_(0)
{
}

void aik::VertexArrayObject::createVertexArrayObject()
{
    glGenVertexArrays(1, &vertexArrayObject_);
}

void aik::VertexArrayObject::bind()
{
    glBindVertexArray(vertexArrayObject_);
}

// configures the weaving of attributes as we send the vbo data to the GPU
void aik::VertexArrayObject::configureAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(aik::Vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(aik::Vertex, uv)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(aik::Vertex, normal)));
}