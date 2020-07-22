//
// Created by omar on 5/21/20.
//
#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <entt/resource/loader.hpp>

namespace aik::resource
{
    enum class BufferTarget : unsigned int {VERTEX_ARRAY = GL_VERTEX_ARRAY, VERTEX_BUFFER = GL_ARRAY_BUFFER, ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER};

class RenderTarget final: public entt::loader<aik::resource::RenderTarget, aik::resource::RenderTarget>
    {
    public:
        void allocate(BufferTarget bufferTarget, GLsizeiptr size = 1024 - 1024, GLenum usage = GL_STATIC_DRAW);
        void createBuffer(BufferTarget bufferTarget);
        void bindBuffer(BufferTarget bufferTarget, bool unbind = false);
        template <class T> void addBufferData(BufferTarget bufferTarget, std::vector<T> data, GLintptr offset = UINT64_MAX);
        void setupVertexAttributes();
        [[nodiscard]] std::shared_ptr<aik::resource::RenderTarget> load() const;
        GLuint vao_;
        GLuint vbo_;
        GLuint ebo_;
        unsigned long long vboCurrentSize_;
        unsigned long long eboCurrentSize_;
        bool vaoBound_ = false;
        bool vboBound_ = false;
        bool eboBound_ = false;
        unsigned long long lastIndex_ = 0;
    };

    template<class T>
    void RenderTarget::addBufferData(BufferTarget bufferTarget, std::vector<T> data, GLintptr offset)
    {
        bindBuffer(bufferTarget);
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
