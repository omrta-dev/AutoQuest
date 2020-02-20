//
// Created by omar on 2/19/20.
//

#include "InputManager.hpp"

aik::InputManager::InputManager(std::shared_ptr<sf::RenderWindow> window)
{
    window_ = window;
}

void aik::InputManager::processEvents(sf::Event event)
{
    if(event.type == sf::Event::Closed)
    {
        window_->close();
    }
}
