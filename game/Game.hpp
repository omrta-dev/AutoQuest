//
// Created by omar on 2/19/20.
//

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>

#include <entt/entt.hpp>
#include <systems/SettingsSystem.hpp>
#include <systems/RenderSystem.hpp>
#include <systems/InputSystem.hpp>
#include <resource/Font.hpp>

class Game
{
public:
    Game();
    void startGame();
private:
    void initializeResources();
    void gameLoop();
    void processInput(const float& dt);
    void update(const float& dt);
    void render();
    void close();
    void updatePreferences();
    void createHGTMeshes();

    int gladStatus = -99;
    sf::RenderWindow window_;
    sf::Clock clock_;
    entt::registry registry_;
    aik::SettingsSystem settingsSystem_;
    aik::RenderSystem renderSystem_;
    aik::InputSystem inputSystem_;
    // Resource Loaders
    entt::cache<aik::resource::Shader> shaderManager_;
    entt::cache<aik::resource::RenderTarget> bufferManager_;
};
