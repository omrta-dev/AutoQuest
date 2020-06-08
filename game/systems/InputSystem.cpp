//
// Created by omar on 5/20/20.
//

#include <SFML/System/Time.hpp>
#include <components/Transform.hpp>
#include "Camera.hpp"
#include "components/Renderable.hpp"
#include "InputSystem.hpp"
#include "Utility/CoordinateHelper.hpp"

aik::InputSystem::InputSystem(entt::registry* registry) :  window_(nullptr), registry_(registry)
{
}

void aik::InputSystem::processKeyboard(const sf::Event * event, const float& dt)
{
    const auto& inputView = registry_->view<aik::Component::Input>();
    for(const auto& inputId : inputView)
    {
        auto& input = registry_->get<aik::Component::Input>(inputId);
        if(event && input.poll && event->key.code == input.key)
        {
                input.action();
                return;
        }
        else // query don't poll
        {
            if(sf::Keyboard::isKeyPressed(input.key))
            {
                input.action();
            }
        }
    }
}

void aik::InputSystem::processKeyHeld(aik::Component::Input &input, const float &dt)
{

}

void aik::InputSystem::processMouse(const sf::Event* event, const float& dt)
{
    const auto& clickableView = registry_->view<aik::Component::Clickable>();
    for(const auto& clickableId : clickableView)
    {
        auto& clickable = registry_->get<aik::Component::Clickable>(clickableId);
        auto cameraView = registry_->view<aik::Camera>();
        auto& camera = registry_->get<aik::Camera>(cameraView.front());
        auto mousePos = sf::Mouse::getPosition(*window_);
        auto flippedMousePos = glm::vec2{mousePos.x, static_cast<int>(window_->getSize().y) - mousePos.y};
        auto& transformable = registry_->get<aik::Component::Transform>(clickableId);

        if(event && event->mouseButton.button == clickable.button && event->type == clickable.type) // button matches event type
        {
            // check for collision in screen->world coordinates for the clickable that was clicked
            if(aik::utility::CoordinateHelper::pointIntersectsBox({flippedMousePos.x, flippedMousePos.y},
                    {transformable.position.x, transformable.position.y, transformable.scale.x, transformable.scale.y}, camera) &&
                    processMouseHeld(clickable, dt))
            {
                clickable.action();
                return;
            }
        }
        // check if the button was "released(is the opposite of the expected event) to reset the held timer"
        else if(event && event->type == (clickable.type == sf::Event::MouseButtonPressed ? sf::Event::MouseButtonReleased : sf::Event::MouseButtonPressed))
        {
            clickable.heldDuration = 0.0f;
        }
        else // button is being queried instead of processed from the queue
        {
            if (sf::Mouse::isButtonPressed(clickable.button) && clickable.duration != std::numeric_limits<float>::max())
            {
                // check for collision in screen->world coordinates for the clickable that was clicked
                if(aik::utility::CoordinateHelper::pointIntersectsBox({flippedMousePos.x, flippedMousePos.y},
                        {transformable.position.x, transformable.position.y, transformable.scale.x, transformable.scale.y}, camera) &&
                        processMouseHeld(clickable, dt) && clickable.type == sf::Event::MouseButtonPressed)
                {
                    clickable.action();
                    return;
                }
            }
        }
    }
}

bool aik::InputSystem::processMouseHeld(aik::Component::Clickable& clickable, const float& dt)
{
    std::cout << clickable.heldDuration << "/" << clickable.duration << std::endl;
    if(clickable.heldDuration == std::numeric_limits<float>::max())
    {
        return true;
    }

    if(clickable.heldDuration >= clickable.duration)
    {
        clickable.heldDuration = 0.0f;
        return true;
    }

    clickable.heldDuration += dt;
    return false;
}

void aik::InputSystem::processJoystick(const sf::Event &event, const float& dt)
{

}

void aik::InputSystem::createKeyInput(const std::function<void()>& action, sf::Keyboard::Key key, bool poll, float duration)
{
    auto input = registry_->create();
    registry_->emplace<aik::Component::Input>(input, action, key, static_cast<sf::Joystick::Axis>(false), poll, duration, sf::seconds(duration).asSeconds());
}

void aik::InputSystem::createJoyStickInput(const std::function<void()> &action, sf::Joystick::Axis axis, bool poll, float duration)
{
    auto input = registry_->create();
    registry_->emplace<aik::Component::Input>(input, action, static_cast<sf::Keyboard::Key>(false), axis, poll, duration, duration);
}
