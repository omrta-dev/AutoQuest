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

void aik::VertexArrayObject::configureAttribs()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(aik::Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(aik::Vertex, color)));
    glEnableVertexAttribArray(1);
}
