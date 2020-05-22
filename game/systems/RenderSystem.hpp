//
// Created by omar on 5/21/20.
//

#pragma once
#include <entt/entt.hpp>
#include <glad/glad.h>

namespace aik
{
    class RenderSystem
    {
    public:
        RenderSystem(entt::registry * registry);
    private:
        entt::registry * registry_;
        GLuint uiVao_;
        GLuint gameVao_;
        GLuint
    };
}
