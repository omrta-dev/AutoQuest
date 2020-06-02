//
// Created by omar on 5/20/20.
//
#pragma once
#include <entt/entt.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "components/Clickable.hpp"
#include "components/Input.hpp"

namespace aik
{
    class InputSystem
    {
    public:
        explicit InputSystem(entt::registry * registry);
        inline void setWindow(sf::RenderWindow * window) {window_ = window;}
        void processKeyboard(const sf::Event& event, float dt);
        void processMouse(const sf::Event& event, float dt);
        void processJoystick(const sf::Event& event, float dt);

        void createKeyInput(const std::function<void()>& action, sf::Keyboard::Key key, bool poll = false, float duration = std::numeric_limits<float>::max());
        void createMouseInput(const std::function<void()>& action, sf::Mouse::Button mButton, bool poll = false, float duration = std::numeric_limits<float>::max());
        void createJoyStickInput(const std::function<void()>& action, sf::Joystick::Axis axis, bool poll = false, float duration = std::numeric_limits<float>::max());
    private:
        sf::RenderWindow * window_;
        entt::registry * registry_;
        entt::basic_view<entt::entity, entt::exclude_t<>, aik::Component::Input> inputView_;
        entt::basic_view<entt::entity, entt::exclude_t<>, aik::Component::Clickable> clickableView_;
    };
}
