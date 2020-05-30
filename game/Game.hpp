//
// Created by omar on 2/19/20.
//

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <entt/entt.hpp>
#include "systems/SettingsSystem.hpp"
#include "systems/RenderSystem.hpp"

class Game
{
public:
    Game();
    void startGame();
private:
    void initializeResources();
    void gameLoop();
    void processInput();
    void update();
    void render();
    void close();
    void updatePreferences();

    int gladStatus = -99;
    sf::RenderWindow window_;
    entt::registry registry_;
    aik::SettingsSystem settingsSystem_;
    aik::RenderSystem renderSystem_;

    // Resource Loaders
    entt::cache<aik::Shader> shaderManager_;
    entt::cache<aik::RenderTarget> bufferManager_;
};

