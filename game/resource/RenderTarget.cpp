//
// Created by omar on 5/21/20.
//
#include "RenderTarget.hpp"

void aik::resource::RenderTarget::allocate(aik::resource::BufferTarget bufferTarget, GLsizeiptr size, GLenum usage)
{
    bindBuffer(bufferTarget);
    glBufferData(static_cast<GLuint>(bufferTarget), size, nullptr, usage);
}

void aik::resource::RenderTarget::createBuffer(aik::resource::BufferTarget bufferTarget)
{
    if(bufferTarget == aik::resource::BufferTarget::VERTEX_ARRAY)
    {
        glGenVertexArrays(1, &vao_);
    }
    else if(bufferTarget == aik::resource::BufferTarget::VERTEX_BUFFER)
    {
        glGenBuffers(1, &vbo_);
    }
    else if(bufferTarget == aik::resource::BufferTarget::ELEMENT_ARRAY)
    {
        glGenBuffers(1, &ebo_);
    }
}

void aik::resource::RenderTarget::bindBuffer(BufferTarget bufferTarget, bool unbind)
{
    if(bufferTarget == aik::resource::BufferTarget::VERTEX_ARRAY)
    {
        if(!vaoBound_)
        {
            glBindVertexArray(unbind ? 0 : vao_);
            vaoBound_ = !unbind;
        }
    }
    else if(bufferTarget == aik::resource::BufferTarget::VERTEX_BUFFER)
    {
        if (!vboBound_)
        {
            glBindBuffer(static_cast<GLuint>(bufferTarget), unbind ? 0 : vbo_);
            vboBound_ = !unbind;
        }
    }
    else if(bufferTarget == aik::resource::BufferTarget::ELEMENT_ARRAY)
    {
        if (!eboBound_)
        {
            glBindBuffer(static_cast<GLuint>(bufferTarget), unbind ? 0 : ebo_);
            eboBound_ = !unbind;
        }
    }
}

void aik::resource::RenderTarget::setupVertexAttributes()
{
    bindBuffer(BufferTarget::VERTEX_ARRAY);
    bindBuffer(BufferTarget::VERTEX_BUFFER);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

// buffers are allocated enough for 16384 sprites, more than what we should ever need, but we can allocate more if needed
std::shared_ptr<aik::resource::RenderTarget> aik::resource::RenderTarget::load() const
{
    auto renderTarget = std::make_shared<aik::resource::RenderTarget>();
    renderTarget->createBuffer(BufferTarget::VERTEX_ARRAY);
    renderTarget->createBuffer(BufferTarget::VERTEX_BUFFER);
    renderTarget->createBuffer(BufferTarget::ELEMENT_ARRAY);
    renderTarget->allocate(BufferTarget::VERTEX_BUFFER, 1024*1024, GL_STATIC_DRAW);
    renderTarget->setupVertexAttributes();
    renderTarget->allocate(BufferTarget::ELEMENT_ARRAY, 1024*1024, GL_STATIC_DRAW);
    return renderTarget;
}