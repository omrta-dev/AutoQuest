//
// Created by omar on 5/21/20.
//

#pragma once
#include <entt/entt.hpp>
#include <glad/glad.h>
#include <SFML/Graphics/Shader.hpp>

#include "components/Renderable.hpp"
#include "glpp/RenderTarget.hpp"

namespace aik
{
    class RenderSystem
    {
    public:
        explicit RenderSystem(entt::registry * registry);
        void initialize();
        void update(float dt);
        void render();
        entt::entity createSprite(aik::RenderTarget* renderTarget, aik::Shader* shader);
    private:
        void renderUi();
        void renderGame();

        entt::registry * registry_;
        RenderTarget * lastBufferIndex_ = nullptr;
    };
}
