//
// Created by omar on 2/23/20.
//

#pragma once
#include <glad/glad.h>

namespace aik
{
    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        void createVertexArrayObject();
        void configureAttribs();
        void bind();
    private:
        GLuint vertexArrayObject_;
    };
}
