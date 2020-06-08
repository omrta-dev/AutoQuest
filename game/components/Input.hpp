//
// Created by omar on 5/31/20.
//

#pragma once
#include <functional>
#include <SFML/Window/Event.hpp>
#include <limits>

namespace aik::Component
{
    class Input
    {
    public:
        std::function<void()> action;
        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(false); // this represents an empty type
        sf::Joystick::Axis axis = static_cast<sf::Joystick::Axis>(false);
        bool poll = false;
        float duration = std::numeric_limits<float>::max();
        float durationCounter = std::numeric_limits<float>::max();
    };
}
