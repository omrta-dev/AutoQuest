//
// Created by omar on 5/21/20.
//

#pragma once
#include <entt/entt.hpp>
#include <glad/glad.h>
#include "glpp/RenderTarget.hpp"

#include <SFML/Graphics/Shader.hpp>

namespace aik
{
    class RenderSystem
    {
    public:
        explicit RenderSystem(entt::registry * registry);
        void initialize();
        void update(float dt);
        void render();
        entt::entity createSquare();
    private:
        void renderUi();
        void renderGame();

        entt::registry * registry_;
        RenderTarget uiRenderTarget_{};
        RenderTarget gameRenderTarget_{};
        unsigned long int lastIndex_ = 0;
        unsigned int lastBufferIndex_ = 0;
        sf::Shader vert, frag;

    };
}
