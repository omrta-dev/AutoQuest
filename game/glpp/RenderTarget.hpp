//
// Created by omar on 5/21/20.
//

#pragma once
#include <glad/glad.h>
#include <vector>
namespace aik
{
    enum class BufferTarget : unsigned int {VERTEX_ARRAY = UINT32_MAX, VERTEX_BUFFER = GL_ARRAY_BUFFER, ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER};

    class RenderTarget
    {
    public:
        void createBuffer(BufferTarget bufferTarget);
        void bindBuffer(BufferTarget bufferTarget);
        template <class T> void addBufferData(BufferTarget bufferTarget, std::vector<T> data, GLintptr offset = UINT64_MAX);
    private:
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
            glBufferSubData(static_cast<GLuint>(bufferTarget), currentSize_, data.size() * sizeof(data[0]), data.data());
        }
        currentSize_ += data.size() * sizeof(data[0]);
    }
}
