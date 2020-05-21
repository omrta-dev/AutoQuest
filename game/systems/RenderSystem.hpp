//
// Created by omar on 5/21/20.
//

#pragma once
#include <entt/entt.hpp>

namespace aik
{
    class RenderSystem
    {
    public:
        RenderSystem(entt::registry * registry);
    private:
        entt::registry * registry_;
    };
}
