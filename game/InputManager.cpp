//
// Created by omar on 2/19/20.
//

#include "InputManager.hpp"
#include <iostream>
aik::InputManager::InputManager() : inputMode_(InputMode::READ)
{

}

void aik::InputManager::addEvent(aik::InputEvent inputEvent)
{
    auto found = inputEvents_.find(inputEvent.trigger);
    if(found != inputEvents_.end())
    {
        inputEvents_[inputEvent.trigger].push_back(inputEvent);
    }
    else
    {
        inputEvents_.insert({inputEvent.trigger, {inputEvent}});
    }
}

void aik::InputManager::processEvents(sf::Event event)
{
    auto found = inputEvents_.find(event.type);
    if(found != inputEvents_.end())
    {
        for(const auto& evnt : inputEvents_[event.type])
        {
            if(inputMode_ == InputMode::READ)
            {
                if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(evnt.filter)))
                {
                    evnt.action();
                }
            }
            else if(inputMode_ == InputMode::POLL)
            {
                if(event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == evnt.filter)
                    {
                        evnt.action();
                        return;
                    }
                }
                else if(event.type == sf::Event::JoystickButtonPressed)
                {
                }
                else if(event.type == sf::Event::MouseWheelScrolled)
                {
                    if(evnt.filter == 0 && event.mouseWheelScroll.delta < 0)
                    {
                        evnt.action();
                        return;
                    }
                    else if(evnt.filter == 1 && event.mouseWheelScroll.delta > 0)
                    {
                        evnt.action();
                        return;
                    }
                }
            }
        }
    }
}

void aik::InputManager::setInputMode(InputMode poll)
{
    inputMode_ = poll;
}
