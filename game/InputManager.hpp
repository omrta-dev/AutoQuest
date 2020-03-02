//
// Created by omar on 2/19/20.
//

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_set>
#include "InputEvent.hpp"


namespace aik
{
    enum class InputMode {POLL, READ};

    class InputManager
    {
    public:
        InputManager();
        void addEvent(aik::InputEvent inputEvent);
        void processEvents(sf::Event event);
        void setInputMode(InputMode poll);
    private:
        std::unordered_map<unsigned int, std::vector<aik::InputEvent>> inputEvents_;
        InputMode inputMode_;
    };
}

