//
// Created by omar on 5/20/20.
//

#include <components/Transform.hpp>
#include "Camera.hpp"
#include "components/Renderable.hpp"
#include "InputSystem.hpp"
#include "Utility/CoordinateHelper.hpp"

aik::InputSystem::InputSystem(entt::registry* registry) :  window_(nullptr), registry_(registry),
        inputView_(registry_->view<aik::Component::Input>()),
        clickableView_(registry_->view<aik::Component::Clickable>())
{
}

void aik::InputSystem::processKeyboard(const sf::Event &event, float dt)
{
    for(const auto& inputId : inputView_)
    {
        auto& input = registry_->get<aik::Component::Input>(inputId);
        if(input.poll && event.key.code == input.key)
        {
            input.action();
        }
    }
}

void aik::InputSystem::processMouse(const sf::Event &event, float dt)
{
    for(const auto& clickableId : clickableView_)
    {
        auto& clickable = registry_->get<aik::Component::Clickable>(clickableId);
        if(event.mouseButton.button == clickable.button)
        {
            auto cameraView = registry_->view<aik::Camera>();
            auto& camera = registry_->get<aik::Camera>(cameraView.front());
            auto mousePos = sf::Mouse::getPosition(*window_);
            auto flippedMousePos = glm::vec2{mousePos.x, static_cast<int>(window_->getSize().y) - mousePos.y};
            auto& transformable = registry_->get<aik::Component::Transform>(clickableId);
            // check for collision in screen->world coordinates for the clickable that was clicked
            if(aik::utility::CoordinateHelper::pointIntersectsBox({flippedMousePos.x, flippedMousePos.y},
                    {transformable.position.x, transformable.position.y, transformable.scale.x, transformable.scale.y}))
            {
                clickable.action();
            }
        }
    }
}

void aik::InputSystem::processJoystick(const sf::Event &event, float dt)
{

}

void aik::InputSystem::createKeyInput(const std::function<void()>& action, sf::Keyboard::Key key, bool poll,
        float duration)
{
    auto input = registry_->create();
    registry_->emplace<aik::Component::Input>(input, action, key, static_cast<sf::Mouse::Button>(false), static_cast<sf::Joystick::Axis>(false), poll, duration);
    inputView_ = registry_->view<aik::Component::Input>();
}

void aik::InputSystem::createMouseInput(const std::function<void()> &action, sf::Mouse::Button mButton, bool poll,
                                        float duration)
{
    auto input = registry_->create();
    registry_->emplace<aik::Component::Input>(input, action, static_cast<sf::Keyboard::Key>(false), mButton, static_cast<sf::Joystick::Axis>(false), poll, duration);
    clickableView_ = registry_->view<aik::Component::Clickable>();
}

void aik::InputSystem::createJoyStickInput(const std::function<void()> &action, sf::Joystick::Axis axis, bool poll,
                                           float duration)
{
    auto input = registry_->create();
    registry_->emplace<aik::Component::Input>(input, action, static_cast<sf::Keyboard::Key>(false), static_cast<sf::Mouse::Button>(false), axis, poll, duration);
}
