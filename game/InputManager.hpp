//
// Created by omar on 2/19/20.
//

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>


namespace aik
{
    class InputManager
    {
    public:
        InputManager(std::shared_ptr<sf::RenderWindow> window);
        void processEvents(sf::Event event);
    private:
        std::shared_ptr<sf::RenderWindow> window_;
    };
}
