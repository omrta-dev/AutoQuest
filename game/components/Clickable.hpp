//
// Created by omar on 5/22/20.
//
#pragma once
#include <functional>

namespace aik::Component
{
    class Clickable
    {
    public:
        std::function<void()> action;
        sf::Mouse::Button button;
        float duration = std::numeric_limits<float>::max();
        float heldDuration = 0.0f;
        sf::Event::EventType type = sf::Event::MouseButtonPressed;
    };
}