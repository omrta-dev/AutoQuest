//
// Created by omar on 2/29/20.
//
#pragma once

#include <SFML/Window/Event.hpp>

namespace aik
{
    struct InputEvent
    {
        InputEvent(sf::Event::EventType type, unsigned int code, std::function<void()> function)
        {
            trigger = type;
            filter = code;
            action = function;
        }

        sf::Event::EventType trigger;
        unsigned int filter;
        std::function<void()> action;
    };
}


